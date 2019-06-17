<?php
/**
 * @file
 * mcstat.inc library for reporting memcache daemon statistics
 *
 * @requires PHP CLI 5.2 or newer.
 *
 * Copyright 2011 Jonathan W. Webb
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

define('DEFAULT_MEMCACHE_HOSTNAME', 'localhost');
define('DEFAULT_MEMCACHE_PORT', 11211);
define('DEFAULT_INTERVAL', 5); // seconds
define('DEFAULT_HEADER_INTERVAL', 40); // lines


// Declaration of ticks is a requirement for PCNTL signal handling.
declare(ticks=1);

function mcstat_exception_error_handler($errno, $errstr, $errfile, $errline) {
  throw new ErrorException($errstr, 0, $errno, $errfile, $errline);
}

/**
 * Utility for converting a value to a human readable number in metric units.
 */
function hr_metric($value) {
  if ($value > 1000000000000.0) {
    $value = sprintf("%4.1fT", ($value/1000000000000.0));
  }
  else if ($value > 1000000000) {
    $value = sprintf("%4.1fG", ($value/1000000000.0));
  }
  else if ($value > 1000000) {
    $value = sprintf("%4.1fM", ($value/1000000.0));
  }
  else if ($value > 1000) {
    $value = sprintf("%4.1fk", ($value/1000.0));
  }
  return $value;
}


/**
 * Utility for converting a value to a human readable number in binary units.
 */
function hr_binary($value) {
  if ($value > 1099511627776) {
    $value = sprintf("%4.1fT", ($value/1099511627776.0));
  }
  else if ($value > 1073741824) {
    $value = sprintf("%4.1fG", ($value/1073741824.0));
  }
  else if ($value > 1048576) {
    $value = sprintf("%4.1fM", ($value/1048576.0));
  }
  else if ($value > 1024) {
    $value = sprintf("%4.1fk", ($value/1024.0));
  }
  return $value;
}


/**
 * Utility for converting a number of seconds into human readable time.
 */
function hr_time($seconds) {
  $time_interval = array(
    'd' => 86400,
    'h' => 3600,
    'm' => 60,
  );
  $output = '';
  foreach ($time_interval as $key => $value) {
    $$key = floor($seconds / $value);
    $seconds -= ($$key * $value);
    $output .= ($$key == 0) ? '' : $$key . "$key ";
  }
  return $output . $seconds . 's';
}


/**
 * Memcache protocol client
 */
class mcstat_client
{
  private $memcached = null;
  private $hostname = null;
  private $port = null;
  private $username = null;
  private $password = null;

  function __construct($hostname, $port, $username, $password) {
    if (!extension_loaded('memcached')) {
      throw new Exception("The Memcached PHP extension is not loaded. This extension is required for mcstat to run.");
    }

    $this->memcached = new Memcached();
    $this->hostname = $hostname;
    $this->port = $port;
    $this->username = $username;
    $this->password = $password;
    $this->connect();
  }

  function connect() {
    // Throw an exception if client emits a warning.
    set_error_handler("mcstat_exception_error_handler");
    $this->memcached->addServer($this->hostname, $this->port);
    $this->memcached->setOption(Memcached::OPT_BINARY_PROTOCOL, true);

    if (!is_null($this->username) && !is_null($this->password)) {
      $this->memcached->setSaslAuthData($this->username, $this->password);
    }
    restore_error_handler();
  }

  function disconnect() {
    $this->memcached->quit();
    print 'Connection closed' . PHP_EOL;
  }

  function get_all_stats() {
    return $this->memcached->getStats()["{$this->hostname}:{$this->port}"];
  }

}


class mcstat_reporter
{
  protected $server = null;
  protected $starting_data = null;
  protected $last_data = null;

  function __construct($hostname, $port, $username, $password) {
    $this->last_data = array(
      'cmd_get' => 0,
      'get_hits' => 0,
      'get_misses' => 0,
      'bytes_read' => 0,
      'bytes_written' => 0,
    );
    $this->server = new mcstat_client($hostname, $port, $username, $password);
  }

  function abort($signal = -1) {
    if ($signal === SIGINT || $signal === SIGTERM || $signal === -1) {
      print $this->output_stats_session();
      $this->server->disconnect();
      exit();
    }
  }

  function output_header() {
    return sprintf(
      "%8s  %4s  % 6s  % 6s  % 6s  % 6s  % 6s  % 6s  % 6s  % 6s  % 6s  % 6s" . PHP_EOL,
      'time',
      'cnct',
      'used',
      'gets',
      'hits',
      'miss',
      'sets',
      'evic',
      'miss%',
      'evic%',
      'in',
      'out'
    );
  }

  function output_stats_update() {
    $last = $this->last_data;
    $data = $start = $this->server->get_all_stats();

    $data['time_string'] = strftime("%T", $data['time']);
    $data['get_total'] = $data['cmd_get'] - $last['cmd_get'];
    $data['set_total'] = $data['cmd_set'] - $last['cmd_set'];
    $data['evictions_total'] = $data['evictions'] - $last['evictions'];
    $data['get_hits_total'] = $data['get_hits'] - $last['get_hits'];
    $data['get_misses_total'] = $data['get_misses'] - $last['get_misses'];
    $data['get_hits_percentage'] = (($data['get_total'] > 0) ? (($data['get_hits'] - $last['get_hits'])/$data['get_total']) : 0) * 100.0;
    $data['get_misses_percentage'] = (($data['get_total'] > 0) ? (($data['get_misses'] - $last['get_misses'])/$data['get_total']) : 0) * 100.0;
    $data['set_evictions_percentage'] = (($data['set_total'] > 0) ? ($data['evictions_total']/$data['set_total']) : 0) * 100.0;
    $data['reads_total'] = $data['bytes_read'] - $last['bytes_read'];
    $data['writes_total'] = $data['bytes_written'] - $last['bytes_written'];

    $output = sprintf(
      "%8s  %4d  % 6s  % 6s  % 6s  % 6s  % 6s  % 6s  % 6.2f  % 6.2f  % 6s  % 6s" . PHP_EOL,
      $data['time_string'],
      $data['curr_connections'],
      hr_binary($data['bytes']),
      hr_metric($data['get_total']),
      hr_metric($data['get_hits_total']),
      hr_metric($data['get_misses_total']),
      hr_metric($data['set_total']),
      hr_metric($data['evictions_total']),
      $data['get_misses_percentage'],
      $data['set_evictions_percentage'],
      hr_binary($data['reads_total']),
      hr_binary($data['writes_total'])
    );

    if (!is_array($this->starting_data)) {
      $header = sprintf("Memcache version %s" . PHP_EOL, $start['version']);
      $header .= sprintf("Max Memory Size: %s  Pointer size: %d bit" . PHP_EOL . PHP_EOL, hr_binary($start['limit_maxbytes']), $start['pointer_size']);
      $output = $header . $this->output_header() . $output . PHP_EOL;
      unset($start['version']);
      unset($start['limit_maxbytes']);
      unset($start['pointer_size']);
      unset($start['time']);
      unset($start['pid']);
      $this->starting_data = $start;
      $this->min_connections = $data['curr_connections'];
      $this->max_connections = $data['curr_connections'];
      $this->min_items = $data['curr_items'];
      $this->max_items = $data['curr_items'];
    }

    print $output;

    $this->last_data = $data;
    if ($data['curr_connections'] < $this->min_connections) {
      $this->min_connections = $data['curr_connections'];
    }
    else if ($data['curr_connections'] > $this->max_connections) {
      $this->max_connections = $data['curr_connections'];
    }
    if ($data['curr_items'] < $this->min_items) {
      $this->min_items = $data['curr_items'];
    }
    else if ($data['curr_items'] > $this->max_items) {
      $this->max_items = $data['curr_items'];
    }
  }

  function disconnect() {
    $this->server->disconnect();
  }

  function output_stats_session() {
    $output = PHP_EOL . "Session Statistics:" . PHP_EOL;
    $output .= sprintf("%22s  % 16s  % 16s" . PHP_EOL,
      "Statistic", "Total", "Session"
    );
    $last = $this->last_data;
    $start = $this->starting_data;

    foreach($start as $key => $value) {
      $t_val = $value;
      $s_val = $last[$key] - $start[$key];
      switch ($key) {
      case 'curr_connections':
         $t_val = " ";
         if ($this->min_connections != $this->max_connections) {
           $s_val = sprintf("%d-%d", $this->min_connections, $this->max_connections);
         }
         else {
           $s_val = sprintf("%d", $this->min_connections);
         }
         break;
      case 'curr_items':
         $t_val = " ";
         if ($this->min_items != $this->max_items) {
           $s_val = sprintf("%d-%d", $this->min_items, $this->max_items);
         }
         else {
           $s_val = sprintf("%d", $this->min_items);
         }
         break;
      case 'uptime':
         $t_val = hr_time($t_val);
         $s_val = hr_time($s_val);
         break;
      case 'bytes':
      case 'bytes_read':
      case 'bytes_written':
         $t_val = hr_binary($t_val);
         $s_val = hr_binary($s_val);
         break;
      case 'rusage_user':
      case 'rusage_system':
         $t_val = hr_time(sprintf("%10.2f", $t_val));
         $s_val = hr_time(sprintf("%10.2f", $s_val));
         break;
      default:
         $t_val = hr_metric($t_val);
         $s_val = hr_metric($s_val);
         break;
      }
      $output .= sprintf("%22s  % 16s  % 16s" . PHP_EOL, $key, $t_val, $s_val);
    }

    return $output;
  }
}

/**
 * PHP CLI session handler singleton
 */
class mcstat_session
{
  private static $instance;

  private function __construct() {}

  public function __clone() {
    trigger_error('Clone is not allowed', E_USER_ERROR);
  }

  public function __wakeup() {
    trigger_error('Unserializing is not allowed', E_USER_ERROR);
  }

  public function __shutdown() {
    echo "Exiting." . PHP_EOL;
  }


  public static function main($argv, $argc) {

    if (!isset(self::$instance)) {
      $className = __CLASS__;
      self::$instance = new $className;
    }

    register_shutdown_function(array(self::$instance, '__shutdown'));

    $args = self::parse($argv);

    $hostname = (!empty($args[0])) ? ($args[0]) : DEFAULT_MEMCACHE_HOSTNAME;
    $port = (!empty($args[1])) ? ($args[1]) : DEFAULT_MEMCACHE_PORT;
    $username = (!empty($args[2])) ? ($args[2]) : null;
    $password = (!empty($args[3])) ? ($args[3]) : null;
    $interval = (!empty($args['interval'])) ? ($args['interval']) : DEFAULT_INTERVAL;
    $header_interval = $interval * DEFAULT_HEADER_INTERVAL;

    date_default_timezone_set('UTC');

    try {
      $sess = new mcstat_reporter($hostname, $port, $username, $password);
    
      if ($sess) {
        if (extension_loaded('pcntl')) {
          pcntl_signal(SIGTERM, array($sess, 'abort'));
          pcntl_signal(SIGINT, array($sess, 'abort'));
        }
        else {
          echo "Warning: The pcntl extension is not loaded. No memcache summary will be displayed." . PHP_EOL;
        }
        $start = time();
        $last_header = $start;
        while (1) {
          $current_time = time();
          if (($last_header + $header_interval) <= $current_time) {
            $last_header = $current_time;
            print $sess->output_header();
          }
          $sess->output_stats_update();
          sleep($interval);
        }
        $sess->abort();
      }
      else {
        fprintf(STDERR, "%s: Connection to memcache daemon failed. Error %d: %s\n", $command, $err['errno'], $err['errstr']);
        return $err['errno'];
      }

      $sess->disconnect();
    }
    catch (Exception $e) {
      fprintf(STDERR, '%s' . PHP_EOL, $e->getMessage());
    }
  }

  public static function parse($argv) {
    $out = array();
    $synonym = array(
      'h' => 'help',
      'i' => 'interval',
      '?' => 'help',
    );
    $out['_'] = array_shift($argv);
    for ($i = 0; $i < count($argv); $i++) {
      $arg = $argv[$i];
      if (substr($arg, 0, 2) == '--') {
        $eqPos = strpos($arg, '=');
        if ($eqPos === false) {
          $key = substr($arg, 2);
          $out[$key] = isset($out[$key]) ? $out[$key] : true;
        }
        else {
          $key = substr($arg, 2, $eqPos - 2);
          $out[$key] = substr($arg, $eqPos + 1);
          $key = null;
        }
      }
      else if (substr($arg, 0, 1) == '-') {
        if (substr($arg, 2, 1) == '=') {
          $key = $synonym[substr($arg, 1, 1)];
          $out[$key] = substr($arg, 3);
          $key = null;
        }
        else {
          $chars = str_split(substr($arg, 1));
          foreach ($chars as $char) {
            $key = $synonym[$char];
            $out[$key] = isset($out[$key]) ? $out[$key] : '_';
          }
        }
      }
      else {
        $out[] = $arg;
      }

      if (isset($key) && $key == 'interval') {
        $out['interval'] = $argv[++$i];
        $key = null;
      }
    }
    return $out;
  }
}


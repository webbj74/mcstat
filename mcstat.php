<?php

/**
 * Reports memcache daemon statistics
 */

$hostname = "localhost";
$port = 11211;

function mc_stats($fp) {
  $data = array();
  fwrite($fp, "stats\r\n");
  while ($line = fgets($fp, 128)) {
    if (preg_match('/^END/', $line)) {
      break;
    }
    list($stat,$key,$value) = preg_split('/[\s]+/', $line, 3);
    $data[$key] = $value;
  }
  return $data;
}


$fp = fsockopen($hostname, $port, $errno, $errstr, 30);
if (!$fp) {
  echo "$errstr ($errno)" . PHP_EOL;
}
else {
  $data = array();
  printf("time      cnct        gets        hits        miss      hit%%       miss%%\n");
  $start = mc_stats($fp);
  $stat_time = strftime("%T", (int) $start['time']);
  $data[$stat_time] = $start;
  printf("%s  %4d  %10d  %10d  %10d  %8.2f  %8.2f\n\n",
    $stat_time,
    $start['curr_connections'],
    $start['cmd_get'],
    $start['get_hits'],
    $start['get_misses'],
    (($start['cmd_get'] > 0) ? ($start['get_hits']/$start['cmd_get']) : 0) * 100.0,
    (($start['cmd_get'] > 0) ? ($start['get_misses']/$start['cmd_get']) : 0) * 100.0
  );
  sleep(60);

  for ($i = 0; $i < 5; $i++) {
    $tmp = mc_stats($fp);
    $stat_time = strftime("%T", (int) $tmp['time']);
    $stat_gets = $tmp['cmd_get'] - $start['cmd_get'];
    $data[$stat_time] = $tmp;
    printf("%s  %4d  %10d  %10d  %10d  %8.2f  %8.2f\n",
      $stat_time,
      $tmp['curr_connections'],
      $stat_gets,
      $tmp['get_hits'] - $start['get_hits'],
      $tmp['get_misses'] - $start['get_misses'],
      (($stat_gets > 0) ? (($tmp['get_hits'] - $start['get_hits'])/$stat_gets) : 0) * 100.0,
      (($stat_gets > 0) ? (($tmp['get_misses'] - $start['get_misses'])/$stat_gets) : 0) * 100.0
    );
    sleep(60);
  }
  fwrite($fp, "quit\r\n");
  fclose($fp);
}


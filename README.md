mcstat
======

Reports memcache daemon statistics


Usage
-----

mcstat [hostname [port]]

* hostname - defaults to localhost
* port - defaults to 11211

If you need to use an alternate port number, then you must specify the
hostname


Example
-------

    
    $ ./mcstat hostname-123 11211
    Memcache version 1.4.5
    Max Memory Size: 64M  Pointer size: 64 bit

    time      cnct   gets   hits   miss    hit%   miss%     in    out
    04:19:31     2    12M     9M     2M   75.42   24.58     3G   106G

    04:19:36     2      0      0      0    0.00    0.00      7    878
    04:19:41     2      0      0      0    0.00    0.00      7    878
    04:19:46     2      4      4      0  100.00    0.00    253    29k
    04:19:51     2      0      0      0    0.00    0.00      7    878
    04:19:56     2      0      0      0    0.00    0.00      7    878
    04:20:01     2      0      0      0    0.00    0.00      7    878
    04:20:06     2      8      8      0  100.00    0.00    509    59k
    04:20:11     2     78     63     15   80.77   19.23    28k   158k
    04:20:16     2     49     36     13   73.47   26.53    22k   100k
    04:20:21     2     35     24     11   68.57   31.43    18k    72k
    04:20:26     2      0      0      0    0.00    0.00      7    878
    04:20:31     2      0      0      0    0.00    0.00      7    878
    04:20:36     2     41     27     14   65.85   34.15    17k    80k
    ^C
    Session Statistics:
                 Statistic           Total         Session
                    uptime      2h 13m 22s           1m 5s
               rusage_user        0.001225        0.000112
             rusage_system        0.002938        0.000125
                   cmd_get             12M             215
                   cmd_set               0               0
                 cmd_flush               0               0
                  get_hits               0             162
                get_misses               0              53
             delete_misses               0               0
               delete_hits               0               0
               incr_misses               0               0
                 incr_hits               0               0
               decr_misses               0               0
                 decr_hits               0               0
                cas_misses               0               0
                  cas_hits               0               0
                cas_badval               0               0
                 auth_cmds               0               0
               auth_errors               0               0
                bytes_read              3G             86k
             bytes_written            107G            424k
       listen_disabled_num               0               0
               conn_yields               0               0
                curr_items               0               0
               total_items               0               0
                 evictions               0               0
                 reclaimed               0               0
    Connection closed
    

Note that there will be at least 7 bytes in and around 1k out per iteration
as this is the amount of data traffic due to running the STATS command.


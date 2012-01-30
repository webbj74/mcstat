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

    
    $ mcstat localhost
    Memcache version 1.4.5
    Max Memory Size: 64M  Pointer size: 32 bit
    
        time  cnct   used   gets   hits   miss    hit%   miss%     in    out
    22:12:57     3    52M   140M    92M    48M   65.48   34.52     1G    76G
    
    22:13:02     3    52M     87     52     35   59.77   40.23     5k    43k
    22:13:07     9    52M    242    161     81   66.53   33.47    22k   161k
    22:13:12     4    52M    233    164     69   70.39   29.61    26k   346k
    22:13:17     9    52M    279    180     99   64.52   35.48    16k   175k
    22:13:22     5    52M    226    152     74   67.26   32.74    15k   183k
    22:13:27    12    52M    281    180    101   64.06   35.94    26k   212k
    22:13:32     6    52M    368    250    118   67.93   32.07    26k   246k
    22:13:37     5    52M    358    250    108   69.83   30.17    29k   237k
    22:13:42     8    52M    325    221    104   68.00   32.00    19k   267k
    22:13:47     6    52M    367    250    117   68.12   31.88    71k   309k
    22:13:52     7    52M    234    161     73   68.80   31.20    32k   185k
    22:13:57     8    52M    285    194     91   68.07   31.93    27k   216k
    22:14:02     7    52M    294    197     97   67.01   32.99    38k   208k
    22:14:07     7    52M    347    233    114   67.15   32.85    30k   245k
    22:14:12     7    52M    166    108     58   65.06   34.94    10k   107k
    ^C
    Session Statistics:
                 Statistic           Total         Session
                    uptime   47d 1h 1m 40s          1m 15s
               rusage_user      539.970000  0.0099999999999909
             rusage_system              1k  0.040000000000191
          curr_connections               3               4
         total_connections             20M             390
     connection_structures              67               0
                   cmd_get            140M              4k
                   cmd_set              1M              49
                 cmd_flush               0               0
                  get_hits             92M              2k
                get_misses             48M              1k
             delete_misses              36               0
               delete_hits             15k               0
               incr_misses               7               0
                 incr_hits               0               0
               decr_misses               0               0
                 decr_hits               0               0
                cas_misses               0               0
                  cas_hits               0               0
                cas_badval               0               0
                 auth_cmds               0               0
               auth_errors               0               0
                bytes_read              1G            400k
             bytes_written              1G              3M
           accepting_conns               1               0
       listen_disabled_num               0               0
                   threads               1               0
               conn_yields               0               0
                     bytes             52M              4k
                curr_items             30k               0
               total_items              1M              49
                 evictions            263k              11
                 reclaimed               0               0
    Connection closed
    

Note that there will be at least 7 bytes in and around 1k out per iteration
as this is the amount of data traffic due to running the STATS command.


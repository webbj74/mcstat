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

    
    Memcache version 1.4.5
    Max Memory Size: 1024.0M  Pointer size: 32 bit

        time  cnct    used   gets   hits   miss    hit%   miss%      in     out
    21:15:34    11  911.6M   4.7B   3.2B   1.5B   68.27   31.73  590.7G   11.7T
    
    21:15:39    15  911.6M    620    582     38   93.87    6.13   56.4k    2.2M
    21:15:44    13  911.6M    876    796     80   90.87    9.13   88.6k    2.8M
    21:15:49    15  911.6M    900    853     47   94.78    5.22   77.5k    2.8M
    21:15:54    14  911.6M    934    878     56   94.00    6.00   84.6k    2.9M
    21:15:59    13  911.6M   1.5k   1.4k     67   95.47    4.53  106.5k    5.0M
    21:16:04    14  911.6M   1.1k   1.0k     62   94.34    5.66   91.2k    3.7M
    21:16:08    15  911.6M   1.2k   1.1k     64   94.54    5.46   96.9k    3.2M
    21:16:13    14  911.6M    920    865     55   94.02    5.98   87.5k    3.1M
    21:16:18    12  911.6M    820    770     50   93.90    6.10   67.2k    2.3M
    21:16:23    13  911.6M    886    837     49   94.47    5.53   81.1k    2.9M
    21:16:28    11  911.6M    673    627     46   93.16    6.84   64.0k    1.9M
    21:16:33    10  911.6M    987    928     59   94.02    5.98   81.5k    3.0M
    ^C
    Session Statistics:
                 Statistic             Total           Session
                    uptime  133d 23h 36m 46s               59s
               rusage_user        10m 51.54s                0s
             rusage_system        49m 14.85s             0.01s
          curr_connections                               10-15
         total_connections             56.5M               253
     connection_structures               452                 0
                   cmd_get              4.7B             11.4k
                   cmd_set             37.3M               291
                 cmd_flush                 0                 0
                  get_hits              3.2B             10.7k
                get_misses              1.5B               673
             delete_misses            967.8k                31
               delete_hits              6.1M               105
               incr_misses                 0                 0
                 incr_hits              8.4k                 0
               decr_misses                 0                 0
                 decr_hits                 0                 0
                cas_misses                 0                 0
                  cas_hits                 0                 0
                cas_badval                 0                 0
                 auth_cmds                 0                 0
               auth_errors                 0                 0
                bytes_read            590.7G            982.9k
             bytes_written             11.7T             35.7M
           accepting_conns                 1                 0
       listen_disabled_num                 0                 0
                   threads                 1                 0
               conn_yields                 0                 0
                     bytes            911.6M             -3660
                curr_items                       742303-742306
               total_items             37.3M               291
                 evictions              6.8M               108
                 reclaimed                 0                 0
    Connection closed
    Exiting.
    

Note that there will be at least 7 bytes in and around 1k out per iteration
as this is the amount of data traffic due to running the STATS command.


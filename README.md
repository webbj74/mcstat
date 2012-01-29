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
    [Ctrl-C]
    Aborting
    Connection closed
    

Note that there will be at least 7 bytes in and around 1k out per iteration
as this is the amount of data traffic due to running the STATS command.


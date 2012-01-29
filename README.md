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
    time      cnct        gets        hits        miss      hit%     miss%
    02:16:09     2    12098564     9124479     2974085     75.42     24.58

    02:16:14     2           3           3           0    100.00      0.00
    02:16:19     2           0           0           0      0.00      0.00
    02:16:24     2           0           0           0      0.00      0.00
    02:16:29     2           0           0           0      0.00      0.00
    02:16:34     2          62          54           8     87.10     12.90
    02:16:39     2           0           0           0      0.00      0.00
    02:16:44     2          15          11           4     73.33     26.67
    02:16:49     2           4           4           0    100.00      0.00
    02:16:54     3          26          17           9     65.38     34.62
    02:16:59     2          23          19           4     82.61     17.39
    [Ctrl-C]
    Aborting
    Connection closed
    


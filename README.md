# Find-the-seat.lalalala
It is used to finding the seat where we met next time.

这个c++程序用于找出下一次见面的地点。


使用libcurl技术。用于爬取腾讯的股价。


同时使用了time库函数，用于求出当天时间。


使用了全域散列表技术，利用了年月日和周日和年日建立a和b，同时根据时间和股价求出用于求摸的数primer和p，然后建立年月日股票的各个散列值，然后再对其进行与primer相乘再摸m，最终求出见面地点。
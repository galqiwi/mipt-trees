#set xrange[8:20]
#set yrange[80:100]

set xlabel "Log_2(n)"
set ylabel "average time to insert one value, ticks"

plot 'rbset.data' u 1:2 w l #, 'rbset.data' u 1:3 w l, 'rbset.data' u 1:2 w l

pause -1
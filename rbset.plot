set terminal png font arial 14 size 800,800
set xrange[10:25]
#set yrange[80:100]

set xlabel "Log_2(n)"
set ylabel "average time for action, ticks"

plot 'rbset.data' u 1:2 w l title "insert", 'rbset.data' u 1:3 w l  title "erase", 'rbset.data' u 1:4 w l title "find"

pause -1
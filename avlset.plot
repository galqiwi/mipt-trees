set terminal png font arial 14 size 800,800
set xrange[16:25]
#set yrange[80:100]

set xlabel "Log_2(n)"
set ylabel "average time for action, ticks"

plot 'avlset.data' u 1:2 w l title "insert", 'avlset.data' u 1:3 w l  title "erase", 'avlset.data' u 1:4 w l title "find"

pause -1
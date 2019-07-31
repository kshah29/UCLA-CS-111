#! /usr/bin/gnuplot
# change to: LNXSRV: /usr/bin/gnuplot  MAC: usr/local/bin/gnuplot
#
# purpose:
#	 generate data reduction graphs for the multi-threaded list project
#
# input: lab2b_list.csv
#	1. test name
#	2. # threads
#	3. # iterations per thread
#	4. # lists
#	5. # operations performed (threads x iterations x (ins + lookup + delete))
#	6. run time (ns)
#	7. run time per operation (ns)
# 8. wait time per operation (ns)
#
# output:
# lab2b_1.png ... throughput vs. number of threads for mutex and spin-lock synchronized list operations.
# lab2b_2.png ... mean time per mutex wait and mean time per operation for mutex-synchronized list operations.
# lab2b_3.png ... successful iterations vs. threads for each synchronization method.
# lab2b_4.png ... throughput vs. number of threads for mutex synchronized partitioned lists.
# lab2b_5.png ... throughput vs. number of threads for spin-lock-synchronized partitioned lists.
#
# Note:
#	Managing data is simplified by keeping all of the results in a single
#	file.  But this means that the individual graphing commands have to
#	grep to select only the data they want.
#
#	Early in your implementation, you will not have data for all of the
#	tests, and the later sections may generate errors for missing data.
#

# general plot parameters
set terminal png
set datafile separator ","


# lab2b_1.png ... throughput vs. number of threads for mutex and spin-lock synchronized list operations.
set title "List-1: Throughput vs. Number of Threads"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:26]
set ylabel "Throughput (operations/s)"
set logscale y 10
set output 'lab2b_1.png'

plot \
     "< egrep 'list-none-m,.*,1000,1,' lab2b_list.csv " using ($2):(1000000000/($7)) \
	title 'Mutex-Lock' with linespoints lc rgb 'red', \
     "< egrep 'list-none-s,.*,1000,1,' lab2b_list.csv " using ($2):(1000000000/($7)) \
	title 'Spin-Lock' with linespoints lc rgb 'green'


# lab2b_2.png ... mean time per mutex wait and mean time per operation for mutex-synchronized list operations.
set title "List-2: Mean Mutex-wait and Opertaion-Time vs. # of Threads"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:26]
set ylabel "Time (ns)"
set logscale y 10
set output 'lab2b_2.png'
# note that unsuccessful runs should have produced no output
plot \
     "< grep 'list-none-m,[1-9]*,1000,1,' lab2b_list.csv" using ($2):($8) \
	title 'Mean Lock-Wait' with linespoints lc rgb 'violet', \
     "< grep 'list-none-m,[1-9]*,1000,1,' lab2b_list.csv" using ($2):($7) \
	title 'Opertaion-Time' with linespoints lc rgb 'orange'

# lab2b_3.png ... successful iterations vs. threads for each synchronization method.
# change mutex - m and no lock -none
set title "List-3: Successful Iterations vs. Number of Threads"
  set logscale x 2
  set xrange [0.75:18]
  set xlabel "Threads"
  set ylabel "successful iterations"
  set logscale y 10
  set output 'lab2b_3.png'
  plot \
      "< grep 'list-id-m,[1-9]*,[1-9]*,4,' lab2b_list.csv" using ($2):($3) \
    with points lc rgb "blue" title "Mutex", \
      "< grep 'list-id-s,[1-9]*,[1-9]*,4,' lab2b_list.csv" using ($2):($3) \
    with points lc rgb "red" title "Spin-Lock", \
      "< grep 'list-id-none,[1-9]*,[1-9]*,4,' lab2b_list.csv" using ($2):($3) \
    with points lc rgb "orange" title "unprotected"
  #
  # "no valid points" is possible if even a single iteration can't run
  #


# lab2b_4.png ... throughput vs. number of threads for mutex synchronized partitioned lists.
set title "List-4: Throughput vs. # of Threads for Mutex"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:14]
set ylabel "Throughput (operations/s)"
set logscale y 10
set output 'lab2b_4.png'
plot \
  "< grep 'list-none-m,[0-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
title '1 List' with linespoints lc rgb 'red', \
  "< grep 'list-none-m,[0-9]*,1000,4,' lab2b_list.csv" using ($2):(1000000000/($7)) \
title '4 Lists' with linespoints lc rgb 'green', \
  "< grep 'list-none-m,[0-9]*,1000,8,' lab2b_list.csv" using ($2):(1000000000/($7)) \
title '8 Lists' with linespoints lc rgb 'orange', \
  "< grep 'list-none-m,[0-9]*,1000,16,' lab2b_list.csv" using ($2):(1000000000/($7)) \
title '16 Lists' with linespoints lc rgb 'blue'


# lab2b_5.png ... throughput vs. number of threads for spin-lock-synchronized partitioned lists.
set title "List-5: Throughput vs. # of Threads for Spin-Lock"
set xlabel "Threads"
set logscale x 2
set xrange [0.75:14]
set ylabel "Throughput (operations/s)"
set logscale y
set logscale y 10
set output 'lab2b_5.png'
plot \
  "< grep 'list-none-s,[1-9]*,1000,1,' lab2b_list.csv" using ($2):(1000000000/($7)) \
title '1 List' with linespoints lc rgb 'red', \
  "< grep 'list-none-s,[1-9]*,1000,4,' lab2b_list.csv" using ($2):(1000000000/($7)) \
title '4 Lists' with linespoints lc rgb 'green', \
  "< grep 'list-none-s,[1-9]*,1000,8,' lab2b_list.csv" using ($2):(1000000000/($7)) \
title '8 Lists' with linespoints lc rgb 'orange', \
  "< grep 'list-none-s,[1-9]*,1000,16,' lab2b_list.csv" using ($2):(1000000000/($7)) \
title '16 Lists' with linespoints lc rgb 'blue'

#!/bin/bash

make clean && make sorting

for i in {3..1000};
do
    echo -n "$i "
	./sorting -n $i -a -p 0 | awk '{print $5}' | tr '\n' ' '
    echo ""
done > /tmp/graph.dat


gnuplot <<END
	set terminal pdf
	set output "plots.pdf"
	set zeroaxis
    set title "Array where Every Other Item Needs to be Swapped with the Previous Item"
	set xlabel "Elements"
	set ylabel "Moves"
    plot "/tmp/graph.dat" using 1:2 with lines title "Bubble Sort", \
        "/tmp/graph.dat" using 1:3 w lines title "Heap Sort", \
        "/tmp/graph.dat" using 1:4 w lines title "Quick Sort", \
        "/tmp/graph.dat" using 1:5 w lines title "Shell Sort"
END

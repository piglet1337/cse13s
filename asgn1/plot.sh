#!/bin/bash

make clean && make collatz

for i in {2..10000};
do
	echo -n "$i ";
	./collatz -n $i | wc -l;
done > /tmp/length.dat

for i in {2..10000};
do
	echo -n "$i ";
	./collatz -n $i | sort -n | tail -1;
done > /tmp/max.dat

for i in {2..10000};
do
	echo "$i $(($(./collatz -n $i | sort -n | tail -1)/$i))";
done > /tmp/diff.dat

gnuplot <<END
	set terminal pdf
	set output "plots.pdf"
	set title "Collatz Sequence Lengths"
	set xlabel "n"
	set ylabel "length"
	set zeroaxis
	plot "/tmp/length.dat" with dots title ""
	
	set title "Maximum Collatz Sequence Value"
	set ylabel "value"
	set yrange [0:100000]
	plot "/tmp/max.dat" with dots title ""
	
	set title "Maximum Value Divided by Starting Value"
	set yrange [*:50]
	plot "/tmp/diff.dat" with dots title ""
END

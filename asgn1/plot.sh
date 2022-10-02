#!/bin/bash

make clean && make collatz #rebuilds collatz

for i in {2..10000};
do
	echo -n "$i ";
	./collatz -n $i | wc -l; #gets length of collatz function
done > /tmp/length.dat #fills dat file with n in one column and length in another

for i in {2..10000};
do
	echo -n "$i ";
	./collatz -n $i | sort -n | tail -1; #gets maximum value of collatz function
done > /tmp/max.dat #fills dat file file with n in one column and maximum value in another

for i in {2..10000};
do
	echo "$i $(($(./collatz -n $i | sort -n | tail -1)/$i))"; #gets n followed by the maximum value divided by n
done > /tmp/diff.dat #fills dat file with n in one column and maximum value divided by n in another

for i in {2..10000};
do
	./collatz -n $i | wc -l; #gets length of collatz function
done | sort -n | uniq -c | awk '{print $2,$1}' > /tmp/histogram.dat #fills dat file with length in one column and frequency in another

# here-document that is sent to gnuplot
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

	set title "Collatz Sequence Length Histogram"
	set xlabel "length"
	set ylabel "frequency"
	set yrange [*:*]
	set style fill solid 1.0
	set boxwidth 0.8
	plot "/tmp/histogram.dat" with boxes title ""
END

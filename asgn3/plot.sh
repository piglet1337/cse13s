#!/bin/bash

make clean && make play-dreidel

for i in {1..10000};
do
	./play-dreidel -p 6 -c 4 -s $i | awk '{print $4}' 
done | sort -n | uniq -c | awk '{print $2,$1}' > /tmp/histogram.dat 

for j in {2..8};
do
echo -n "$j "
for i in {1..2000};
do
	./play-dreidel -p $j -c 3 -s $i | awk '{print $4}' 
done | seq 100 | jq -s add/length
done > /tmp/player.dat

for i in {1..30000};
do
	./play-dreidel -p 8 -c 4 -s $i | awk '{print $1}' 
done | sort -n | uniq -c | awk '{print $2,$1}' > position.dat 

#here-document that is sent to gnuplot
gnuplot <<END
	set terminal pdf
	set output "plots.pdf"
	set zeroaxis
    set title "Dreidel Number of Rounds Histogram"
	set xlabel "rounds"
	set ylabel "frequency"
	set style fill solid 1.0
	set boxwidth 0.8
	plot "/tmp/histogram.dat" with boxes title ""

    set title "difference in player count"
    set xlabel "num of players"
    set ylabel "average length"
    plot "/tmp/player.dat" with lines title ""
END

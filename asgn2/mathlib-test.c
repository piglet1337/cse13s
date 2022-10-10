//include necassary libraries.
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "mathlib.h"

//define OPTIONS constant with all available command line arguments.
#define OPTIONS "ascSCTl"

//main function
int main(int argc, char **argv) {
    int opt = 0;
    int not_all = 1;
    //takes comand line arguments and runs associated code.
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        //a: runs all code then returns.
        case 'a':
            not_all = 0;
        //s: prints header and output of my_sin sin and their difference.
        case 's':
            printf("  x\t\tsin\t\tLibrary\t\tDifference\n");
            printf("  -\t\t---\t\t-------\t\t----------\n");
            for (double x = 0.0; x<2*M_PI; x+=0.05*M_PI) {
                double my = my_sin(x);
                double libr = sin(x);
                double diff = (my>libr) ? my-libr : libr-my;
                printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", x, my, libr, diff);
            }
            if (not_all) {break;}
        //c: prints header and output of my_cos cos and their difference.
        case 'c':
            printf(" x\t\tcos\t\tLibrary\t\tDifference\n");
            printf(" -\t\t---\t\t-------\t\t----------\n");
            for (double x = 0.0; x<2*M_PI; x+=0.05*M_PI) {
                double my = my_cos(x);
                double libr = cos(x);
                double diff = (my>libr) ? my-libr : libr-my;
                printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", x, my, libr, diff);
            }
            if (not_all) {break;}
        //S: prints header and output of my_arcsin asin and their difference.
        case 'S':
            printf(" x\t\tarcsin\t\tLibrary\t\tDifference\n");
            printf(" -\t\t------\t\t-------\t\t----------\n");
            for (double x = -1; x<1; x+=0.05) {
                double my = my_arcsin(x);
                double libr = asin(x);
                double diff = (my>libr) ? my-libr : libr-my;
                printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", x, my, libr, diff);
            }
            if (not_all) {break;}
        //C: prints header and output of my_arccos acos and their difference.
        case 'C':
            printf(" x\t\tarccos\t\tLibrary\t\tDifference\n");
            printf(" -\t\t------\t\t-------\t\t----------\n");
            for (double x = -1; x<1; x+=0.05) {
                double my = my_arccos(x);
                double libr = acos(x);
                double diff = (my>libr) ? my-libr : libr-my;
                printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", x, my, libr, diff);
            }
            if (not_all) {break;}
        //T: prints header and output of my_arctan atan and their difference.
        case 'T':
            printf(" x\t\tarctan\t\tLibrary\t\tDifference\n");
            printf(" -\t\t------\t\t-------\t\t----------\n");
            for (double x = 1; x<10; x+=0.05) {
                double my = my_arctan(x);
                double libr = atan(x);
                double diff = (my>libr) ? my-libr : libr-my;
                printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", x, my, libr, diff);
            }
            if (not_all) {break;}
        //l: prints header and output of my_log log and their difference.
        case 'l':
            printf(" x\t\tlog\t\tLibrary\t\tDifference\n");
            printf(" -\t\t---\t\t-------\t\t----------\n");
            for (double x = 1; x<10; x+=0.05) {
                double my = my_log(x);
                double libr = log(x);
                double diff = (my>libr) ? my-libr : libr-my;
                printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n", x, my, libr, diff);
            }
            if (not_all) {break;}
        }
        if (!not_all) {return 0;}
    }
    return 0;
}

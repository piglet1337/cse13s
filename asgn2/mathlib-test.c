#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "mathlib.h"

#define OPTIONS "ascSCTl:"

int main(int argc, char **argv) {
    int opt = 0;
    int not_all = 1;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a':
            not_all = 0;
        case 's':
            printf(" x\tsin\tLibrary\tDifference\n");
            printf(" -\t------\t-------\t----------\n");
            for (double x = 0.05; x<2*M_PI; x+=0.05*M_PI) {
                double my = my_sin(x);
                double libr = sin(x);
                printf(" %7.4lf % 16.91f % 16.9lf % 16.12lf\n", x, my, libr, my-libr);
            }
            if (not_all) {break;}
        case 'c':
            printf(" x\tcos\tLibrary\tDifference\n");
            printf(" -\t------\t-------\t----------\n");
            for (double x = 0.05; x<2*M_PI; x+=0.05*M_PI) {
                double my = my_cos(x);
                double libr = cos(x);
                printf(" %7.4lf % 16.91f % 16.9lf % 16.12lf\n", x, my, libr, my-libr);
            }
            if (not_all) {break;}
        case 'S':
            printf(" x\tarcsin\tLibrary\tDifference\n");
            printf(" -\t------\t-------\t----------\n");
            for (double x = -1; x<1; x+=0.05) {
                double my = my_arcsin(x);
                double libr = asin(x);
                printf(" %7.4lf % 16.91f % 16.9lf % 16.12lf\n", x, my, libr, my-libr);
            }
            if (not_all) {break;}
        case 'C':
            printf(" x\tarccos\tLibrary\tDifference\n");
            printf(" -\t------\t-------\t----------\n");
            for (double x = -1; x<1; x+=0.05) {
                double my = my_arccos(x);
                double libr = acos(x);
                printf(" %7.4lf % 16.91f % 16.9lf % 16.12lf\n", x, my, libr, my-libr);
            }
            if (not_all) {break;}
        case 'T':
            printf(" x\tarctan\tLibrary\tDifference\n");
            printf(" -\t------\t-------\t----------\n");
            for (double x = 1; x<10; x+=0.05) {
                double my = my_arctan(x);
                double libr = atan(x);
                printf(" %7.4lf % 16.91f % 16.9lf % 16.12lf\n", x, my, libr, my-libr);
            }
            if (not_all) {break;}
        case 'l':
            printf(" x\tlog\tLibrary\tDifference\n");
            printf(" -\t------\t-------\t----------\n");
            for (double x = 1; x<10; x+=0.05) {
                double my = my_log(x);
                double libr = log(x);
                printf(" %7.4lf % 16.91f % 16.9lf % 16.12lf\n", x, my, libr, my-libr);
            }
            if (not_all) {break;}
        }
        if (!not_all) {return 0;}
    }
    return 0;
}

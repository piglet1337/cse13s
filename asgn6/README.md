HOW TO RUN PROGRAM  
In command line: make  
Then follow usage  
  
Usage: ./banhammer [options]  
  ./banhammer will read in words from stdin, identify any badspeak or old speak and output an appropriate punishment message. The badspeak and oldspeak (with the newspeak translation) that caused the punishment will be printed after the message. If statistics are enabled punishment messages are suppressed and only statistics will be printed.
    -t <ht_size> : Hash table size set to <ht_size>. (default: 10000)  
    -f <bf_size> : Bloom filter size set to <bf_size>. (default 2^19)  
    -s           : Enables the printing of statistics.  
    -m           : Enables move-to-frount rule.  
    -h           : Display program synopsis and usage.  
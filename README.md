# Compiler Design
This repository contains two projects from my Compiler Design Course, where we are working on building components of a compiler

# Tokenizer
This project simply takes in input text file and converts each line into a series of valid tokens that could be understood by a compiler. If the program cannot convert the line into valid tokens, it returns an error message. To use this program and see how it works, simply compile and run the tokenizer.cpp program with parser_text_hw2.txt as a command line argument.

# LL1
This project codes an LL(1) grammar given by the class textbook (Engineering A Compiler by Keith Cooper and Linda Torczon. ISBN-10: 012088478X), and then creates FIRST, FOLLOW and FIRST+ tables for that gramar, to then create the LL(1) parsing table. The codes for building each table are adapted from psuedocode in the textbook. The grammar productions, FIRST, FOLLOW, FIRST+, and LL(1) parsing table, are printed to the console upon running the program. To use this program and see how it works, simply compile and run LL1.cpp. The program does not take any command line arguments.

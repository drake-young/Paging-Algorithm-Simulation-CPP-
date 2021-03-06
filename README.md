# Paging Algorithm Simulation (C++)

This project is a simulation of four virtual memory paging algorithms.

#### Assignment Description

Write a C or C++ program that simulates the operation of 4 page replacement algorithms used in a virtual memory management system:
1. FIFO (First-in-First-out) Algorithm
2. Least Recently Used (LRU) Algorithm
3. Most Frequently Used (MFU) Algorithm
4. Optimal Algorithm

You will simulate each algorithm on the input data sets and report the performance of each. A description of each of these algorithms appears in Chapter 10 of our text.

#### Assignment Specifications

In this lab assignment, you will be calculating the page fault rate of the each different page replacement algorithms for the given page reference string for one processonly. Your program should accept three arguments, which are the number of page frames of the physical memory, the names of an input file, and an output file. The input file contains the page reference string in the order of page request, and the output file stores the display events and the analysis results. You will run each algorithms four times, the first time with 128 page frames, the second time with 256 page frames, the third time with 512 page frames, and the 4th time with 1024 frames.Sample usage is: 
```
	prompt> memsim frame_size input.txt output.txt
```
where
1. frame_size: number of page frames in the primary memory
2. input.txt: refers to the input file and<br/>
*NOTE:*<br/>
    * input file must contain only integers
    * input file must not contain a value >4096
    * input file must contain no more than 10,000 integers
3. output.txt: the name of the output file in which results will be stored

## Source Files

**you0254.cpp**<br/>
Main program.  This file contains all functions used for the simulation as well as all utility functions to perform the simulation
   
**input.txt**<br/>
Contains 10,000 integers representing page requests in a reference stream, the integers are separated one per line this is the file that was used to produce the sample output

**output.txt**<br/>
Output file generated by the program (name varies based on user input) containing a formatted results table with computed statistics for the paging algorithms simulated

## Circumstances of the Program

The Program Runs Successfully

The program was developed and tested on gnu g++ 4.4.2.

## How to Build and Run the Program

1. Download, Pull, or Clone this repository 
2. Ensure that you have the following files within your local copy:<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;you0254.cpp<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;input.txt<br/>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;makefile<br/><br/>
3. Build the Program.<br/><br/>
Compile the Program By:<br/>
% make<br/><br/>
4. Run the Program by:<br/>
% ./you0254<br/><br/>
5. Delete the obj files, executables, and core dump by<br/>
% ./make clean

## Author

* **Drake Young** - [drake-young](https://github.com/drake-young)

## Sample Output

Below is a sample output when running the program against the input4 file using the FCFS scheduling algorithm.

<img src="https://github.com/drake-young/Paging-Algorithm-Simulation-CPP-/blob/master/Sample%20Output.png?raw=true" alt="sample output image" />

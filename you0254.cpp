// =====================================================================
// PROGRAM:	LAB 3 -- MEMORY MANAGEMENT SIMULATION
// =====================================================================
// 
// Author:			Drake Young
//
// Last Modified:	November 2018
//
// Assignment Description:
//		Write a C or C++ program that simulates the operation of 4 page 
//		replacement algorithms used in a virtual memory management 
//		system:
//			1.	FIFO (First-in-First-out) algorithm
//			2.	Least Recently Used (LRU) algorithm  
//			3.	Most  Frequently Used (MFU) algorithm
//			4.	Optimal algorithm 
//		You will simulate each algorithm on the input data sets and 
//		report the performance of each. A description of each of 
//		these algorithms appears in Chapter 10 of our text.
//
//	Assignment Specifications:
//		In this lab assignment, you will be calculating the page fault 
//		rate of the each different page replacement algorithms for the 
//		given page reference string for one processonly. Your program 
//		should accept three arguments, which are the number of page 
//		frames of the physical memory, the names of an input file, 
//		and an output file. The input file contains the page reference 
//		string in the order of page request, and the output file stores 
//		the display events and the analysis results. You will run each 
//		algorithms four times, the first time with 128 page frames, the 
//		second time with 256 page frames, the third time with 512 page 
//		frames, and the 4th time with 1024 frames.Sample usage is   
//				prompt> memsim frame_size input.dat output.dat        
//							where 
//				1.	frame_size:	no. of page frames in the primary memory
//				2.	input.dat:	refers to the input file and  
//						NOTE: 
//							*	input file must contain only integers
//							*	input file must not contain a value >4096
//							*	input file must contain no more than 10,000 integers
//							*	one integer per line in the input file
//				3.	output.dat:	the name of the output file.  
//
// =====================================================================

// =====================================================================
// INCLUDES
// =====================================================================
//
//		*	iostream:	used for I/O operations, like cout/cin/etc
//		*	string:		string object for ease-of-operations for I/O
//		*	fstream:	used for file operations (reading/writing)
//		*	iomanip:	used for formatting output (width, precision, etc)
//		*	sstream:	stringstream object used to convert floating point
//						results into strings with precision-n
//
// =====================================================================
// NAMESPACES
// =====================================================================
//
//		*	std:	standard namespace encapsulates iostream, string, and
//					many oter operations used
//
// =====================================================================
#include<iostream>
#include<string>
#include<fstream>
#include<iomanip>
#include<sstream>
using namespace std;



// =====================================================================
// FUNCTION: toInt
// =====================================================================
//
// Input:	string str --	string to be converted into an integer (valid
//							int assumed)
// Output:	integer value equivalent to the number represented by the string
//			parameter
// Task:	iterate through the string, converting each character into 
//			its corresponding numeric digit in an integer number to be 
//			returned
//
// =====================================================================
int toInt(string str)
{
	// === LOCAL VARIABLES === //
	int result = 0;

	// === OPERATIONS === //
	for (int i = 0; i < str.length(); ++i)
	{
		result = (result * 10) + (str[i] - '0');
	}

	// === RETURN === //
	return result;
}



// =====================================================================
// FUNCTION: output
// =====================================================================
//
// Input:
//		*	string s:		string to be output
//		*	ofstream out:	outfile stream object for output to stream to
//		*	[int width]:	width allowed for stream to display (optional)
// Output:
//		*	void return
//		*	output to console
//		*	ofstream out modified
// Task: (output both to the passed stream and to the console)
//		-	if the with parameter is provided, set it for the
//			output stream
//		-	cout the s parameter
//		-	stream the s parameter into the out parameter
//
// =====================================================================
void output(string s, ofstream& out, int width=0)
{
	// === SETUP === //
	if (width > 0)
	{
		cout.width(width);
		out.width(width);
	}

	// === OUTPUT === //
	cout << s;
	out << s;
}



// =====================================================================
// FUNCTION: printStat
// =====================================================================
//
// Input:
//		*	float val:			floating point value to be printed
//		*	ofstream& out:		outfile stream object for output to stream to
//		*	[int precision]:	desired precision of display float (optional, default 3)
//		*	[int width]:		width of output field (optional, default 0)
// Output:
//		*	void return
//		*	output to console
//		*	ofstream parameter modified
// Task:
//		-	use stringstream to create a formatted string according to the
//			input parameters
//		-	call the "output" function (above) with the newly created string
//
// =====================================================================
void printStat(float val, ofstream& out, int precision = 3, int width = 0)
{
	// === LOCAL VARIABLES === //
	stringstream stream;

	// === SETUP === //
	stream << fixed << setprecision(precision) << val;

	// === OUTPUT === //
	output(stream.str(), out, width);
}



// =====================================================================
// FUNCTION: populateFromFile
// =====================================================================
//
// Input:
//		*	ifstream in:	input file stream object for the file beign
//							read from
//		*	int data[]:		integer array to hold the contents of the file
//		*	const int len:	integer representing the number of indexes
//							in the array
// Output:
//		*	void return
//		*	data array parameter is modified
// Task:
//		-	iterate through the file, reading integers until the
//			the file has been fully read, or the length of the arry is
//			reached
//		-	store each int from the file in sequential order in the array
//
// =====================================================================
void populateFromFile(ifstream& in, int data[], const int len)
{
	// === LOCAL VARIABLES === //
	int value = 0;
	int index = 0;

	// === READ AND STORE === //
	while (in >> value)
	{
		if (index >= len)
			break;
		data[index] = value;
		++index;
	}
}



// =====================================================================
// FUNCTION: contains
// =====================================================================
// 
// Input:
//		*	int value:		value to be searched for in the array
//		*	int arr[]:		array to be searched
//		*	const int len:	number of elements in the array
// Output:	
//		*	Boolean (true if found, false otherwise)
// Task:
//		-	perform a linear search on the arr parameter for the val 
//			parameter
//		-	if val is found in arr, return true
//		-	if val is not found in arr, return false
//			
//
// =====================================================================
bool contains(int value, int arr[], const int len)
{
	// === SEARCHING LOOP === //
	for (int i = 0; i < len; ++i)
	{
		if (arr[i] == value)
			return true;
	}
	return false;
}



// =====================================================================
// FUNCTION: getMaxValue
// =====================================================================
//
// Input:
//		*	int arr[]:		array to be searched
//		*	const int len:	number of elements in the array
// Output:
//		*	integer representig the largest value in the arr parameter
// Task:
//		-	iterate through the arr parameter, keeping track of largest
//			value known
//		-	after fully iterating, return the largest value found
//
// =====================================================================
int getMaxValue(int arr[], const int len)
{
	// === LOCAL VARIABLES === //
	int max = 0;

	// === SEARCHING LOOP === //
	for (int i = 0; i < len; ++i)
	{
		if (arr[i] > max)
			max = arr[i];
	}

	// === RETURN === //
	return max;
}



// =====================================================================
// FUNCTION: minimumIndex
// =====================================================================
//
// Input:
//		*	int arr[]:		array to be searched
//		*	const int len:	number of elements in the array
// Output:
//		*	integer representing the index of the minimum value in arr
// Task:
//		-	iterate through arr to determine the minimum value, and 
//			the index of that value
//		-	return that value
// NOTE:
//		*	if multiple values are tied for minimum, the one with the 
//			smallest index is returned
//
// =====================================================================
int minimumIndex(int arr[], const int len)
{
	// === LOCAL VARIABLES === //
	int idx = 0;

	// === SEARCHING LOOP === //
	for (int i = 0; i < len; ++i)
	{
		if (arr[i] < arr[idx])
			idx = i;
	}

	// === RETURN === //
	return idx;
}



// =====================================================================
// FUNCTION: getIndex()
// =====================================================================
//
// Input:
//		*	int value:		value being searched for
//		*	int arr[]:		array being searched
//		*	const int len:	number of elements in arr
// Output:
//		*	integer representing the smallest index of arr that contains
//			val, or -1 if val is not in arr
// Task:
//		-	iterate through arr, checking for an index that contains val
//		-	when such an index is found, that index number is returned
//		-	if val is never found, -1 is returned
//
// =====================================================================
int getIndex(int value, int arr[], const int len)
{
	// === SEARCHING LOOP === //
	for (int i = 0; i < len; ++i)
	{
		if (arr[i] == value)
			return i;
	}
	return -1;
}



// =====================================================================
// FUNCTION: getFrequency
// =====================================================================
//
// Input:
//		*	int value:		value being counted
//		*	int arr[]:		array being searched
//		*	const int len:	number of elements in the array
//		*	[int start]:	starting index of the search range, inclusive 
//							(default=0)
//		*	[int end]:		ending index of the search range, non-inclusive
//							(if none specified, range ends at end of array)
// Output:
//		*	integer representing the number of times value appeared in arr, within
//			the specified range
// Task:
//		-	ensure that the start and end parameters are not illegal values
//		-	iterate through the specified range of indexes of arr
//		-	a counter is incremented every time value is found
//		-	the counter is returned once iteration ends
//
// =====================================================================
int getFrequency(int value, int arr[], const int len, int start = 0, int end=-1)
{
	// === LOCAL VARIABLES === //
	int count = 0;

	// === SETUP === //
	if (end == -1)
		end = len;
	if (start < 0 || start > len)
		return -1;
	if (end < 0 || end > len || end < start)
		return -1;

	// === COUNTING LOOP === //
	for (int i = start; i < end; ++i)
	{
		if (arr[i] == value)
			++count;
	}

	// === RETURN === //
	return count;
}



// =====================================================================
// FUNCTION: outputTableHeader
// =====================================================================
//
// Input:
//		*	ofstream& outfile:	file to be output to
//		*	int frameSize:		frameSize to be displayed in the table
//								header
// Output:
//		*	void return
//		*	output to console
//		*	output to file
// Task:
//		-	display the header to the results table with formatted 
//			output as specified by the assignment
//
// =====================================================================
void outputTableHeader(ofstream& outfile, int frameSize)
{
	// === LOCAL VARIABLES === //
	string border = "======================================================================================\n";
	string dashedLine = "--------------------------------------------------------------------------------------\n";

	// === OUTPUT === //
	output(border, outfile);
	output(("Page Replacement Algorithm Simulation (frame size = " + to_string(frameSize) + ")\n"), outfile, border.length());
	output(border, outfile);
	output("Page Fault Rates\n", outfile, border.length()*0.9);
	output("Algorithm    Total Page Faults  ", outfile);
	output("2000", outfile, (border.length() - 30) / 5);
	output("4000", outfile, (border.length() - 30) / 5);
	output("6000", outfile, (border.length() - 30) / 5);
	output("8000", outfile, (border.length() - 30) / 5);
	output("10,000\n", outfile, (border.length() - 30) / 5);
	output(dashedLine, outfile);
}



// =====================================================================
// FUNCTION: doFIFO
// =====================================================================
//
// Input:
//		*	int refStr[]:	list of all requests in the order they arrive
//		*	const int len:	lenth of refStr
//		*	int frameSize:	number of frames allowed for the simulation
//		*	ofstream& out:	file to output results to
// Output:
//		*	void return
//		*	output to console
//		*	output to out parameter
// Task:
//		-	set up the simulation, creating any necessary variables to
//			compute the output statistics or the next frame to be paged
//		-	run the simulation by iterating through the refStr, if the
//			refStr value isn't currently in the frame value, page out the
//			frame that has been around the longest
//		-	whenever the page is replaced, count an additional page fault
//		-	after iterating through the simulation, output the result 
//			calculations
// FIFO Algorithm:
//		1.	whenever there is a request, check if the page is already in
//			memory
//		2.	if the page isn't in memory, a page fault occurred and must be
//			brought in
//		3.	replace the page that has been in memory the longest with the
//			new one
// NOTES:
//		-	FIFO will always page in the same order since memory starts empty, 
//			so there is no need to keep track of arrival time, simply increment 
//			the index to be replaced, and set to 0 if incremented outside of 
//			number of frames
//
// =====================================================================
void doFIFO(int refStr[], const int len, int frameSize, ofstream& out)
{
	// === LOCAL VARIABLES === //
	int* frames = new int[frameSize];
	int faults[] = { 0 , 0 , 0 , 0 , 0 };
	int nextFaultReplaceIndex = 0;
	int totalFault = 0;

	// === SETUP === //
	fill_n(frames, frameSize, -1);

	// === FUNCTIONALITY === //
	output("FIFO", out, 9);
	for (int i = 0; i < len; ++i)
	{
		if (!contains(refStr[i], frames, frameSize))
		{
			frames[nextFaultReplaceIndex] = refStr[i];
			nextFaultReplaceIndex = (nextFaultReplaceIndex + 1) % frameSize;
			faults[(int)(i / (len/5.))] += 1;
			++totalFault;
		}
	}

	// === TABLE OUTPUT === //
	totalFault = faults[0] + faults[1] + faults[2] + faults[3] + faults[4];
	output(to_string(totalFault), out, 21);
	printStat(((float)(faults[0]) / (float)2000), out, 3, 13);
	printStat(((float)(faults[0] + faults[1]) / (float)4000), out, 3, 11);
	printStat(((float)(faults[0] + faults[1] + faults[2]) / (float)6000), out, 3, 11);
	printStat(((float)(faults[0] + faults[1] + faults[2] + faults[3]) / (float)8000), out, 3, 11);
	printStat(((float)(faults[0] + faults[1] + faults[2] + faults[3] + faults[4]) / (float)10000), out, 3, 10);
	output("\n", out);

	// === CLEANUP === //
	delete[] frames;
}



// =====================================================================
// FUNCTION: doLRU
// =====================================================================
//
// Input:
//		*	int refStr[]:	list of all requests in the order they arrive
//		*	const int len:	lenth of refStr
//		*	int frameSize:	number of frames allowed for the simulation
//		*	ofstream& out:	file to output results to
// Output:
//		*	void return
//		*	output to console
//		*	output to out parameter
// Task:
//		-	set up the simulation, creating any necessary variables to
//			compute the output statistics or the next frame to be paged
//		-	run the simulation by iterating through the refStr, if the
//			refStr value isn't currently in the frame value, page out the
//			frame that has been unused the longest
//		-	whenever the page is replaced, count an additional page fault
//		-	after iterating through the simulation, output the result 
//			calculations
// LRU Algorithm:
//		1.	whenever there is a request, check if the page is already in
//			memory
//		2.	if the page isn't in memory, a page fault occurred and must be
//			brought in
//		3.	replace the page that has been unused the longest with the
//			new one (smallest "last use" time)
//
// =====================================================================
void doLRU(int refStr[], const int len, int frameSize, ofstream& out)
{
	// === LOCAL VARIABLES === //
	int* frames = new int[frameSize];
	int* useTime = new int[frameSize];
	int faults[] = { 0 , 0 , 0 , 0 , 0 };
	int nextFaultReplaceIndex = 0;
	int totalFault = 0;

	// === SETUP === //
	fill_n(frames, frameSize, -1);
	fill_n(useTime, frameSize, -1);

	// === FUNCTIONALITY === //
	output("LRU", out, 9);
	for (int i = 0; i < len; ++i)
	{
		// If the page is not in memory, perform page fault operations
		if (!contains(refStr[i], frames, frameSize))
		{
			nextFaultReplaceIndex = minimumIndex(useTime, frameSize);
			frames[nextFaultReplaceIndex] = refStr[i];
			useTime[nextFaultReplaceIndex] = i;
			faults[(int)(i / (len / 5.))] += 1;
			++totalFault;
		}
		// otherwise, one in memory has a new "last use" time
		else
		{
			useTime[getIndex(refStr[i], frames, frameSize)] = i;
		}
	}

	// === TABLE OUTPUT === //
	output(to_string(totalFault), out, 21);
	printStat(((float)(faults[0]) / (float)2000), out, 3, 13);
	printStat(((float)(faults[0] + faults[1]) / (float)4000), out, 3, 11);
	printStat(((float)(faults[0] + faults[1] + faults[2]) / (float)6000), out, 3, 11);
	printStat(((float)(faults[0] + faults[1] + faults[2] + faults[3]) / (float)8000), out, 3, 11);
	printStat(((float)(faults[0] + faults[1] + faults[2] + faults[3] + faults[4]) / (float)10000), out, 3, 10);
	output("\n", out);

	// === CLEANUP === //
	delete[] frames;
	delete[] useTime;
}



// =====================================================================
// FUNCTION: doMFU
// =====================================================================
//
// Input:
//		*	int refStr[]:	list of all requests in the order they arrive
//		*	const int len:	lenth of refStr
//		*	int frameSize:	number of frames allowed for the simulation
//		*	ofstream& out:	file to output results to
// Output:
//		*	void return
//		*	output to console
//		*	output to out parameter
// Task:
//		-	set up the simulation, creating any necessary variables to
//			compute the output statistics or the next frame to be paged
//		-	run the simulation by iterating through the refStr, if the
//			refStr value isn't currently in the frame value, page out the
//			frame that has been used the most total times throughout the
//			duration of the simulation
//		-	whenever the page is replaced, count an additional page fault
//		-	after iterating through the simulation, output the result 
//			calculations
// MFU Algorithm:
//		1.	whenever there is a request, check if the page is already in
//			memory
//		2.	if the page isn't in memory, a page fault occurred and must be
//			brought in
//		3.	replace the page that has been referenced the most number of 
//			times up to the current instance in time the longest with the 
//			new one (smallest "last use" time)
//		4.	if two or more frames are tied for selection, defer to FIFO
//			selection among the candidates
//
// =====================================================================
void doMFU(int refStr[], const int len, int frameSize, ofstream& out)
{
	// === LOCAL VARIABLES === //
	int freqLen = getMaxValue(refStr, len)+1;
	int* frames = new int[frameSize];
	int* frequency = new int[freqLen];
	int* arrivalTime = new int[frameSize];
	int faults[] = { 0 , 0 , 0 , 0 , 0 };
	int nextFaultReplaceIndex = 0;
	int totalFault = 0;

	// === SETUP === //
	fill_n(frames, frameSize, -1);
	fill_n(frequency, freqLen, 0); // arbitrarily large to ensure replacement first

	// === FUNCTIONALITY === //
	output("MFU", out, 9);
	for (int i = 0; i < len; ++i)
	{
		// If not in memory, do page fault operations
		if (!contains(refStr[i], frames, frameSize))
		{
			// If there is a -1 in the "frames" array, there is a free frame 
			// that can be filled instead of replacing one in memory, pick a
			// free frame first if there are any
			if (contains(-1, frames, frameSize))
			{
				nextFaultReplaceIndex = getIndex(-1, frames, frameSize);
			}
			// If there are no free frames, select based on use frequency 
			// (which we keep track of). If there is a tie, defer selection
			// to FIFO between the tied candidates
			else
			{
				// Get the frame with the highest frequency
				for (int j = 0; j < frameSize; ++j)
				{
					if (frequency[frames[j]] > frequency[frames[nextFaultReplaceIndex]])
					{
						nextFaultReplaceIndex = j;
					}
				}
				// If there are any frames with the same frequency that have been in memory longer,
				// get that instead
				for (int j = 0; j < frameSize; ++j)
				{
					if (frequency[frames[j]] == frequency[frames[nextFaultReplaceIndex]] && arrivalTime[j] < arrivalTime[nextFaultReplaceIndex])
						nextFaultReplaceIndex = j;
				}
			}
			// now that replacement index is known, perform the rest of the 
			// page replacement operations
			frames[nextFaultReplaceIndex] = refStr[i];
			arrivalTime[nextFaultReplaceIndex] = i;
			frequency[refStr[i]] += 1;
			faults[(int)(i / (len / 5.))] += 1;
			++totalFault;
		}
		// If the page is already in memory, increment it's frequency counter
		else
		{
			frequency[refStr[i]] += 1;
		}
	}

	// === TABLE OUTPUT === //
	output(to_string(totalFault), out, 21);
	printStat(((float)(faults[0]) / (float)2000), out, 3, 13);
	printStat(((float)(faults[0] + faults[1]) / (float)4000), out, 3, 11);
	printStat(((float)(faults[0] + faults[1] + faults[2]) / (float)6000), out, 3, 11);
	printStat(((float)(faults[0] + faults[1] + faults[2] + faults[3]) / (float)8000), out, 3, 11);
	printStat(((float)(faults[0] + faults[1] + faults[2] + faults[3] + faults[4]) / (float)10000), out, 3, 10);
	output("\n", out);

	// === CLEANUP === //
	delete[] frames;
	delete[] frequency;
	delete[] arrivalTime;
}



// =====================================================================
// FUNCTION: doOptimal
// =====================================================================
//
// Input:
//		*	int refStr[]:	list of all requests in the order they arrive
//		*	const int len:	lenth of refStr
//		*	int frameSize:	number of frames allowed for the simulation
//		*	ofstream& out:	file to output results to
// Output:
//		*	void return
//		*	output to console
//		*	output to out parameter
// Task:
//		-	set up the simulation, creating any necessary variables to
//			compute the output statistics or the next frame to be paged
//		-	run the simulation by iterating through the refStr, if the
//			refStr value isn't currently in the frame value, page out the
//			frame that will remain unused the longest, and replace that frame
//			with the page being referenced
//		-	whenever the page is replaced, count an additional page fault
//		-	after iterating through the simulation, output the result 
//			calculations
// Optimal Algorithm:
//		1.	whenever there is a request, check if the page is already in
//			memory
//		2.	if the page isn't in memory, a page fault occurred and must be
//			brought in
//		3.	replace the page that will be unused for the longest amount 
//			of time with the new one (greatest "next use" time)
//		4.	if two or more frames are tied for selection, defer to FIFO
//			selection among the candidates (this only happen if 2 or more
//			frames will never be used again)
//
// =====================================================================
void doOptimal(int refStr[], const int len, int frameSize, ofstream& out)
{
	// === LOCAL VARIABLES === //
	int freqLen = getMaxValue(refStr, len) + 1;
	int* frames = new int[frameSize];
	int* nextUse = new int[freqLen];
	int* arrivalTime = new int[frameSize];
	int faults[] = { 0 , 0 , 0 , 0 , 0 };
	int nextFaultReplaceIndex = 0;

	// === SETUP === //
	fill_n(frames, frameSize, -1);
	fill_n(nextUse, freqLen, len+1);

	// === FUNCTIONALITY === //
	output("Optimal", out, 9);
	int totalFault = 0;
	for (int i = 0; i < len; ++i)
	{
		// if the page being referenced is not currently in memory, 
		// then perform the page fault operations
		if (!contains(refStr[i], frames, frameSize))
		{
			// -1 in the frames array represents a free frame.
			// if there is a free frame, that should be filled before
			// any page replacement occurs
			if (contains(-1, frames, frameSize))
			{
				nextFaultReplaceIndex = getIndex(-1, frames, frameSize);
			}
			// otherwise, if there are no free frames, select a frame to
			// replace
			else
			{
				// determine the "next use" for each frame currently in memory
				// (if next use is > "len" that means it is never used again
				fill_n(nextUse, freqLen, len + 1);
				for (int j = i+1; j < len; ++j)
				{
					if (nextUse[refStr[j]] > len)
						nextUse[refStr[j]] = j;
				}

				// determine which frame in memory has the greatest "next use"
				// (this frame will be paged-out)
				for (int j = 0; j < frameSize; ++j)
				{
					if (nextUse[frames[j]] > nextUse[frames[nextFaultReplaceIndex]])
					{
						nextFaultReplaceIndex = j;
					}
				}

				// ensure that fifo is deferred to if there are more than one with the same 
				// (this should only happen if more than one frame are never used again)
				for (int j = 0; j < frameSize; ++j)
				{
					if (nextUse[frames[j]] == nextUse[frames[nextFaultReplaceIndex]] && arrivalTime[j] < arrivalTime[nextFaultReplaceIndex])
						nextFaultReplaceIndex = j;
				}
			}
			// now that the frame to be replace has been selected, perform the
			// page fault operations
			frames[nextFaultReplaceIndex] = refStr[i];
			arrivalTime[nextFaultReplaceIndex] = i;
			faults[(int)(i / (len / 5.))] += 1;
			++totalFault;
		}
	}

	// === TABLE OUTPUT === //
	output(to_string(totalFault), out, 21);
	printStat(((float)(faults[0]) / (float)2000), out, 3, 13);
	printStat(((float)(faults[0] + faults[1]) / (float)4000), out, 3, 11);
	printStat(((float)(faults[0] + faults[1] + faults[2]) / (float)6000), out, 3, 11);
	printStat(((float)(faults[0] + faults[1] + faults[2] + faults[3]) / (float)8000), out, 3, 11);
	printStat(((float)(faults[0] + faults[1] + faults[2] + faults[3] + faults[4]) / (float)10000), out, 3, 10);
	output("\n", out);

	// === CLEANUP === //
	delete[] frames;
	delete[] nextUse;
	delete[] arrivalTime;
}



// =====================================================================
// FUNCTION: main
// =====================================================================
//
// Input:
//		*	N/A
// Output:
//		*	void return
//		*	output to console
//		*	output to file
// Task:
//		-	prompt the user for frame size, input file, and output file
//		-	retrieve these inputs in the order above
//		-	call each of the algorithms in the following order:
//				fifo, lru, mfu, optimal
// Note:
//		-	the program expects inputs to be formatted appropriately, 
//			and performance cannot be guaranteed in the case of bad input
//				*	frame size must be an integer
//				*	input file must exist
//				*	input file cannot have spaces in its name
//				*	all three inputs are required, and output cannot be
//					guaranteed if there are more than 3 inputs
//
// =====================================================================
int main()
{
	// === VARIABLES === //
	const int MAX_REFERENCES = 10000;
	int frameSize = 0;
	string frameSizeStr = "";
	string inFileName = "";
	string outFileName = "";
	ifstream infile;
	ofstream outfile;
	int refStream[MAX_REFERENCES];

	// === SETUP === //
	fill_n(refStream, MAX_REFERENCES, -1);

	// === PROMPT=== //
	cout << "Please Input in the Following Format: frame_size input_file output_file (no spaces in file names)" << endl;
	cout << "prompt>memsim ";
	cin >> frameSizeStr;
	cin >> inFileName;
	cin >> outFileName;
	frameSize = toInt(frameSizeStr);
	infile.open(inFileName);
	outfile.open(outFileName);

	// === READ FILE === //
	populateFromFile(infile, refStream, MAX_REFERENCES);
	int len = (contains(-1, refStream, MAX_REFERENCES)) ? minimumIndex(refStream, MAX_REFERENCES) : MAX_REFERENCES;

	// === OPERATION === //
	outputTableHeader(outfile, frameSize);
	doFIFO(refStream, len, frameSize, outfile);
	doLRU(refStream, len, frameSize, outfile);
	doMFU(refStream, len, frameSize, outfile);
	doOptimal(refStream, len, frameSize, outfile);

	// === CLEANUP === //
	infile.close();
	outfile.close();
	cout << endl << endl;
	system("PAUSE");
}

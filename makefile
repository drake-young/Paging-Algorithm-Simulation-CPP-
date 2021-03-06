#Place this makefile in the same directory as your
#all .cpp and .h files
#at the command prompt
#make
#./you0254

#for use with c++ files
.SUFFIXES : .cpp

#Compile with g++
CC = g++

#use -g for gnu debugger and -std= for c++11 compiling
CXXFLAGS = -g -std=c++11

######Change to match all .cpp files.  Do not include .h files####
OBJS = you0254.o

######Change from matrix to you0254.#######
TARGET = you0254

######Do NOT change this...Tabs Matter!#########
$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

.cpp.o:
	$(CC) -c $(CXXFLAGS) $(INCDIR) $<

#In order to get rid of all .o files create, at the command prompt
#make clean

clean:
	rm -f $(OBJS) $(TARGET) core



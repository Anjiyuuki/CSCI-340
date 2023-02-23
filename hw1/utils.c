// -----------------------------------
// CSCI 340 - Operating Systems I 
// Spring 2023 (Anthony Leclerc)
// utils.c implementation file
// Homework Assignment 1
// Angie Bui
// -----------------------------------

// these are the ONLY library header files that can be used. Under no
// circumstances can additional library header files be included.  In
// particular, this restriction means you CANNOT USE any function
// declared in <string.h>, which includes functions such as: strtok(),
// strchr(), strstr(), strlen(), index(), and others.

// NOTE: when you compile, DO NOT IGNORE WARNINGS!  These warnings are
// likely indicating you are doing something wrong, such as using a
// library function that you are not allowed to use or passing the
// wrong type of argument into a function.

#include <stdio.h>	// standard I/O functions
#include <stdlib.h> 	// atof()
#include <math.h>	// math functions

#include "utils.h"	// user defined functions

// see utils.h for the required function prototypes and specifications

// NOTE: you may add any "helper" functions you need to get the job done.
//       for example, if you need to determine the length of a string.

// put your IMPLEMENTATION of the function prototypes below:


int read_lines(char* filename, line_t* line_arr, int n) {

  FILE* fp = fopen(filename, "r");
		if (fp == NULL) {
			printf("no file found");
			return -1; }


	char arrayChar[80];



	int numLines = 0; //used as a counter, loop until reaches n or no more lines 
	while (numLines < n && fgets(arrayChar, 80, fp)) { //fgets reads 79 characters from the file pointed by fp and stores them in arrayChar
	char* x0 = arrayChar; // point to the start of the line in arrayChar
	
	// pointers to store the x and y coordinates, null for now 
	char* y0 = NULL;
	char* x1 = NULL;
	char* y1 = NULL;

    int location;
		for (location = 0; arrayChar[location]; location++) {
			if (arrayChar[location] == ',') { // if current character hit comma 
				arrayChar[location] = 0; // replace it will 0 to mark it 
			if (y0 == NULL) { 
				y0 = arrayChar + location + 1; // Set y0 to the first character following the first comma
			} else if (x1 == NULL) {
				x1 = arrayChar + location + 1; 
			} else if (y1 == NULL) {
				y1 = arrayChar + location + 1; 
			break; // when all comma found
    }
  }
}

	// convert string number stored using atof and put into 
	line_arr[numLines].p0.x = atof(x0); // x-coord of the first point of the the line in the array.
	line_arr[numLines].p0.y = atof(y0);
	line_arr[numLines].p1.x = atof(x1);
	line_arr[numLines].p1.y = atof(y1);
	
	numLines++; 
	
  }

  fclose(fp);
  return numLines;
}

	double calc_direction(line_t* line_ptr) {
		double x_difference = line_ptr->p1.x - line_ptr->p0.x;
		double y_difference = line_ptr->p1.y - line_ptr->p0.y;
	return atan2(y_difference, x_difference);
	}

	double calc_magnitude(line_t* line_ptr) {
	double x_difference = line_ptr->p1.x - line_ptr->p0.x;
	double y_difference = line_ptr->p1.y - line_ptr->p0.y;
	return sqrt(x_difference * x_difference + y_difference * y_difference);
	}

 
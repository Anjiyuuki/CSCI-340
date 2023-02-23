// these are the ONLY library header files that can be used. UNDER NO
// CIRCUMSTANCES can additional library headers be included!

#include <stdio.h>	// I/O functions
#include <stdlib.h> 	// atof() function
#include <math.h>	// math functions (eg. sin(), cos(), sqrt())

#include "utils.h"	// user defined functions, constants, and data types

// ----------------------------------------------
// define constants
// define constants
#define PI 3.1415926535  // constant Pi
#define TOTAL_LINES 10   // constant that defines the total number
                         // of line_t values that can be handled
// -----------------------------------
// Main function 
// parameters:	argc --- number of arguments suppled by user on command-line
//		argv --- array of command-line argument values

int main(int argc, char** argv)
{
  
    //steps to be performed
        //1. declare needed variables
		int num_lines = TOTAL_LINES, amt_lines, i;
		line_t* lines;
		

	//2. allocate DYNAMIC MEMORY for your line array of size
	   //TOTAL_LINES. NOTE: you MUST use dynamic memory.  points will
	   //be taken off if you do not.
	lines = (line_t*) malloc(TOTAL_LINES * sizeof(line_t));
		if (lines == NULL) {
		printf("Error\n");
		return 1; }
	//3. check argc and make sure the program is being called with
	  // one (extra) command-line parameter.  For example, from the
	   //command-line, the user would enter:
	   //$ ./hw1 lines.txt
	    if (argc < 2) {
    printf("Usage: %s <filename>\n", argv[0]);
    return 1;
  }

	//4. using the read_lines() function described in utils.h, read
  	   //lines from the text file specified in argv[1].  Again, the
  	   //location of the text file (eg lines.txt) MUST BE specified
  	   //in argv[1].
	    amt_lines = read_lines(argv[1], lines, TOTAL_LINES);
  if (amt_lines == -1) {
    printf("Error reading lines\n");
    return 1;
  }

    	//4. Loop through the array of line_t structs and for each line:

	  //- compute the magnitude using calc_magnitude()
	  //- compute the direction angle in radians using calc_direction()
	  //- convert the direction angle to degrees, place in new variable
	  //- print the following valus on one line, separated by a comma:
	   // * line magnitude (using only 2 decimal places)
  	    //* line direction in radians (using only 2 decimal places)
  	    //* line direction in degrees (using only 2 decimal places)
	    //* (x, y) value of P0 endpoint (using only 2 decimal places)
  	    //* (x, y) value of P1 endpoint (using only 2 decimal places)
		for ( i = 0; i < num_lines; i++) {
			float mag = calc_magnitude(lines [i]);
			float dir_rad = calc_direction(lines [i]);
			float dir_deg = dir_rad * 180 / M_PI;
			printf("%.2f,%.2f,%.2f,(%.2f,%.2f),(%.2f,%.2f)\n", mag, dir_rad, dir_deg,
			lines[i].p0.x, lines[i].p0.y, lines[i].p1.x, lines[i].p1.y);
  }
  
  	    //FOR EXAMPLE, for the following two lines of input:
	    //0,0,1,1
	    //-1, 1, 1, -1

	    //THE following two lines would be the corresponding output:
	    //1.41,0.79,45.00,(0.00,0.00),(1.00,1.00)
	    //2.83,-0.79,-45.00,(-1.00,1.00),(1.00,-1.00)
	free(lines); 
  
  return 0;   // main must return an int --- zero means "no errors"
}

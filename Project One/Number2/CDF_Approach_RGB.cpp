// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <typeinfo>
#include <math.h>
#include <fstream>

using namespace std;


// This function computes the frequency distribution of a 2D array.
// params 2D array, number of rows, and number of columns
// Returns the pointer to the array of the distribution
// PRE -- 0 <= int(arr2D[i][j]) <= 255
//     -- 0 <= i <= Size1
//     -- 0 <= i <= Size
int * freq_dist(unsigned char ** arr2D, int Size1, int Size){
	// Create output array
	int *dist = new int[256];

        for (int i = 0; i < 256 ; i++){
	   dist[i] = 0;
	}
	
	// Count the values in the array
	int value = 0;
	for(int i = 0; i< Size1; i++){
   	   for(int j = 0; j < Size; j++){
       	      value = int(arr2D[i][j]);
	      dist[value] = dist[value] + 1;
           }
        }
	return dist;
}


// This function covert the frequency distribution to cummulative frequency.
// @params frequency array
// @returns the cummulative frequency array
// PRE --For the array index i (0 <= i <= 255)
int * cumm_freq (int* dist){

	for (int i = 1; i < 256; i++){
		dist[i] = dist[i] + dist[i-1];
	}
	return dist;
}

// This function finds the maximum and minimum values in a 2D array.
// params 2D array, number of rows, and number of columns
// Returns the pointer to the array containing the maximum and minimum values
// PRE -- 0 <= int(ptr[i][j]) <= 255
//     -- 0 <= i <= Size1
//     -- 0 <= i <= Size
int *  min_max(unsigned char **ptr, int rows, int cols){
 	// Create the output array
	int *m = new int[2];
	m[0] = 255;
	m[1] = 0;
	int value;

	// Search for max value
	for (int i = 0; i < rows; i++){
	   for (int j = 0; j < cols; j++){
	      value = int(ptr[i][j]);
	      if (value < m[0]){
	         m[0]= value; 
	      }
	      if (value > m[1]){
		 m[1]= value; 
	      }
  	   }
	}
	return m;
}


// This function finds the transfer function and converts the image with it.
// @params 2D array pointer, number of rows, and number of columns
// @Returns 2D array pointer	     
// PRE -- 0 <= arr[i][j] <= 255
//     -- 0 <= i <= Size1
//     -- 0 <= i <= Size
unsigned char** trans_Fxn(unsigned char **ptr, int Size1, int Size){

	//Compute the transfer function
	unsigned char **arr2D = new unsigned char*[Size1];
	int * cummFreq = cumm_freq(freq_dist(ptr, Size1, Size));

	int* m = min_max(ptr, Size1, Size);
	//int r = m[1] - m[0];
	int x;
	double y; 
	for (int i = 0; i < Size1; i++){
	   arr2D[i] = new unsigned char[Size];
	   for (int j = 0; j < Size; j++){
	       x = int(ptr[i][j]);
	       y = (255.0 * (cummFreq[x] -cummFreq[m[0]]) ) / ((cummFreq[255]- cummFreq[m[0]])*1.0);
	       arr2D[i][j] = char(int(round(y)));
	   }
	}
	
	// Write the transfer function into an array
	ofstream unequalized;
	unequalized.open("Transfer_Function.csv");
	
	int val;
	for (int i = 0; i < 256; i++){
	   
	   val = int(round((255.0 * (cummFreq[i] -cummFreq[m[0]]) ) / ((cummFreq[255]- cummFreq[m[0]])*1.0) ));
	   if (val < 0){
		val = 0;
	   }
	   if (val > 255){
		val = 255;
	   }
	   unequalized << val << endl;
	}
	unequalized.close();

	return arr2D;	
}

// This function finds the frequency distribution and saves it into a file
// @params file name, 2D array, number of rows, and number of columns
// PRE -- 0 <= arr[i][j] <= 255
//     -- 0 <= i <= Size1
//     -- 0 <= i <= Size
void save_dist (string name, unsigned char ** arr2D, int Size1, int Size){

	// Define the name of outut file
	int len = string(name).length();

	char *ptr = new char [len];
	for(int i = 0; i < len; i++){
	    ptr[i] = name[i];
	}

	int *pt = freq_dist(arr2D, Size1, Size);
 
	ofstream unequalized;
	unequalized.open(ptr);
	for (int i = 0; i < 256; i++){
	    unequalized << pt[i] << endl;
	}
	unequalized.close();
}


// MAIN
int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int Size = 256;
	int Size1 = 256;
	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 4){
		BytesPerPixel = 1; // default is grey image
	} 
	else {
		BytesPerPixel = atoi(argv[3]);
		// Check if size is specified
		if (argc >= 5){
			Size = atoi(argv[4]);
		   if (argc == 6){
			Size1 = atoi(argv[5]);
		   }
		}
	}

	// Allocate image data array
	unsigned char Imagedata[Size1][Size][BytesPerPixel];
	
	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size1*Size*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	
	// Initialize the pointers to the image arrays
	unsigned char **arr2D_red = new unsigned char*[Size1];
	unsigned char **arr2D_green = new unsigned char*[Size1];
	unsigned char **arr2D_blue = new unsigned char*[Size1];

	for(int i = 0; i< Size1; i++){
   	   arr2D_red[i] = new unsigned char[Size];
	   arr2D_green[i] = new unsigned char[Size];
	   arr2D_blue[i] = new unsigned char[Size];
   	   for(int j = 0; j < Size; j++){
       	      arr2D_red[i][j] = Imagedata[i][j][0];
	      arr2D_green[i][j] = Imagedata[i][j][1];
	      arr2D_blue[i][j] = Imagedata[i][j][2];
           }
        }

	
	// Perform histogram equalization on the channels
	arr2D_red = trans_Fxn(arr2D_red, Size1, Size);
	arr2D_green = trans_Fxn(arr2D_green, Size1, Size);
	arr2D_blue = trans_Fxn(arr2D_blue, Size1, Size);


	// Write the new color channels into a new 3D-image
	for(int i = 0; i< Size1; i++){
   	   for(int j = 0; j < Size; j++){
       	      Imagedata[i][j][0] = arr2D_red[i][j]; 
	      Imagedata[i][j][1] = arr2D_green[i][j];
	      Imagedata[i][j][2] = arr2D_blue[i][j];
           }
        }
	

	// Write image data (filename specified by second argument) from image data matrix
	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), Size1*Size*BytesPerPixel, file);
	fclose(file);

	return 0;
}

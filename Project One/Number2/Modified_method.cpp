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
// PRE -- 0 <= arr[i][j] <= 255
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


// This function finds the maximum and minimum values in a 2D array.
// params 2D array, number of rows, and number of columns
// Returns the pointer to the array containing the maximum and minimum values
// PRE -- 0 <= arr[i][j] <= 255
//     -- 0 <= i <= Size1
//     -- 0 <= i <= Size
int *  min_max(unsigned char **ptr, int rows, int cols){
 	// Create the output array
	int *m = new int[2];
	m[0] = 255;
	m[1] = 0;
	int value;

	// Search for max valuue
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
	//int* m = min_max(ptr, Size1, Size);
	//int r = m[1] - m[0];
	int x,y;
	float step; 
	for (int i = 0; i < Size1; i++){
	   arr2D[i] = new unsigned char[Size];
	   for (int j = 0; j < Size; j++){
	       x = int(ptr[i][j]);
	       if (x <= 43){
		   step = 127.0/ 43.0 ;
	       	   y = round(x* step);
	       }
	       else{
		   step = 128.0/ 43.0;
		   y = round(130 + ((x-218)*step));
	       } 
	       arr2D[i][j] = (char)y;
	   }
	}
	
	return arr2D;	
}

// This function finds the frequency distribution and saves it into a file
// @params 2D array, number of rows, and number of columns
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
	unsigned char Imagedata[Size1][Size];
	
	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size1*Size, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	
	// Initialize the pointers to the image array
	unsigned char **arr2D = new unsigned char*[Size1];
	for(int i = 0; i< Size1; i++){
   	   arr2D[i] = new unsigned char[Size];
   	   for(int j = 0; j < Size; j++){
       	      arr2D[i][j] = Imagedata[i][j];
           }
        }	
	
	// Save the frequency distribution of the  image before histogram equalization
	save_dist ("Image_Before_Equalization.csv", arr2D, Size1, Size);

	// Perform Histogram Equalization
	arr2D = trans_Fxn(arr2D, Size1, Size);

	for(int i = 0; i< Size1; i++){
   	   for(int j = 0; j < Size; j++){
       	      Imagedata[i][j] = arr2D[i][j]; 
           }
        }

	// Save the frequency distribution of the image after histogram equalization
	save_dist ("Image_After_Equalization.csv", arr2D, Size1, Size);

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), Size1*Size, file);
	fclose(file);

	return 0;
}

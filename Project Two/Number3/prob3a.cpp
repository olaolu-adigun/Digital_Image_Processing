// EE569  Homework Assignment #2
// Date:     Feb 7, 2017
// Name:     Adigun Olaoluwa
// ID:       2543-8895-69
// Email:    adigun@usc.edu


#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>
#include "helper_fxn.h"

using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int Size = 256;
	int Size1 = 256;

	int Size_b = 256;
	int Size_b1 = 256; 
	
	// Check for proper syntax
	if (argc < 6){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "The input is:" << endl;
		cout << "top_image.raw output.raw BytesPerPixel top_width top_height base_image.raw" << endl;
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
		
	       Size = atoi(argv[4]);
	       Size1 = atoi(argv[5]);
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
	unsigned char*** image_arr = new unsigned char**[Size1];

	// Initialize the pointers to image array
	for (int i = 0; i < Size1; i++){
	    image_arr[i] = new unsigned char*[Size];
	    for (int j = 0; j < Size; j++){
		image_arr[i][j] = new unsigned char[BytesPerPixel];		
		for (int k = 0; k < BytesPerPixel; k++){
		    // Assign the pointers to the pixel values
		    image_arr[i][j][k] = Imagedata[i][j][k];		
		}
	    } 
	}
	
	// Create Image object	
	Image image(image_arr, Size1, Size, BytesPerPixel);

	// Perform shrinking
	unsigned char*** image_out = image.Shrinking();

	// Transfer the output pixel value to the image array
	for (int i = 0; i < Size1; i++){
	    for (int j = 0; j < Size; j++){
	        Imagedata[i][j][0] =  image_out[i][j][0];
	    }
	}

	///// Write image data (filename specified by second argument) from image data matrix
	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), Size1*Size*BytesPerPixel, file);
	fclose(file);
	return 0;
}



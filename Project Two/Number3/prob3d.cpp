// EE569   Homework Assignment #2
// Date:   Feb 7, 2017
// Name:   Olaoluwa Adigun
// ID:     2543-8895-69
// Email:  adigun@usc.edu


#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>
#include "helper_fxn2.h"

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
	
	unsigned char Imagedata1[Size1][Size][BytesPerPixel];
	unsigned char*** image_arr = new unsigned char**[Size1];
	
	// Initialize the pointers for the image array
	for (int i = 0; i < Size1; i++){
	    image_arr[i] = new unsigned char*[Size];
	    for (int j = 0; j < Size; j++){
		image_arr[i][j] = new unsigned char[BytesPerPixel];		
		for (int k = 0; k < BytesPerPixel; k++){
		    image_arr[i][j][k] = Imagedata[i][j][k];		
		}
	    } 
	}
	// Find the complement of the image
	for (int i = 0; i < Size1; i++){
	    for (int j = 0; j < Size; j++){
			// Take the complement of the image pixel
	        if(int(Imagedata[i][j][0]) > 100){
		     image_arr[i][j][0] = char(0);
		}
		else{
		     image_arr[i][j][0] = char(255);
		}
	    }
	}
	
	// Write the image complement to an output image
	for (int i = 0; i < Size1; i++){
	    for (int j = 0; j < Size; j++){
		Imagedata1[i][j][0] = image_arr[i][j][0];       
	    }
	}
	file = fopen("Image_Complement.raw","wb");
	fwrite(Imagedata1, sizeof(unsigned char), Size1*Size*BytesPerPixel, file);
	fclose(file);
	
	// Shrink the black holes to white spots 
	cout << "*****************STARTING****************" << endl;	
	cout << "NUMBER OF BLACK HOLES";
	Image image(image_arr, Size1, Size, BytesPerPixel);
	unsigned char*** image_out = image.Shrinking();
	
	// Copy the original image
	for (int i = 0; i < Size1; i++){
	    for (int j = 0; j < Size; j++){
		 image_arr[i][j][0] = Imagedata[i][j][0];		
	    } 
	}

	// Write the black hole spots to an output image
	for (int i = 0; i < Size1; i++){
	    for (int j = 0; j < Size; j++){
		Imagedata1[i][j][0] = image_out[i][j][0];       
	    }
	}
	file = fopen("Black_Hole_Spots.raw","wb");
	fwrite(Imagedata1, sizeof(unsigned char), Size1*Size*BytesPerPixel, file);
	fclose(file);
	
	// Remove the black holes
	cout << "NUMBER OF WHITE SHAPES";
	image_out = image.Remove_holes(image_arr);

	for (int i = 0; i < Size1; i++){
	    for (int j = 0; j < Size; j++){
		 image_arr[i][j][0] = image_out[i][j][0];		
	    } 
	}
	
	// Write the image without holes to an output image
	for (int i = 0; i < Size1; i++){
	    for (int j = 0; j < Size; j++){
		Imagedata1[i][j][0] = image_out[i][j][0];       
	    }
	}
	file = fopen("Without_Holes.raw","wb");
	fwrite(Imagedata1, sizeof(unsigned char), Size1*Size*BytesPerPixel, file);
	fclose(file);

	// Shrink all white objects to a spot	
	image_out = image.Shrinking();
	
	// Write the white spots to an image
	for (int i = 0; i < Size1; i++){
	    for (int j = 0; j < Size; j++){
		Imagedata[i][j][0] = image_out[i][j][0];       
	    }
	}
	
	// Test the shapes on the image array
	image_out = image.Shape_Test(image_arr);

	///// Write image data (filename specified by second argument) from image data matrix
	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), Size1*Size*BytesPerPixel, file);
	fclose(file);
	return 0;
}



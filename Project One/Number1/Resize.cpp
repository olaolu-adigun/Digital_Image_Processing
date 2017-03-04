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

using namespace std;

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
		   if (argc >= 6){
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

	// Create an array for the resized image
	
	int size = atoi(argv[6]);
	int size1 = atoi(argv[7]);
	unsigned char  Imagedata1[size][size][BytesPerPixel];

	// Creeate the step size for resized image
	float delX = (Size -1)/(size*1.0);
	float delY = (Size1-1)/(size1*1.0);

	float F;
	int p, q;
	

	// Use Bilinear Interpolation to find pixel values
	for(int i = 0; i < size1; i++){
           for(int j = 0; j < size; j++){
	       for(int k = 0; k < BytesPerPixel; k++){ 

		   p = int(delX * i);
		   q = int(delY * j);

		   F = ((1-delX)*(1-delY)*(int)Imagedata[p][q][k]);
		   F = F + ((delX)*(1-delY)*(int)Imagedata[p][q+1][k]);
	   	   F = F + ((1-delX)*(delY)*(int)Imagedata[p+1][q][k]) + (delX*delY*(int)Imagedata[p+1][q+1][k]);
                   
		   Imagedata1[i][j][k] = (char)((int)F);
               }
            }
        }

	// Write output image data (filename specified by second argument) from image data matrix
	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata1, sizeof(unsigned char), size1*size*BytesPerPixel, file);
	fclose(file);

	return 0;
}

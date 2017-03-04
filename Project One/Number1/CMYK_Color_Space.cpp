// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <typeinfo>
#include <string>

using namespace std;

char* file_name(char *name1, string name2 ){

     int len1 = string(name1).length();
     int len2 = string(name2).length();

     char *ptr = new char [len1+len2];
     for(int i = 0; i < len1 + len2 ; i++){
	if (i < len2){
	   ptr[i] = name2[i];
	}
	else {
	   ptr[i] = name1[i-len2];
	}
     }
     return ptr; 
}


int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int BytesPerPixel;
	int Size = 256;
	int Size1 = 256;
	
	// Check for proper syntax
	if (argc < 2){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 3){
		BytesPerPixel = 1; // default is grey image
	} 
	else {
		BytesPerPixel = atoi(argv[2]);
		// Check if size is specified
		if (argc >= 4){
			Size = atoi(argv[3]);
		   if (argc == 5){
			Size1 = atoi(argv[4]);
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
	unsigned char Imagedata1[Size1][Size];
	
	// Define the prefix of output files.
	string filePrefix[3];
	filePrefix[0] = "Cyan_";
	filePrefix[1] = "Magenta_";
	filePrefix[2] = "Yellow_";

	unsigned char Im_dat[Size1][Size];

	FILE *file1;

	for(int k = 0; k < BytesPerPixel; k++){ 
	   for(int i = 0; i < Size1; i++){
   	      for(int j = 0; j < Size; j++){

		  // Transform the color space
                  Imagedata1[i][j] = (char)(255 -(int)Imagedata[i][j][k]);	
	       }
            }
	    // Save the new color channels
	    file1 = fopen(file_name(argv[1], filePrefix[k]), "wb");
	    fwrite(Imagedata1, sizeof(unsigned char), Size1*Size, file1);
	    fclose(file1);
	 }	

	// Write image data (filename specified by second argument) from image data matri

	return 0;
}



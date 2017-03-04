// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//		RAW format. You will need to make corresponding changes to
//		accommodate images of different sizes and/or types

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
	FILE *file1;
	int BytesPerPixel;

	int Size = 256;
	int Size1 = 256;

	int insert_image_height = 256;
	int insert_image_width = 256;
	
	int insert_x = 0;
	int insert_y = 0;
	
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
			Size1 = atoi(argv[5]);
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

	///////////////////////Load the top image///////////////////////////////////////////////
	
	// load the insert loacation, and image size
	if (argc < 11){
	  cout << "INPUT ERROR !!!" << endl;
	  return 0;
	} 
	else {
		insert_image_height = atoi(argv[7]);
		insert_image_width = atoi(argv[8]);
	
		insert_x = atoi(argv[9]);
		insert_y = atoi(argv[10]);
	}

	// Read the insert image 
	unsigned char Insertdata[insert_image_width][insert_image_height][3];
	if (!(file1 = fopen(argv[6],"rb"))) {
		cout << "Cannot open file: " << argv[6] <<endl;
		exit(1);
	}
	fread(Insertdata, sizeof(unsigned char), insert_image_width*insert_image_height*BytesPerPixel, file1);
	fclose(file1);

	////////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	
	// Insert the top image into the base image 
	for(int i = 0; i< insert_image_width; i++){
   	   for(int j = 0; j < insert_image_height; j++){
	       // Test for insert image pixels and copy to the base image
	       if(((int)Insertdata[i][j][2]-(int)Insertdata[i][j][1]) < 50 || ((int)Insertdata[i][j][2]-(int)Insertdata[i][j][0]) < 50){
		    for(int k = 0; k < BytesPerPixel; k++){
			Imagedata[i + insert_x][j + insert_y][k] = Insertdata[i][j][k];
		    }
	        }
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

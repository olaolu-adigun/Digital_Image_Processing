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
#include <math.h>

using namespace std;

// This function converts RGB to grayscale
// params R, G, and B values
// Returns the grayscale value
//// PRE -- 0 <= r <= 255
//     -- 0 <= g <= 255
//     -- 0 <= b <= 255 
int Sephia_Color_to_Gray(int r, int g, int b){
 
  	float y = (0.21*r) + (0.72*g) + (0.07*b);
	if (y > 255.0){
	    return 255;
	}
	return round(y);
}


// This function converts Sephia color to RGB
// params grayscale value y
// Returns S_R, S_G, and S_B values
//// PRE -- 0 <= y <= 255   
int *  Sephia_Gray_to_Color(int y){
 
	int *rgb = new int[3];

	rgb[0] = int((0.393 + 0.769 + 0.189)*y);
	rgb[1] = int((0.349 + 0.686 + 0.168)*y);
	rgb[2] = int((0.272 + 0.534 + 0.131)*y);
	for (int i = 0; i < 3 ; i++){
	    if (rgb[i] > 255){
	        rgb[i] = 255;
	    }
	} 
	return rgb;
}

// This function converts RGB to Sephia directly
// params R, G, and B values
// Returns the RGB values for Sephia color image. S_R, S_G, and S_B values
// PRE -- 0 <= SR <= 255
//     -- 0 <= SG <= 255
//     -- 0 <= SB <= 255 
int *  Sephia_RGB_to_Color(int r, int g, int b){
 
	int *rgb = new int[3];

	rgb[0] = int((0.393*r) + (0.349*g) + (0.272*b));
	rgb[1] = int((0.769*r) + (0.686*g) + (0.534*b));
	rgb[2] = int((0.189*r) + (0.168*g)+ (0.131*b));

	for (int i = 0; i < 3 ; i++){
	  if (rgb[i] > 255){
	     rgb[i] = 255;
	  }
	}
	return rgb;
}

// CREATE FILE_NAME FOR OUTPUT FILE
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

	unsigned char Imagedata_gray[Size1][Size];
	
	// Define the file name for output files
	string filePrefix[2];
	filePrefix[0] = "Grayscale_";
	filePrefix[1] = "Sephia_";

	FILE *file1;
	int *pt; 
	for(int i = 0; i < Size1; i++){
   	    for(int j = 0; j < Size; j++){
		// Convert the RGB values to Grayscale 
		Imagedata_gray[i][j] = char(Sephia_Color_to_Gray(int(Imagedata[i][j][0]), int(Imagedata[i][j][1]), int(Imagedata[i][j][2])));

		// Convert the grayscale value to Sephia image values.
		pt = Sephia_Gray_to_Color(int(Imagedata_gray[i][j]));
		
		Imagedata[i][j][0] = (char)pt[0]; 
		Imagedata[i][j][1] = (char)pt[1]; 
		Imagedata[i][j][2] = (char)pt[2];
	    }
	}

	// Write Output
	file1 = fopen(file_name(argv[1], filePrefix[0]), "wb");
	fwrite(Imagedata_gray, sizeof(unsigned char), Size1*Size, file1);
	fclose(file1);

	file1 = fopen(file_name(argv[1], filePrefix[1]), "wb");
	fwrite(Imagedata, sizeof(unsigned char), Size1*Size*BytesPerPixel, file1);
	fclose(file1);

	return 0;
}



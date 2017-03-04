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


// This function computes the maximum number.
// params 3 integer values
// Returns the maximum value from the given inputs 
int findMax (int a, int b, int c){
	
	int maxValue = a;
	if (maxValue < b){
	   maxValue = b;
	}
	if (maxValue < c){
	  maxValue = c;
	}
	return maxValue;
}


// This function computes the minimum number.
// params 3 integer values
// Returns the minimum value from the given inputs 
int findMin (int a, int b, int c){
	
	int minValue = a;
	if (minValue > b){
	   minValue = b;
	}
	if (minValue > c){
	  minValue = c;
	}
	return minValue;
}


// This function computes the range.
// params 3 integer values
// Returns the range of the given inputs 
float range (int a, int b, int c)
{
  return (findMax(a,b,c) - findMin(a,b,c));
}



// This function computes the hue value
// params r, g, and b values
// Returns the Hue value 
// PRE -- 0 <= r <= 255
//     -- 0 <= g <= 255
//     -- 0 <= b <= 255
int Hue (int r, int g, int b)
{
	int pt;
	int c = range(r,g,b);
	double u;

	if (c == 0){
	   pt = 0;
	}
	else if (findMax(r,g,b) == r){
	   u = (g-b)/(1.0*c);
	   u = (60*((u - int(u)) + (int(u) % 6)));
	   pt = round(int(u));
	}
	else if (findMax(r,g,b) == g){
	   u = ((b-r)/(c*1.0)) + 2;
	   pt = round(60*u);
	}
	else{
	   u  = ((r-g)/(c*1.0)) + 4;
	   pt = round(60*u);
	}
	pt = (pt+360) % 360;

	return pt;
}


// This function computes the light value
// params r, g, and b values
// Returns the light value 
// PRE -- 0 <= r <= 255
//     -- 0 <= g <= 255
//     -- 0 <= b <= 255
float Lightness(int r, int g, int b){

    return (findMax(r,g,b) + findMin(r,g,b))/(2*255.0);
}


// This function computes the saturation value
// params r, g, and b values
// Returns the saturation value 
// PRE -- 0 <= r <= 255
//     -- 0 <= g <= 255
//     -- 0 <= b <= 255
float Saturation (int r, int g, int b){

	float L = Lightness(r,g,b);
	float c = range(r,g,b)/255.0;
	float S;
	if (L == 0.0){
	   S = 0;
 	}
	else if (L > 0.0 && L < 0.5){
           S = c/(2*L);
	}else {
           S = c/(2*(1.0-L));
	}
	return S;
}

// This function concatenates a char array and string 
// params char array pointer and string 
// Returns the char array point
// PRE -- name1 is a file name prefix
//     -- name2 contains the file format
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
	filePrefix[0] = "Hue_";
	filePrefix[1] = "Saturation_";
	filePrefix[2] = "Lightness_";

	FILE *file1;
	for(int k = 0; k < BytesPerPixel; k++){ 
	   for(int i = 0; i < Size1; i++){
   	      for(int j = 0; j < Size; j++){
		   // Transform the color space to HSL
		  if (k == 0){
		     // Determine the hue value
                     Imagedata1[i][j] = char(int(Hue(Imagedata[i][j][0], Imagedata[i][j][1], Imagedata[i][j][2])*(255.0/359.0)));
		  }
		  else if (k == 1){
		     // Determine the saturation value
                     Imagedata1[i][j] = char(int(Saturation(Imagedata[i][j][0], Imagedata[i][j][1], Imagedata[i][j][2])*(255.0)));
		  }
	          else{
		     // Determine the light value
		     Imagedata1[i][j] = char(int(Lightness(Imagedata[i][j][0], Imagedata[i][j][1], Imagedata[i][j][2])*(255.0)));
		  }
	      }
	   }
	   // Save the new Color Channels
	   file1 = fopen(file_name(argv[1], filePrefix[k]), "wb");
	   fwrite(Imagedata1, sizeof(unsigned char), Size1*Size, file1);
	   fclose(file1);
        }
	  
	////// Write image data (filename specified by second argument) from image data matrix

	return 0;
}



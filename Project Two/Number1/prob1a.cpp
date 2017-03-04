// EE569 Homework Assignment #2
// Date:     Feb 7, 2017
// Name:     Adigun Olaoluwa
// ID:       2543-8895-69
// Email:    adigun@usc.edu


#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <typeinfo>
#include <string>
#include <math.h>

using namespace std;

// Apply the bilinear interpolation on fractional pixel
//   index. This method finds the corresonding value.
// @params image array arr3D, pixel index (x,y), and channel z
// @return integer value for the fractional pixel index  
int bilinear_interpolate(float x, float y, int z, unsigned char ***arr3D){
	int val;
	float F, delX, delY;

	delX = x - int(x);
	delY = y - int(y);

	int x0 = int(x);
	int y0 = int(y);

	// Compute the fractional pixel value 
	F = (1-delX)*(1-delY)* int(arr3D[x0 +1][y0 +1][z]);
	F = F + (delX * (1-delY) * int(arr3D[x0 +2][y0 +1][z]));
	F = F + ((1-delX) * delY * int(arr3D[x0 +1][y0 +2][z]));
	F = F + (delX * delY * int(arr3D[x0+2][y0 +2][z]));

	if (F > 255.0){
	   F = 255.0;
	}
	return int(round(F));
}




// Main function for spatial warping
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

	// Initialize the pointers to the image arrays
	// Augument the array at the border
	unsigned char ***arr3D = new unsigned char**[Size1 + 2];
	
	for(int i = 0; i < Size1 + 2; i++){
   	   arr3D[i] = new unsigned char*[Size + 2];
   	   for(int j = 0; j < Size + 2; j++){
       	      arr3D[i][j] = new unsigned char[BytesPerPixel];
               for(int k = 0; k < BytesPerPixel; k++){
           	   if (i == 0 || i == Size + 1 || j == 0 || j == Size + 1){
			arr3D[i][j][k] = 0;
		   }
		   else{
			 arr3D[i][j][k] = Imagedata[i-1][j-1][k];	
 		   } 
               }
            }
        }
	
	// Pad the added columns
	for (int k = 0; k < BytesPerPixel; k++){
	     for (int i = 0; i < Size1+2; i++){
		arr3D[i][0][k] = arr3D[i][1][k];
		arr3D[i][Size+1][k] = arr3D[i][Size][k];
	     }
	}
	
	// Pad the added rows
	for (int k = 0; k < BytesPerPixel; k++){
	    for (int j = 0; j < Size + 2; j++){
		arr3D[0][j][k] = arr3D[1][j][k];
		arr3D[Size1+1][j][k] = arr3D[Size1][j][k];
	     
	    }
	}

	// Define the local variables
	int x0 = 0;
	int x1 = Size1/2;
	int y0 = 0;
	int y1 = Size/2; 
	
	float step;
	float p1, p2, q1,q2, delx, delY;
	int val;
	
        // Apply Bilinear interpolation along the rows
	unsigned char Imagedata1[Size1][Size][BytesPerPixel];
	
	for (int i = 0; i < x1; i++){
	    if (i == 0){
	        Imagedata1[i][x1-1][0] = arr3D[1][1][0];
		Imagedata1[i][x1][0] = arr3D[1][Size1][0];
		Imagedata1[Size-1-i][x1-1][0] = arr3D[Size][1][0];
		Imagedata1[Size-1-i][x1][0] = arr3D[Size][Size1][0];
		
	    }
	    else {
		val = x1 -1;
		step = (val-x0) / (1.0*i);
		
		for (int j = i; j >= 0; j--){
		    
		   p1 = i;
		   p2 = Size1 - 1 -p1;
		   q1 = step*j;
		   q2 = Size -1 -q1;

		   for (int k = 0; k < BytesPerPixel; k++){
		       Imagedata1[i][val][k] = char(bilinear_interpolate(p1, q1, k, arr3D));
		       Imagedata1[i][Size-1-val][k] = char(bilinear_interpolate(p1, q2, k, arr3D));

		       Imagedata1[Size1-1-i][val][k] = char(bilinear_interpolate(p2, q1, k, arr3D));
		       Imagedata1[Size1-1-i][Size-1-val][k] = char(bilinear_interpolate(p2, q2, k, arr3D));
		   }
		  
		   val = val - 1;
		}
	    }
	}
		
	
	///// Write image data (filename specified by second argument) from image data matrix
	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata1, sizeof(unsigned char), Size1*Size*BytesPerPixel, file);
	fclose(file);

	return 0;
}



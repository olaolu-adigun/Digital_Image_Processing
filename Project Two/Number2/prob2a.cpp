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

// Apply the Bayer matrix I2 to a pixel value
// @params pixel value F, index of the pixel (i,j)
// @return true if F > threshold and false otherwise
bool BayerC2(int F, int i, int j){
	
	// Define the I2 Bayer matrix
	int C[2][2] = { {0,2},
			{3,1} };
			
	// Compute the threshold value
	float threshold = (C[i % 2][j % 2] + 0.5) * (255.0/4.0);
	
	// Compute transformed value
	if(F > threshold){
		return true;               
	}
	return false;
}

// Apply the Bayer matrix I4 to a pixel value
// @params pixel value F, index of the pixel (i,j)
// @return true if F > threshold and false otherwise
bool BayerC4(int F, int i, int j) {

	// Define the I4 Bayer matrix
	int C[4][4] = { {0, 8, 2,10},
			{12,4, 14,6},
			{3, 11, 1,9},
			{15,7,13, 5} };
	
	// Compute the threshold value
	float threshold = (C[i % 4][j % 4] + 0.5) *(255.0/ 16.0);
	
	// Compute the threshold value
	if(F > threshold){
		return true;               
	}
	return false;
}

// Apply the Bayer matrix I8 to a pixel value
// @params pixel value F, index of the pixel (i,j)
// @return true if F > threshold and false otherwise
bool BayerC8(int F, int i, int j) {

	// Define the I8 Bayer matrix 
	int C[8][8] = { { 0,48,12,60, 3,51,15,63},
			{32,16,44,28,35,19,47,31},
			{ 8,56, 4,52,11,59, 7,55},
			{40,24,36,20,43,27,39,23},
			{ 2,50,14,62,01,43,27,61},
			{34,18,46,30,33,17,45,29},
			{10,58,06,54, 9,57,05,53},
			{42,26,38,22,41,25,37,21} };

	// Compute the threshold
	float threshold = (C[i % 8][j % 8] + 0.5) *(255.0/ 64.0);
	
	// Compute the transformed value
	if(F > threshold){
		return true;               
	}
	return false;
}

// Apply the matrix A4 to a pixel value
// @params pixel value F, index of the pixel (i,j)
// @return true if F > threshold and false otherwise
bool BayerA4(int F, int i, int j) {

	// Define the A4 matrix
	int A[4][4] = { {14,10,11,15},
			{ 9, 3, 0, 4},
			{ 8, 2, 1, 5},
			{13, 7, 6,12} };
	
	// Compute the threshold value 
	float threshold = (A[i % 4][j % 4] + 0.5) *(255.0/ 16.0);
	
	// Compute the  transformed value
	if(F > threshold){
		return true;               
	}
	return false;
}


// This is the main function that handles the Dithehring
// It takes in an image and applies the I2, I4, I8, and A4 
//    matrices for image dithering. 
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
	unsigned char Imagedata1[Size1][Size][BytesPerPixel];
	unsigned char Imagedata[Size1][Size][BytesPerPixel];
	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size1*Size*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	
	// Define the variables
	bool check;
	int val;
	
	// Use Bayer_C2
	
	for(int i = 0; i < Size1 ; i++){
   	   for(int j = 0; j < Size ; j++){
	      // Apply the C2 matrix
               check = BayerC2(int(Imagedata[i][j][0]),i,j);
	       if (check == true){
		   Imagedata1[i][j][0] = char(255);
	       }
	       else {
	           Imagedata1[i][j][0] = char(0);
	       }	 
           }
        }
    
	// Save the C2 dithered image
	file = fopen("BayerC2.raw","wb");
	fwrite(Imagedata1, sizeof(unsigned char), Size1*Size*BytesPerPixel, file);
	fclose(file);

	// Use BayerC4

	for(int i = 0; i < Size1 ; i++){
   	    for(int j = 0; j < Size ; j++){
		// Apply the C4 matrix
                check = BayerC4(int(Imagedata[i][j][0]),i,j);
	        if (check == true){
		    Imagedata1[i][j][0] = char(255);
		}
		else {
		    Imagedata1[i][j][0] = char(0);
		}	 
            }
         }
    
        // Save the C4 dithered image	
	file = fopen("BayerC4.raw","wb");
	fwrite(Imagedata1, sizeof(unsigned char), Size1*Size*BytesPerPixel, file);
	fclose(file);

	// Use BayerA4

	for(int i = 0; i < Size1 ; i++){
   	    for(int j = 0; j < Size ; j++){
	       // Apply the A4 matrix to each pixel value
               check = BayerA4(int(Imagedata[i][j][0]),i,j);
	       if (check == true){
	           Imagedata1[i][j][0] = char(255);
	       }
	       else {
		   Imagedata1[i][j][0] = char(0);
	       }	 
           }
        }	
        // Write the A4 dithered image into the output file
	file=fopen("BayerA4.raw","wb");
	fwrite(Imagedata1, sizeof(unsigned char), Size1*Size*BytesPerPixel, file);
	fclose(file);

	// Use BayerC8
	for(int i = 0; i < Size1 ; i++){
   	    for(int j = 0; j < Size ; j++){
                check = BayerC8(int(Imagedata[i][j][0]),i,j);
	        // Apply the C8 matrix to each pixel value
	        if (check == true){
		     Imagedata1[i][j][0] = char(255);
		}
		else {
		     Imagedata1[i][j][0] = char(0);
		}	 
            }
        }	
    
    	// Write the C8 dithered image into an output file 
	file = fopen("BayerC8.raw","wb");
	fwrite(Imagedata1, sizeof(unsigned char), Size1*Size*BytesPerPixel, file);
	fclose(file);

	// My proposed method
	
	for(int i = 0; i < Size1 ; i++){
   	   for(int j = 0; j < Size ; j++){
       	   	val = int(Imagedata[i][j][0]);
	       if (0 <= val && val < 42){
		    Imagedata[i][j][0] = char(0);
	 	}
		else if (42 <= val && val < 127){
		    Imagedata[i][j][0] = char(85);
		}
		else if (127 <= val && val < 212){
		    Imagedata[i][j][0] = char(170);
		}
		else{
		    Imagedata[i][j][0] = char(255);
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

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
// @params 2D array pointer, number of rows, and number of columns
// @returns the pointer to the array of the distribution
// PRE -- 0 <= int(arr[i][j]) <= 255
//     -- 0 <= i <= Size1
//     -- 0 <= i <= Size
int * freq(unsigned char ** arr2D, int Size1, int Size){
	
	int *dist = new int[256];

        for (int i = 0; i < 256 ; i++){
	   dist[i] = 0;
	}

	int value = 0;
	for(int i = 0; i< Size1; i++){
   	   for(int j = 0; j < Size; j++){
       	      value = int(arr2D[i][j]);
	      dist[value] = dist[value] + 1;
           }
        }
	return dist;
}



// This function finds the frequency distribution and saves it into a file
// @params file name, 2D array, number of rows, and number of columns
// PRE -- 0 <= arr[i][j] <= 255
//     -- 0 <= i <= Size1
//     -- 0 <= j <= Size
void save_dist (string name, unsigned char ** arr2D, int Size1, int Size){

     int len = string(name).length();

     char *ptr = new char [len];
     for(int i = 0; i < len; i++){
	
	ptr[i] = name[i];
     }

     int *pt = freq(arr2D, Size1, Size);
 
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
	file = fopen("pepper_noisy.raw","rb");
	fread(Imagedata1, sizeof(unsigned char), Size1*Size*BytesPerPixel, file);
	fclose(file);
	
	unsigned char **noiseless_red = new unsigned char*[Size1];
	unsigned char **noiseless_green = new unsigned char*[Size1];
	unsigned char **noiseless_blue = new unsigned char*[Size1];

	unsigned char **noisy_red = new unsigned char*[Size1];
	unsigned char **noisy_green = new unsigned char*[Size1];
	unsigned char **noisy_blue = new unsigned char*[Size1];

	for(int i = 0; i< Size1; i++){
   	   noiseless_red[i] = new unsigned char[Size];
	   noiseless_green[i] = new unsigned char[Size];
	   noiseless_blue[i] = new unsigned char[Size];

	   noisy_red[i] = new unsigned char[Size];
	   noisy_green[i] = new unsigned char[Size];
	   noisy_blue[i] = new unsigned char[Size];

   	   for(int j = 0; j < Size; j++){
       	      noiseless_red[i][j] = Imagedata[i][j][0];
	      noisy_red[i][j] = Imagedata1[i][j][0];

	      noiseless_green[i][j] = Imagedata[i][j][1];
	      noisy_green[i][j] = Imagedata1[i][j][1];
	 
	      noiseless_blue[i][j] = Imagedata[i][j][2];
	      noisy_blue[i][j] = Imagedata1[i][j][2];
	     
           }
        }	
	
	save_dist ("Noiseless_Red.csv", noiseless_red, Size1, Size);
	save_dist ("Noiseless_Green.csv", noiseless_green, Size1, Size);
	save_dist ("Noiseless_Blue.csv", noiseless_blue, Size1, Size);
	
	save_dist ("Noisy_Red.csv", noisy_red, Size1, Size);
	save_dist ("Noisy_Green.csv", noisy_green, Size1, Size);
	save_dist ("Noisy_Blue.csv", noisy_blue, Size1, Size);
	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), Size1*Size*BytesPerPixel, file);
	fclose(file);

	return 0;
}


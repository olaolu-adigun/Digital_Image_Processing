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
int * cumm_freq(unsigned char ** arr2D, int Size1, int Size){
	
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



// This function finds the maximum and minimum values in a 2D array.
// @params 2D array, number of rows, and number of columns
// @returns the pointer to the array containing the maximum and minimum values
// PRE -- 0 <= int(ptr[i][j]) <= 255
//     -- 0 <= i <= Size1
//     -- 0 <= i <= Size
int *  min_max(unsigned char **ptr, int rows, int cols){
 
	int *m = new int[2];
	m[0] = 255;
	m[1] = 0;
	int value;

	for (int i = 0; i < rows; i++){
	   for (int j = 0; j < cols; j++){
	      value = int(ptr[i][j]);
	      if (value < m[0]){
	         m[0]= value; 
	      }
	      if (value > m[1]){
		 m[1]= value; 
	      }
  	   }
	}
	return m;
}



// This function finds the transfer function and converts the image with it.
// @params 2D array pointer, number of rows, and number of columns
// @Returns 2D array pointer	     
// PRE -- 0 <= arr[i][j] <= 255
//     -- 0 <= i <= Size1
//     -- 0 <= i <= Size
unsigned char** trans_Fxn(unsigned char **ptr, int Size1, int Size){
    
	int* m = min_max(ptr, Size1, Size);
	int r = m[1] - m[0];
	int x,y;
	float step = 255.0 / r; 
	for (int i = 0; i < Size1; i++){
	   for (int j = 0; j < Size; j++){
	       x = int(ptr[i][j]);
	       y = round((x - m[0]) * step);
	       ptr[i][j] = (char)y;
	   }
	}
	return ptr;	
}


// This function finds the frequency distribution and saves it into a file
// @params 2D array, number of rows, and number of columns
// PRE -- 0 <= arr[i][j] <= 255
//     -- 0 <= i <= Size1
//     -- 0 <= i <= Size
void save_dist (string name, unsigned char ** arr2D, int Size1, int Size){

     int len = string(name).length();

     char *ptr = new char [len];
     for(int i = 0; i < len; i++){
	
	ptr[i] = name[i];
     }

     int *pt = cumm_freq(arr2D, Size1, Size);
 
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
	
	// Define the pointers for the channels	
	unsigned char **arr2D_red = new unsigned char*[Size1];
	unsigned char **arr2D_green = new unsigned char*[Size1];
	unsigned char **arr2D_blue = new unsigned char*[Size1];

	for(int i = 0; i< Size1; i++){
   	   arr2D_red[i] = new unsigned char[Size];
	   arr2D_green[i] = new unsigned char[Size];
	   arr2D_blue[i] = new unsigned char[Size];
   	   for(int j = 0; j < Size; j++){
       	      arr2D_red[i][j] = Imagedata[i][j][0];
	      arr2D_green[i][j] = Imagedata[i][j][1];
	      arr2D_blue[i][j] = Imagedata[i][j][2];
           }
        }	
	
	// Save the histogram of the channels before equalization
	//save_dist ("Image_Before_Equalization_Red.csv", arr2D_red, Size1, Size);
	//save_dist ("Image_Before_Equalization_Green.csv", arr2D_green, Size1, Size);
	//save_dist ("Image_Before_Equalization_Blue.csv", arr2D_blue, Size1, Size);

	// Perform equalization on the channels
	arr2D_red = trans_Fxn(arr2D_red, Size1, Size);
	arr2D_green = trans_Fxn(arr2D_green, Size1, Size);
	arr2D_blue = trans_Fxn(arr2D_blue, Size1, Size);
 	
	for(int i = 0; i< Size1; i++){
   	   for(int j = 0; j < Size; j++){
       	      Imagedata[i][j][0] = arr2D_red[i][j]; 
	      Imagedata[i][j][1] = arr2D_green[i][j];
	      Imagedata[i][j][2] = arr2D_blue[i][j];
           }
        }

	//Save the histogram of the channels after equalization
	//save_dist ("Image_Equalized_Distribution_Linear_Red.csv", arr2D_red, Size1, Size);
	//save_dist ("Image_Equalized_Distribution_Linear_Green.csv", arr2D_green, Size1, Size);
	//save_dist ("Image_Equalized_Distribution_Linear_Blue.csv", arr2D_blue, Size1, Size);

	// Write image data (filename specified by second argument) from image data matrix
	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), Size1*Size*BytesPerPixel, file);
	fclose(file);

	return 0;
}

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
// params 2D array, number of rows, and number of columns
// Returns the pointer to the array of the distribution
// PRE -- 0 <= arr[i][j] <= 255
//     -- 0 <= i <= Size1
//     -- 0 <= i <= Size
int * cum_freq(unsigned char ** arr2D, int Size1, int Size){
	
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
// params 2D array, number of rows, and number of columns
// Returns the pointer to the array containing the maximum and minimum values
// PRE -- 0 <= arr[i][j] <= 255
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



// This function computes uniform frequency distribution of over 0-255.
// params Size1, Size
// Returns the pointer to the array of uniform distribution
// PRE -- 0 < Size1
//     -- 0 < Size
int * unif_dist(int Size1, int Size){

	int *pt = new int[256];
	int x = (Size*Size1) /256;
	for (int i = 0; i < 256; i++){
	    pt[i] = x;
	}

	int res = (Size*Size1) % 256;
	if (res > 0){
	   for (int i = 0; i < res; i++){
	        pt[i] = pt[i] + 1;
	   } 
	}
	return pt;
}



// This function assigns values to the pixels according to a given distribution
// params 2D array type int (input)
//        2D array of type char holds the output (output)
//        1D array containing minimum and maximum values input(min_max)
//    	  1D array holding the initial frequency distribution (cdf_init)
//        1D array holding the expected frequency distribution (cdf_final)
//	  numbers of rows (Size), number of columns (Size1)
// Returns the output array after new pixel value
// PRE -- 0 <= input[i][j] <= 255
//     -- 0 <= i <= Size1
//     -- 0 <= j <= Size
unsigned char ** task( int ** input , unsigned char ** output, int* min_max, int * cdf_init, int * cdf_final, int Size1, int Size){

	int val = 0;
	int val_search = min_max[0];

	while(val < 256){
	   
	   for (int i = 0; i < Size1; i++){

	      for (int j = 0; j < Size; j++){
	         if (input[i][j] == val_search){
		    output[i][j] = char(val);
		    cdf_init[val_search] = cdf_init[val_search] - 1;
		    cdf_final[val] = cdf_final[val] - 1;
		 }
		 while(cdf_init[val_search] == 0){
		   
		   val_search = val_search + 1;
                   if (val_search > min_max[1]){
		      return output;
		   } 
		 }
		 if(cdf_final[val] == 0){
		
		   val = val + 1;
		   if (val == 256){
		      return output;
		   }
		 } 	   
	      } 
	   } 
	}
	return output;
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

     int *pt = cum_freq(arr2D, Size1, Size);
 
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
	unsigned char Imagedata[Size1][Size];
	
	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size1*Size, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	
	// Initialize the pointers to the image array
	unsigned char **arr2D = new unsigned char*[Size1];
        int **arr2D_num = new int *[Size1];

	for(int i = 0; i< Size1; i++){
   	   arr2D[i] = new unsigned char[Size];
	   arr2D_num[i] = new int [Size];
   	   for(int j = 0; j < Size; j++){
       	      arr2D[i][j] = Imagedata[i][j];
	      arr2D_num[i][j] = int(Imagedata[i][j]);;
           }
        }	
	
	// Save the frequency distribution of the  image before histogram equalization
	save_dist ("Image_Before_Equalization.csv", arr2D, Size1, Size);

	// Perform digital implementation of histogram equalization
	arr2D = task( arr2D_num , arr2D, min_max(arr2D, Size1, Size), cum_freq(arr2D, Size1, Size), unif_dist(Size1, Size), Size1, Size);
	
	//Copy the output pointer to output array
	for(int i = 0; i< Size1; i++){
   	   for(int j = 0; j < Size; j++){
       	      Imagedata[i][j] = arr2D[i][j];
           }
        }
	
	// Save the frequency distribution of the image after histogram equalization
	save_dist ("Image_After_Equalization.csv", arr2D, Size1, Size);

	// Write image data (filename specified by second argument) from image data matrix
	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), Size1*Size, file);
	fclose(file);

	return 0;
}


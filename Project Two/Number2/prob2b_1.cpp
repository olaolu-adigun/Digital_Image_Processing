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


const double PI = 3.14159;
using namespace std;

// This function truncates the pixel values greater than 255
//   and values less than 0
// @param pixel value v
// @return truncated value if 0<= v <= 255
int trim (int v){

	// Truncate pixel value less than 0
	if (v < 0){
	   return 0;
	}
	// Truncate pixel value greater than 255
 	if (v > 255){
	   return 255;
	}	
	return v;
}

// Augment the boundary of a given image array
// @params image arr arr2D, num of rows (Size1),
//     num of cols (Size), and num of augments
// @return output of augmentation
unsigned char*** augment_image(unsigned char*** arr2D, int Size1, int Size, int offset){

	// Initialize the new image array
	unsigned char*** arr = new unsigned char**[Size1 + (2*offset)];
	
	for(int i = 0; i < Size1 + (2*offset); i++){
   	   arr[i] = new unsigned char*[Size + (2*offset)];
   	   for(int j = 0; j < Size + (2*offset); j++){
       	       arr[i][j] = new unsigned char[1];
       	       arr[i][j][0] = char(255);
           }
        }
        
	// Copy the original image content
	for (int i = 0; i < Size1; i++){
	    for (int j = 0; j < Size;  j++){
	        arr[i+offset][j+offset][0] = arr2D[i][j][0];
	    }
	}
	return arr;
}

// Apply Floydstein error diffusion matrix to an image pixel
//    scanning from left hand side to the right hand side.
// @params image array arr2D, index of pixel (i,j)
// @return image array
unsigned char *** HandleSerpentine_FloydStein (unsigned char *** arr, int i, int j){
	
	// Define the local variables
	int F, G, err;
	F = int(arr[i][j][0]);
	
	// Apply the threshold rule
	if (F > 127){
	  G = 255;
	}
	else{
	  G = 0;
	}
	
	err = F - G;
	arr[i][j][0] = char(G);
	
	// Apply the diffusion matrix
	arr[i][j+1][0] = char(trim(int(arr[i][j+1][0]) + int(round(err*(7.0/16.0)))));
	arr[i+1][j+1][0] = char(trim(int(arr[i+1][j+1][0])+ int(round(err *(1.0/16.0)))));
	arr[i+1][j][0]   = char(trim(int(arr[i+1][j][0])  + int(round(err *(5.0/16.0)))));
	arr[i+1][j-1][0] = char(trim(int(arr[i+1][j-1][0])+ int(round(err *(3.0/16.0)))));
	
	return arr;
}


// Apply Floydstein error diffusion matrix to an image pixel
//    scanning from left hand side to the right hand side.
// @params image array arr2D, index of pixel (i,j)
// @return image array
unsigned char *** HandleSerpentine_FloydStein1 (unsigned char *** arr, int i, int j){
	
	// Define the local variables
	int F, G, err;
	F = int(arr[i][j][0]);
	// Apply the threshold rule
	if (F > 127){
	  G = 255;
	}
	else{
	  G = 0;
	}
	err = F - G;
	arr[i][j][0] = char(G);
	
	// Apply the diffusion matrix
	arr[i][j-1][0] = char(trim(int(arr[i][j-1][0]) + int(round(err*(7.0/16.0)))));
	arr[i+1][j-1][0] = char(trim(int(arr[i+1][j-1][0])+ int(round(err *(1.0/16.0)))));
	arr[i+1][j][0]   = char(trim(int(arr[i+1][j][0])  + int(round(err *(5.0/16.0)))));
	arr[i+1][j+1][0] = char(trim(int(arr[i+1][j+1][0])+ int(round(err *(3.0/16.0)))));
	return arr;
}


// Apply Floydstein error diffusion matrix to an image array
// @params image array arr2D, num of rows (Size1),
//      and num of cols (Size)
// @return output image after using the Floydstein matrix 
unsigned char*** FloydStein(unsigned char*** arr2D, int Size1, int Size) {
	
	int offset = 1;
	unsigned char *** arr = augment_image(arr2D, Size1, Size, offset);
	for (int i = offset; i < Size1 + offset; i++){
		// Determine which type of Floydstein matrix to use and apply it
	   if (i%2 == 0){
	      for (int j = offset; j < Size + offset; j++){
	         arr = HandleSerpentine_FloydStein(arr,i,j);
	      }
	   }
	   else{
	      for (int j = Size + offset -1; j >= offset; j--){
		       arr = HandleSerpentine_FloydStein1(arr,i,j);	  
	      }
	   }
	}
	return arr;
}

// Apply JJN error diffusion matrix to an image array
// @params image array arr2D, num of rows (Size1),
//      and num of cols (Size)
// @return output image after using the Floydstein matrix 
unsigned char *** Jarvis_Judice_Ninke(unsigned char*** arr, int Size1, int Size){
	// Define local variables
	int offset = 2;
	int F, G, err;
	
	arr = augment_image(arr, Size1, Size, offset);
	// Apply the Jarvis Judice Ninke Diffusion error matrix
	for (int i = offset; i < Size1 + offset; i++){
	    for (int j = offset; j < Size + offset; j++){
	        F = int(arr[i][j][0]);
		if (F > 127){
	  	   G = 255;
		}
		else{
	  	   G = 0;
		}
		err = F - G;
		
		// Use JJN difussion matrix
		/***
		arr[i][j][0] = char(G);
		arr[i][j+1][0] = char(trim(int(arr[i][j+1][0]) + int(round(err*(7.0/48.0)))));
		arr[i][j+2][0] = char(trim(int(arr[i][j+2][0]) + int(round(err*(5.0/48.0)))));

		arr[i+1][j+2][0] = char(trim(int(arr[i+1][j+2][0]) + int(round(err *(3.0/48.0)))));
		arr[i+1][j+1][0] = char(trim(int(arr[i+1][j+1][0]) + int(round(err *(5.0/48.0)))));
		arr[i+1][j][0]   = char(trim(int(arr[i+1][j][0])   + int(round(err *(7.0/48.0)))));
		arr[i+1][j-1][0] = char(trim(int(arr[i+1][j-1][0]) + int(round(err *(5.0/48.0)))));
		arr[i+1][j-2][0] = char(trim(int(arr[i+1][j-2][0]) + int(round(err *(3.0/48.0)))));

		arr[i+2][j+2][0] = char(trim(int(arr[i+2][j+2][0]) + int(round(err *(1.0/48.0)))));
		arr[i+2][j+1][0] = char(trim(int(arr[i+2][j+1][0]) + int(round(err *(3.0/48.0)))));
		arr[i+2][j][0]   = char(trim(int(arr[i+2][j][0])   + int(round(err *(5.0/48.0)))));
		arr[i+2][j-1][0] = char(trim(int(arr[i+2][j-1][0]) + int(round(err *(3.0/48.0)))));
		arr[i+2][j-2][0] = char(trim(int(arr[i+2][j-2][0]) + int(round(err *(1.0/48.0)))));
		***/
		
	   }
	}
	return arr;

}




// This is the main function for dithering with
//    error diffusion matrices.
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
	//unsigned char Imagedata1[Size1][Size][BytesPerPixel];
	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size1*Size*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	unsigned char Imagedata1[Size1][Size][BytesPerPixel];
	unsigned char ***arr2D = new unsigned char**[Size1];

	for(int i = 0; i < Size1 ; i++){
   	   arr2D[i] = new unsigned char*[Size];
   	   for(int j = 0; j < Size ; j++){
       	      arr2D[i][j] = new unsigned char[BytesPerPixel];
               for(int k = 0; k < BytesPerPixel; k++){
		 arr2D[i][j][k] = Imagedata[i][j][k];
               }
            }
        }

	///// FloydStein
	unsigned char *** arrFS = FloydStein(arr2D, Size1, Size);
	
	for (int i = 0; i < Size1; i++){
	    for (int j = 0; j < Size; j++){
		Imagedata1[i][j][0] = arrFS[i+1][j+1][0];
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



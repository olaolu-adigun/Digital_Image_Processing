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

// Compute the euclidean disatnce between two points on an image board
// @params the 2 points (x0, y0) and (x1, y1)
// @return the distance between the points
float euclidean_dist(float x0, float y0, float x1, float y1){
	// Compute the euclidean distance
	return sqrt(pow((x1-x0),2.0) + pow((y1-y0),2.0));

}


// Find the closest non-fractional pixel index to a point
// @params the coordinate of the pixel
// @return array containing the closest non-fractional pixel index
int * closest (float x, float y ){

	int *val = new int[2];
	float dist = 5;
	// Test Right down corner
	if (dist > euclidean_dist(x,y, ceil(x),ceil(y))){
	   val[0] = int(ceil(x));
	   val[1] = int(ceil(y));
	   dist = euclidean_dist(x,y, ceil(x),ceil(y));
	}
	// Test left down corner
	if (dist > euclidean_dist(x,y, floor(x),ceil(y))){
	   val[0] = int(floor(x));
	   val[1] = int(ceil(y));
	   dist = euclidean_dist(x,y, floor(x),ceil(y));
	}
	// Test left up corner
	if (dist > euclidean_dist(x,y, floor(x), floor(y))){
	   val[0] = int(floor(x));
	   val[1] = int(floor(y));
	   dist = euclidean_dist(x,y, floor(x), floor(y));
	}
	// Test right up corner
	if (dist > euclidean_dist(x,y, ceil(x),floor(y))){
	   val[0] = int(ceil(x));
	   val[1] = int(floor(y));
	   dist = euclidean_dist(x,y, ceil(x),floor(y));
	}
	
	return val;
}

// Apply homographic transformation matrix to pixel location (i,j)
// @params pixel location (i, j)
// @return array containing the transformed index location

int * homographic(int i, int j){
	
	// Define H matrix
	float H11 = 1.9767;
	float H12 = 0.7422;
	float H13 = 363.00;
	float H21 = 0.7410;
	float H22 = 0.0501;
	float H23 = 571.00;
	float H31 = 0.0026;
	float H32 = 0.0000;

	// Apply H matrix
	float y = (H11*j) + (H12*i) + (H13);
	float x = (H21*j) + (H22*i) + (H23);
	float w = (H31*j) + (H32*i) + 1;

	int* val = closest (x/w, y/w); 
	return val;
}



// Main function for homographic transformation and 
//    overlay method.
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

	unsigned char Image1[197][350][BytesPerPixel];	
	file = fopen("trojans.raw", "rb");
	fread(Image1, sizeof(unsigned char), 197*350*3, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////

	// Initialize pointers to the image array 
	unsigned char ***piece = new unsigned char**[Size1];

	for(int i = 0; i < Size1 ; i++){
   	   piece[i] = new unsigned char*[Size];
   	   for(int j = 0; j < Size ; j++){
       	      piece[i][j] = new unsigned char[BytesPerPixel];
               for(int k = 0; k < BytesPerPixel; k++){
		 piece[i][j][k] = char(255);
               }
            }
        }

	// Initialize and declare the pointers to image arrays
	unsigned char ***arr3D = new unsigned char**[197];
	for(int i = 0; i < 197; i++){
   	   arr3D[i] = new unsigned char*[350];
   	   for(int j = 0; j < 350; j++){
       	      arr3D[i][j] = new unsigned char[BytesPerPixel];
               for(int k = 0; k < BytesPerPixel; k++){
		  arr3D[i][j][k] = Image1[i][j][k];
               }
            }
        }

	// Apply a homographic transformation to the "Trojans" text image 
	int* val;
	for (int i = 0; i < 197; i++){
	   for (int j = 0; j < 350; j++){
	      val = homographic(i, j);
	      for (int k = 0; k < BytesPerPixel; k++){
	         piece[val[0]][val[1]][k] = arr3D[i][j][k];
	      }
	   }
	}

	// Overlay or Blending
	for (int i = 0; i < Size1; i++){
	   for (int j = 0; j < Size ; j++){
	      if (piece[i][j][0] < 180 || piece[i][j][1] < 180 || piece[i][j][2] < 180){
	         for (int k = 0; k < BytesPerPixel; k++){
	            Imagedata[i][j][k] = piece[i][j][k];
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




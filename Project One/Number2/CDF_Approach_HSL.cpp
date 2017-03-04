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
int * freq_dist(unsigned char ** arr2D, int Size1, int Size){
	// Create output array
	int *dist = new int[256];

        for (int i = 0; i < 256 ; i++){
	   dist[i] = 0;
	}
	
	// Count the values in the array
	int value = 0;
	for(int i = 0; i< Size1; i++){
   	   for(int j = 0; j < Size; j++){
       	      value = int(arr2D[i][j]);
	      dist[value] = dist[value] + 1;
           }
        }
	return dist;
}


// This function covert the frequency distribution to cummulative frequency.
// @params frequency array
// @returns the cummulative frequency array
// PRE --For the array index i (0 <= i <= 255)
int * cumm_freq (int* dist){

	for (int i = 1; i < 256; i++){
		dist[i] = dist[i] + dist[i-1];
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
 	// Create the output array
	int *m = new int[2];
	m[0] = 255;
	m[1] = 0;
	int value;

	// Search for max value
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


// This function finds the maximum value out of 3 given values.
// @params 3 numbers
// @returns the maximum number

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


// This function finds the minimum value out of 3 given values.
// @params 3 numbers
// @returns the minimum number
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

// This function finds the range of 3 given values.
// @params 3 numbers
// @returns the minimum number
float range (int a, int b, int c)
{
  return (findMax(a,b,c) - findMin(a,b,c));
}


// This function computes the Hue value from RGB values
// @params rgb values
// @returns the hue value
// PRE --  0 <= r << 255 
//     --  0 <= g << 255
//     --  0 <= b << 255
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
	   pt = int(round(60*((u - int(u)) + (int(u) % 6))));
	}
	else if (findMax(r,g,b) == g){
	   pt = round(60*((b-r)/(c*1.0)) + 2);
	}
	else{
	   pt = round(60*((r-g)/(c*1.0) + 4));
	}
	pt = (pt+360) % 360;
	return pt;
}


// This function computes the light value from RGB values
// @params rgb values
// @returns the light value
// PRE --  0 <= r << 255 
//     --  0 <= g << 255
//     --  0 <= b << 255
double Lightness(int r, int g, int b)
{
	return (findMax(r,g,b) + findMin(r,g,b))/(2*255.0);
}


// This function computes the saturation value from RGB values
// @params rgb values
// @returns the saturation value
// PRE --  0 <= r << 255 
//     --  0 <= g << 255
//     --  0 <= b << 255
double Saturation (int r, int g, int b){

	double L = Lightness(r,g,b);
	double c = range(r,g,b)/255.0;
	double pt;
	if (L == 0.0 || L == 1.0){
	   pt = 0.0;
	}
	else if (L > 0.0 && L < 0.5){
	   pt = c/(2*L);
   	}else {
	   pt = c/(2*(1-L));
	}
	return pt;
}


// This function finds the transfer function and converts the image with it.
// @params 2D array pointer, number of rows, and number of columns
// @Returns 2D array pointer	     
// PRE -- 0 <= arr[i][j] <= 255
//     -- 0 <= i <= Size1
//     -- 0 <= i <= Size
unsigned char** trans_Fxn(unsigned char **ptr, int Size1, int Size){

	//Compute the transfer function
	unsigned char **arr2D = new unsigned char*[Size1];
	int * cummFreq = cumm_freq(freq_dist(ptr, Size1, Size));

	int* m = min_max(ptr, Size1, Size);
	//int r = m[1] - m[0];
	int x;
	double y; 
	for (int i = 0; i < Size1; i++){
	   arr2D[i] = new unsigned char[Size];
	   for (int j = 0; j < Size; j++){
	       x = int(ptr[i][j]);
	       y = (255.0 * (cummFreq[x] -cummFreq[m[0]]) ) / ((cummFreq[255]- cummFreq[m[0]])*1.0);
	       arr2D[i][j] = char(int(round(y)));
	   }
	}
	
	// Write the transfer function into an array
	ofstream unequalized;
	unequalized.open("Transfer_Function.csv");
	
	int val;
	for (int i = 0; i < 256; i++){
	   
	   val = int(round((255.0 * (cummFreq[i] -cummFreq[m[0]]) ) / ((cummFreq[255]- cummFreq[m[0]])*1.0) ));
	   if (val < 0){
		val = 0;
	   }
	   if (val > 255){
		val = 255;
	   }
	   unequalized << val << endl;
	}
	unequalized.close();

	return arr2D;	
}


// This function converts HSL values to RGB values
// @params HSL values
// @Returns 1D array containing the rgb values	     
// PRE -- 0 <= hue <= 359
//     -- 0 <= sat <= 1.0
//     -- 0 <= light <= 1.0
double* convert_HSL_to_RGB(int hue, float sat, int light){
	
	double* pt = new double[3];
	double c, h1, l1, x;
	l1 = light / 255.0;
	
	if ( l1 > 0.5){
	   c = (1 - 2*(l1-0.5))* sat;
	}
	else{
	   c = (1 - 2*(0.5-l1))* sat;
	}

	h1 = hue / 60.0;
	
	
	if (int(h1) % 2 == 0){
	   x = (h1 - int(h1))*c; 
	} 
	else{
	   x = (1 - (h1 - int(h1)))*c;	
	}
	

	if (h1 <= 1){
	   pt[0] = c;
	   pt[1] = x;
	   pt[2] = 0;
	}
	else if ((1 < h1) &&  (h1 < 2)){
	   pt[0] = x;
	   pt[1] = c;
	   pt[2] = 0;
	}
	else if ((2 < h1) &&  (h1 < 3)){
	   pt[0] = 0;
	   pt[1] = c;
	   pt[2] = x;
	}
	else if ((3 < h1) &&  (h1 < 4)){
	   pt[0] = 0;
	   pt[1] = x;
	   pt[2] = c;
	}
	else if ((4 < h1) &&  (h1 < 5)){
	   pt[0] = x;
	   pt[1] = 0;
	   pt[2] = c;
	}
	else if ((5 < h1) &&  (h1 < 6)){
	   pt[0] = c;
	   pt[1] = 0;
	   pt[2] = x;
	}
	else {
	    pt[0] = 0;
	    pt[1] = 0;
	    pt[2] = 0;	
	}

	double m = l1 - (0.5*c);
	pt[0] = pt[0] + m;
	pt[1] = pt[1] + m;
	pt[2] = pt[2] + m;
	
	return pt;
}



// This function finds the frequency distribution and saves it into a file
// @params file name, 2D array, number of rows, and number of columns
// PRE -- 0 <= arr[i][j] <= 255
//     -- 0 <= i <= Size1
//     -- 0 <= i <= Size
void save_dist (string name, unsigned char ** arr2D, int Size1, int Size){

	// Define the name of outut file
	int len = string(name).length();

	char *ptr = new char [len];
	for(int i = 0; i < len; i++){
	    ptr[i] = name[i];
	}

	int *pt = freq_dist(arr2D, Size1, Size);
 
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
	
	// Initialize the pointers to the image arrays
	// Read the input image into 3D-array pointer	
	// Initialize the 2D arrays for HSL values 
	
	int **hue = new int*[Size1];
	double **sat = new double*[Size1];
	double **light = new double*[Size1];

	unsigned char ** light_char = new unsigned char*[Size1];
	
	for(int i = 0; i< Size1; i++){

	   hue[i] = new int[Size];
	   sat[i] = new double[Size];
	   light[i] = new double[Size];
	   light_char[i] = new unsigned char[Size];

   	   for(int j = 0; j < Size; j++){
	      // Convert the RGB values to HSL values 
	      hue[i][j] = Hue((int)Imagedata[i][j][0], (int)Imagedata[i][j][1], (int)Imagedata[i][j][2]);
	      sat[i][j] = Saturation((int)Imagedata[i][j][0], (int)Imagedata[i][j][1], (int)Imagedata[i][j][2]);
	      light[i][j] = Lightness((int)Imagedata[i][j][0], (int)Imagedata[i][j][1], (int)Imagedata[i][j][2]);
	      light_char[i][j] = (char)(round(light[i][j]*255));
	      
            }
        }
	
	save_dist ("Light_Unequalized.csv", light_char, Size1, Size);

	// Perform Histogram Equalization on the light channel
	light_char = trans_Fxn(light_char, Size1, Size);

	save_dist ("Light_Equalized.csv", light_char, Size1, Size);
	
	double *a;
	for (int i = 0; i < Size1; i++){
	   for (int j= 0; j < Size; j++){
	       a = convert_HSL_to_RGB(hue[i][j], sat[i][j], int(light_char[i][j]));
	       for (int k = 0; k < 3; k++){
	           Imagedata[i][j][k] = char(int(round(a[k]*255.0)));
	       }
	   }	    
	}
	// Write image data (filename specified by second argument) from image data matrix
	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), Size1*Size*BytesPerPixel, file);
	fclose(file);

	return 0;
}

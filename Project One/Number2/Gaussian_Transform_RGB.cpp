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



// This function compute Gaussian pdf value for a give value .
// @params x, mean(mu), standard deviation (std_dev)
// @returns the pointer to the array containing the maximum and minimum values
// PRE -- 0 <= arr[i][j] <= 255
//     -- 0 <= i <= Size1
//     -- 0 <= i <= Size
double gauss_pdf(int x, double mu, double std_dev){
	
	double p;
	double pi = 3.14159;

	p = pow((x - mu),2.0) / (2*pow(std_dev,2.0));
	p = exp(-1*p);
	p = p / sqrt(2.0* pi* std_dev);
	return p;

}


// This function creates the desire gaussian distribution for a given size of pixels.
// @params number of rows (Size1)
//         number of columns(Size)
//         mean (mu)
//	   standard deviation (std_dev)
// @returns the pointer to the array containing the distribution
int * Gauss_dist(int Size1, int Size, double mu, double std_dev){

	int *pt = new int[256];
	double *g = new double[256];

	double val = 0.0;
	for (int i = 0; i < 256; i++){
	    g[i] = gauss_pdf(i, mu, std_dev);
	    val = val + g[i];
	}
	
	int check = 0;

	// Renormalization
	for (int i = 0; i < 256; i++){
	    g[i] = g[i]/val;
	    pt[i] = int(round(g[i]*Size1*Size));
	    check = check + pt[i];
	}

	int v;
	if (check < (Size1*Size)){
	    v = (Size1*Size) - check;
	    while ( v > 0){
	      pt[v % 255] = pt[v % 255] + 1;
	      v = v -1;   
	    }	
	} 
	else{
	    v = check - (Size1*Size);
	    while (v > 0){
	      pt[v % 255] = pt[v % 255] + 1;
	      v = v -1;   
	    }
	}

	return pt;
}

// This function transforms the pixel distribution according to a given frequency distribution.
// @params 2D array type int (input)
//        2D array of type char holds the output (output)
//        1D array containing minimum and maximum values input(min_max)
//    	  1D array holding the initial frequency distribution (cdf_init)
//        1D array holding the expected frequency distribution (cdf_final)
//	  numbers of rows (Size), number of columns (Size1)
// @returns the output array after new pixel value
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
	
	unsigned char **arr_red = new unsigned char*[Size1];
	unsigned char **arr_green = new unsigned char*[Size1];
	unsigned char **arr_blue = new unsigned char*[Size1];

        int **num_red = new int *[Size1];
	int **num_green = new int *[Size1];
	int **num_blue = new int *[Size1];

	for(int i = 0; i< Size1; i++){
   	   arr_red[i] = new unsigned char[Size];
	   arr_green[i] = new unsigned char[Size];
	   arr_blue[i] = new unsigned char[Size];

	   num_red[i] = new int [Size];
	   num_green[i] = new int [Size];
	   num_blue[i] = new int [Size];

   	   for(int j = 0; j < Size; j++){
       	      arr_red[i][j] = Imagedata[i][j][0];
	      num_red[i][j] = int(Imagedata[i][j][0]);

	      arr_green[i][j] = Imagedata[i][j][1];
	      num_green[i][j] = int(Imagedata[i][j][1]);

	      arr_blue[i][j] = Imagedata[i][j][2];
	      num_blue[i][j] = int(Imagedata[i][j][2]);
           }
        }	
	
	save_dist ("Image_Unequalized_Gaussian_Red.csv", arr_red, Size1, Size);
	save_dist ("Image_Unequalized_Gaussian_Green.csv", arr_green, Size1, Size);
	save_dist ("Image_Unequalized_Gaussian_Blue.csv", arr_blue, Size1, Size);
	
	double mu = 70.0;
	double sdev = 100.0;
	arr_red = task(num_red,arr_red, min_max(arr_red,Size1,Size), freq(arr_red,Size1,Size), Gauss_dist(Size1,Size,mu,sdev), Size1, Size);
	arr_green = task(num_green,arr_green, min_max(arr_green,Size1,Size), freq(arr_green,Size1,Size), Gauss_dist(Size1,Size,mu, sdev),Size1,Size);
	arr_blue = task(num_blue,arr_blue,min_max(arr_blue,Size1,Size), freq(arr_blue,Size1,Size), Gauss_dist(Size1,Size,mu, sdev), Size1,Size);

	for(int i = 0; i< Size1; i++){
   	   for(int j = 0; j < Size; j++){

       	      Imagedata[i][j][0] = arr_red[i][j];
	      Imagedata[i][j][1] = arr_green[i][j];
	      Imagedata[i][j][2] = arr_blue[i][j];
           }
        }
	
	save_dist ("Image_Equalized_Gaussian_CDF_Red.csv", arr_red, Size1, Size);
	save_dist ("Image_Equalized_Gaussian_CDF_Green.csv", arr_green, Size1, Size);
	save_dist ("Image_Equalized_Gaussian_CDF_Blue.csv", arr_blue, Size1, Size);
	


	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata, sizeof(unsigned char), Size1*Size*BytesPerPixel, file);
	fclose(file);

	return 0;
}


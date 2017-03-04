// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//		RAW format. You will need to make corresponding changes to
//		accommodate images of different sizes and/or types


//To use this program for another image, remove the "pepper_noisy.raw"   
// under the main function and replace it with the desired noise image

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

// This function computes the PSNR
// @params 2D array pointer to noisy image
//         2D array pinter to noiseless image
//         BytesPerPixel
//         number of rows, and number of columns
// PRE -- 0 <= int(arr[i][j]) <= 255
//     -- 0 <= i <= Size1
//     -- 0 <= i <= Size
void compute_psnr(unsigned char ***noisy, unsigned char ***noiseless, int Size1, int Size, int BytesPerPixel){
	float p = 0;
	for (int k = 0; k < BytesPerPixel; k++){

	   
	   for (int i =  0; i < Size1;i++){
	       for (int j = 0; j < Size; j++){
		   p = p + pow(int(noisy[i][j][k]) - int(noiseless[i][j][k]) , 2.0);		
	       }
	   } 
	}
	p = p/(Size1*Size*3.0);
	p = 10 * log10(65025 / p);
	cout << "PSNR: " << p << endl;
}


// This function generates the index of of windowed pixel around (x,y)
// The windows is a squuared centered at (x,y)
// @params (x,y) is the center pixel 
//         number of rows Size1
//         number of columns Size
//         window is the size of window
// @return 2D array containing neighboring index
// PRE--- window can only be an odd number (1,3,5,7,9, ...) 
int** gen_index(int x, int y, int Size1, int Size, int window){
	
	int offset = window / 2;
	int ind = 0;
	int r = 0;
	int x1 , y1;

	int** indices = new int*[window*window];
	for (int i = 0; i < window*window ; i++){
	   indices[i] = new int[2];
	   for (int j = 0; j < 2; j++){
	     indices[i][j] = -10;
	   }
	}

	for (int i = -1*offset; i <= offset; i++){
	    for (int j = -1*offset; j <= offset; j++){
				
		x1 = x + i;
		y1 = y + j;

		if (x1 < 0){
		   x1 = 0;
		}
		if (x1 > Size1-1)
		{
		   x1 = Size1 - 1;
		}

		if (y1 < 0){
		   y1 = 0;
		}
		if (y1 > Size-1)
		{
		   y1 = Size - 1;
		}		
		
		//cout << "x: " << x1 << " y: " << y1 << endl;		
  
		indices[ind][0] = x1;
		indices[ind][1] = y1;
		ind = ind + 1;
	    }
	}
	return indices;
}


// This function generates the index of a windowed pixel around (x,y)
// The window is T-shaped centered at (x,y)
// @params (x,y) is the center pixel 
//         number of rows Size1
//         number of columns Size
//         window is the size of window
// @return 2D array containing neighboring index
// PRE--- window can only be an odd number (1,3,5,7,9, ...)
int** gen_index1(int x, int y, int Size1, int Size, int window){
	
	int offset = window / 2;
	int ind = 0;
	int r = 0;
	int x1 , y1;

	int** indices = new int*[(2*window)-1];
	for (int i = 0; i < (2*window)-1 ; i++){
	   indices[i] = new int[2];
	   for (int j = 0; j < 2; j++){
	     indices[i][j] = -10;
	   }
	}
	
	for (int i = -1*offset; i <= offset; i++){
	    for (int j = -1*offset; j <= offset; j++){
	    		
	     	if (i ==  0 || j == 0){		
		     x1 = x + i;
		     y1 = y + j;

		     if (x1 < 0){
		         x1 = 0;
		     }
		     if (x1 > Size1-1){
		         x1 = Size1 - 1;
		     }

		     if (y1 < 0){
		        y1 = 0;
		     }
		     if (y1 > Size-1){
		        y1 = Size - 1;
		     }		
		
		    cout << "x: " << x1 << " y: " << y1 << endl;		
  
		     indices[ind][0] = x1;
		     indices[ind][1] = y1;
		     ind = ind + 1;
		 }
		
	    }
	}
	return indices;
}
 

// This function generates the coefficient for a linear average window
// @params (x,y) window is the size of window
// @return 1D array containing the coefficients
// PRE--- window_size can only be an odd number (1,3,5,7,9, ...)
float * linear1_window(int window_size){

	float* window = new float[window_size*window_size];
	for (int i = 0; i < window_size*window_size ; i++){
	   window[i] = 1.0 / (window_size*window_size);
	}
	return window;
}

// This function generates the coeeficient for a modified linear window. 
// Center pixel with concentrated weight and others with 1 
// @params (x,y) window is the size of window
// @return 1D array containing the coefficients
// PRE--- window_size can only be an odd number (1,3,5,7,9, ...) 
float* linear2_window(int window_size){

	int val = (window_size * window_size) + window_size - 2 ;
	float* window = new float[window_size*window_size];
	for (int i = 0; i < window_size*window_size ; i++){
	   window[i] = 1.0 / val;
	}
	window[(window_size*window_size)/2] = (window_size-1) / (val*1.0); 
	 
	return window;
}


// This function generates the coeeficient for general low pass filter. 
// Center pixel with concentrated weight and others with 1 
// @params (x,y) window is the size of window
// @return 1D array containing the coefficients
// PRE--- window_size can only be an odd number (1,3,5,7,9, ...)
float* linear3_window(int b){
	
	int val = (b+2)*(b+2);
	float* window = new float[9];
	for (int i = 0; i < 9;  i++){
	
	   if (i % 2 == 1){
	       window[i] = b / (val * 1.0);
	   }
	   else if(i == 4){
	       window[i] = pow(b,2.0) / (val*1.0);
	   }	
	   else {
	       window[i] = 1 / (val*1.0);
	   }
	} 
	return window;
}


// This function performs linear filterning
// @params 2D array of image 
//         number of rows Size1
//         number of columns Size
//         window_size is the size of window
// 	   BytesPerPixel
// @return 2D array containing neighboring index
// PRE--- window can only be an odd number (1,3,5,7,9, ...)
unsigned char*** filter(unsigned char*** arr2D, int Size1, int Size, int window_size, float * window, int BytesPerPixel){
	
	int offset = window_size / 2;
	int **ptr;
 	float val;
	 
	unsigned char ***arr = new unsigned char**[Size1];
	for(int i = 0; i< Size1; i++){
	   arr[i] = new unsigned char*[Size];
   	   for(int j = 0; j < Size; j++){
	      arr[i][j] = new unsigned char[BytesPerPixel];
           }
        }

	for (int k = 0; k < BytesPerPixel; k++){
	   for(int i = 0; i < Size1;  i++){
	      for (int j = 0; j < Size; j++){
	       
	          val = 0;
	          ptr = gen_index(i, j, Size1, Size, window_size);
	          for (int l = 0; l < window_size*window_size ; l++){
		  
		     val = val + (int(arr2D[ptr[l][0]][ptr[l][1]][k])* window[l]); 
		     
	          }
		  val = round(val);
	          if (int(val) > 255){
		     val = 255;
	          }
	          arr[i][j][k] = char(int(val));
	      }
	   }	
	}
	return arr;
}


// This function finds the median of an array
// @params 1D array of numbers
//         size of array N
// @return the median
int median (int* arr, int N){

	int r = (N/2) + 1;
	int m = 0;
	int ind, x;
	while (m < r){
	    x  = 2000;
	    for (int j = 0; j < N; j++){
		if (x > arr[j]){
		   x = arr[j];
		   ind = j;
		}
		
	    }
	    arr[ind] = 2000;
	    m = m + 1;	
	}

	return x;
} 


// This function performs median filterning
// @params 2D array of image 
//         number of rows Size1
//         number of columns Size
//         window_size is the size of window
// 	   BytesPerPixel
//         Control
// @return 2D array containing neighboring index
// PRE-- window can only be an odd number (1,3,5,7,9, ...) 
//    -- Control == 0 for square window else T-shaped window
unsigned char*** median_filter(unsigned char*** arr2D, int Size1, int Size, int window_size, int BytesPerPixel, int control){


	int offset = window_size / 2;
	int **ptr;
 	int *val = new int [window_size*window_size];
	 
	unsigned char ***arr = new unsigned char**[Size1];
	for(int i = 0; i< Size1; i++){
	   arr[i] = new unsigned char*[Size];
   	   for(int j = 0; j < Size; j++){
	      arr[i][j] = new unsigned char[BytesPerPixel];
           }
        }

	for (int k = 0; k < BytesPerPixel; k++){
	   for(int i = 0; i < Size1;  i++){
	      for (int j = 0; j < Size; j++){
		
		if (control == 0){
	          ptr = gen_index(i, j, Size1, Size, window_size);
	          for (int l = 0; l < window_size*window_size ; l++){
		     val[l] = int(arr2D[ptr[l][0]][ptr[l][1]][k]);    
	          }
			
	          arr[i][j][k] = median(val, window_size*window_size);
		}
		else{
		  ptr = gen_index1(i, j, Size1, Size, window_size);
	          for (int l = 0; l < (2*window_size) - 1 ; l++){
		     val[l] = int(arr2D[ptr[l][0]][ptr[l][1]][k]);    
	          }
	          arr[i][j][k] = median(val, (2*window_size)-1);	
		} 

	      }
	   }	
	}
	return arr;
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


// Main
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
	
	unsigned char ***noiseless = new unsigned char**[Size1];
	unsigned char ***noisy = new unsigned char**[Size1];
	
	for(int i = 0; i< Size1; i++){
   	   
	   noiseless[i] = new unsigned char*[Size];
	   noisy[i] = new unsigned char*[Size];

   	   for(int j = 0; j < Size; j++){
       	      noiseless[i][j] = new unsigned char[BytesPerPixel];
	      noisy[i][j] = new unsigned char[BytesPerPixel];

	      for(int k = 0; k < BytesPerPixel; k++){
           	  noiseless[i][j][k] = Imagedata[i][j][k];
		  noisy[i][j][k] = Imagedata1[i][j][k];
              }
           }
        }	
	
	// Compute channel PSNR
	cout << "PSNR NOISY IMAGE " <<  endl;
	compute_psnr(noisy, noiseless, Size1, Size,BytesPerPixel);
	cout << endl;

	// Median Filtering + Gaussian + Linear
	int window_size = 7;
	unsigned char*** n1 = median_filter(noisy, Size1, Size, window_size, BytesPerPixel, 0);

	int b = 2;
	unsigned char*** noisy1 = filter(n1, Size1, Size, 3, linear3_window(b), BytesPerPixel);

	//window_size = 3;
	//unsigned char*** noisy1 = filter(n2, Size1, Size, window_size, linear1_window(window_size), BytesPerPixel);
	
	compute_psnr(noisy1, noiseless, Size1, Size,BytesPerPixel);

	for(int i = 0; i< Size1; i++){
   	   for(int j = 0; j < Size; j++){
       	      for(int k = 0; k < BytesPerPixel; k++){
           	  
		  Imagedata1[i][j][k] = noisy1[i][j][k];

              }
           }
        }
	
	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata1, sizeof(unsigned char), Size1*Size*BytesPerPixel, file);
	fclose(file);

	return 0;
}


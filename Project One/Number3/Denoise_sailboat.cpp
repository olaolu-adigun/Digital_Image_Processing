// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//		RAW format. You will need to make corresponding changes to
//		accommodate images of different sizes and/or types

//To use this program for another image, remove the "sailboat_noisy.raw"   
// under the main function and replace it with the desired noise image


#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <typeinfo>
#include <math.h>
#include <fstream>

using namespace std;

//COMPUTE CUMMULATIVE FREQUENCY
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

// MINIMUM
int min(int x, int y){

	if (x < y){
	   return x;
	}
	return y;
}

//MMAXIMUM
int max(int x, int y){

	if (x > y){
	   return x;
	}
	return y;
}

// COMPUTE PSNR
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
	cout << "PSNR = " << p << endl;
}

// GAUSS PDF
float gauss_kernel(float x , float mu, float std_dev){

	float a = pow((x - mu), 2.0);
	float b = exp((-1*a)/(2*pow(std_dev, 2.0)));
	return b;
}


// EUCLIDEAN DISTANCE
float euc_dist(int x1, int y1, int x2, int y2){

	return sqrt(pow((x1-x2),2.0) + pow((y1-y2),2.0) );
}


// WINDOW
int nl_mean_window(unsigned char*** arr, int Size1, int Size, int sigma_s, int sigma_c, int x, int y, int z){
		
	double u,v,w;
	double p = 0;
	double sum = 0;
	int offset = 10;
	for (int i = max(0, x-offset) ; i < min( x+offset, Size1);  i++){
	   for (int j = max(0, y-offset) ; j < min(y+offset, Size); j++){
		
		u = euc_dist(x, y, i, j);
		v = gauss_kernel(u, 0, sigma_s) * gauss_kernel(int(arr[i][j][z]), int(arr[x][y][z]), sigma_c);
		p = p + v;	

		sum = sum + v*int(arr[i][j][z]);
	   }  
	} 
	return int(round(sum/p));
}

// FILTERING
unsigned char*** nlm_filter(unsigned char*** arr3D, int Size1, int Size, int BytesPerPixel, float sigma_s, float sigma_c){
	
 	int val = 0;
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
	       
	          val = nl_mean_window(arr3D, Size1, Size, sigma_s, sigma_c, i, j, k);
		  val = round(val);
	          if (int(val) > 255){
		     val = 255;
	          }
	          arr[i][j][k] = char(int(val));
	      }
	    //cout << i << " ";
	   }	
	}
	cout << endl;
	return arr;
}


// SAVE DISTRIBUTION OF IMAGE FILE TO A FILE
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
	file = fopen("sailboat_noisy.raw","rb");
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

	// Compute channel PSNR
	cout << "PSNR NON_LOCAL MEAN IMAGE " <<  endl;
	unsigned char ***no_noise = nlm_filter(noisy, Size1, Size, BytesPerPixel, 10.0, 50.0);
	compute_psnr(no_noise, noiseless, Size1, Size,BytesPerPixel);
	cout << endl;
	
	for(int i = 0; i< Size1; i++){
   	   for(int j = 0; j < Size; j++){
       	      for(int k = 0; k < BytesPerPixel; k++){
           	  
		  Imagedata1[i][j][k] = no_noise[i][j][k];
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


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


// Apply bilinear interpolation to find the pixel value 
//   of fractional pixel location on the image.
// @params pixel location (x,y), image array (arr3D), and channel z
// @return pixel value after the interpolation

int bilinear_interpolate(float x, float y, int z, unsigned char ***arr3D){

	int val;
	float F, delX, delY;

	// Compute the fractional difference
	delX = x - int(x);
	delY = y - int(y);

	int x0 = int(x);
	int y0 = int(y);

	// Compute the pixel value
	F = (1-delX)*(1-delY)* int(arr3D[x0 +1][y0 +1][z]);
	F = F + (delX * (1-delY) * int(arr3D[x0 +2][y0 +1][z]));
	F = F + ((1-delX) * delY * int(arr3D[x0 +1][y0 +2][z]));
	F = F + (delX * delY * int(arr3D[x0+2][y0 +2][z]));

	if (F > 255.0){
	   F = 255.0;
	}

	return int(round(F));

}

// Determine if there is a white pixel on any 
//   surrounding side of a pixel location.
// @params image array (piece), and pixel location (i,j)
// @return int value specifying the number sides with white pixel

int test_neighbors(unsigned char ***piece, int i, int j){
	int val = 0;
	if(int(piece[i-1][j][0]) != 255 && int(piece[i-1][j][1]) != 255 && int(piece[i-1][j][2]) != 255){
	   val = val + 1;
	}
	if(int(piece[i+1][j][0]) != 255 && int(piece[i+1][j][1]) != 255 && int(piece[i+1][j][2]) != 255){
	   val = val + 1;
	}
	if(int(piece[i][j-1][0]) != 255 && int(piece[i][j-1][1]) != 255 && int(piece[i][j-1][2]) != 255){
	   val = val + 1;
	}
	if(int(piece[i][j+1][0]) != 255 && int(piece[i][j+1][1]) != 255 && int(piece[i][j+1][2]) != 255){
	   val = val + 1;
	}
	return val;

}

// To translate an image piece
// @params image array (piece), translation (xo, yo), dimension of the 
//     the image (height and width), number of bytes per pixel
// @return the image array after the translation
unsigned char*** fill_up(unsigned char ***piece, int xo, int yo, int height, int width, int BytesPerPixel){

	// Trim the Edges
	for (int i = xo-5; i < xo + height + 5 ; i++){
	for (int j = yo-5; j < yo + width + 5; j++){
	    if (i < xo || i > xo + height -1 || j < yo || j > yo + width -1){

		for (int k = 0; k < BytesPerPixel; k++){
		    piece[i][j][k] = 255;
		}		 
	    }
	}
	}
	
	
	//Move the image on the image array
	for (int i = xo; i < xo + height; i++){
 	for (int j = yo; j < yo + width; j++){
	    int val = test_neighbors(piece, i, j);
	    int y;
	    if (val == 4){
		for (int k = 0; k < BytesPerPixel; k++){
	    	   y = int(piece[i-1][j][k]) + int(piece[i+1][j][k]) + int(piece[i][j-1][k]) + int(piece[i][j+1][k]) ;
		   piece[i][j][k] = char(int(round((y*1.0)/4.0)));
		}
	     }
	}
	}

	// Adjust the bondary
	for (int j = yo + 1; j < yo + width -1; j++){
            for (int k = 0; k < BytesPerPixel; k++){

		piece[xo+1][j][k] = piece[xo+2][j][k];
		piece[xo][j][k] = piece[xo+1][j][k];
		
		piece[xo+height-2][j][k] = piece[xo+height-3][j][k];
		piece[xo+height-1][j][k] = piece[xo+height-2][j][k];
		
	    }
	}

	for (int i = xo ; i < xo + height; i++){
            for (int k = 0; k < BytesPerPixel; k++){

		piece[i][yo+1][k] = piece[i][yo+2][k];
		piece[i][yo][k] = piece[i][yo+1][k];

		piece[i][yo+width-2][k] = piece[i][yo+width-3][k];
		piece[i][yo+width-1][k] = piece[i][yo+width-2][k];
	    }
	}
	return piece;
}


// Compute the euclidean disatnce between two points on an image board
// @params the 2 points (x0, y0) and (x1, y1)
// @return the distance between the points
float euclidean_dist(float x0, float y0, float x1, float y1){

	 // Compute the distance
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

// Find the corners of an image piece
// @params image array (arr3D), search starting point (start1, start),
//    search stoping point (stop1, stop), and number of bytes per pixel.
// @return prints the location on the screen
void find_corners(unsigned char ***arr3D, int start1, int start, int stop1, int stop, int BytesPerPixel){

	int  val;				
	// Search for left corner	
	for (int i = start1; i < stop1 - 1; i++){
	   for (int j = start ; j < stop -1; j++){
	       if (int(arr3D[i][j][0]) != 255 || int(arr3D[i][j][1]) != 255 || int(arr3D[i][j][2])!= 255){
		  cout << "[x = " << i << ", y = " << j << "]" << endl;
		  goto Loop;		
	       }
	   } 
	}
	// Search for left corner
  Loop: for (int i = stop1 - 1; i > start1; i--){
            for (int j = start; j < stop -1; j++){
	       if (int(arr3D[i][j][0]) != 255 || int(arr3D[i][j][1]) != 255 || int(arr3D[i][j][2])!= 255){	
	          cout << "[x = " << i << ", y = " << j << "]" << endl;
	          goto Loop1;		
	       }
	   } 
	}
	// Search for right corner
  Loop1: for (int j = start; j < stop -1; j++){
	     for (int i = start1; i < stop1; i++){
	       if ( int(arr3D[i][j][0]) != 255 || int(arr3D[i][j][1]) != 255 || int(arr3D[i][j][2])!= 255){		
		  cout << "[x = " << i << ", y = " << j << "]" << endl;
		  goto Loop2;		
	       }
	    } 
	 }
	// Search for right corner
  Loop2: for (int j = stop - 1; j > start; j--){
	     for (int i = start1; i < stop1; i++){
	        if ( int(arr3D[i][j][0]) != 255 || int(arr3D[i][j][1]) != 255 || int(arr3D[i][j][2])!= 255 ){		
		    cout << "[x = " << i << ", y = " << j << "]" << endl;
		    cout << endl;
		    return;		
	       }
	    } 
	 }
}

// Compute the an interger value for the surrounding pixels
// @params image array (arr), and pixel index (i,j)
// @return the integer value
int compute_value(unsigned char*** arr, int i, int j){

	int val = 0;

	// Compute the value of the neighboring bytes converted to an integer
	if (int(arr[i][j+1][0]) == 255){
		val = val + 1;
	} 
	if (int(arr[i+1][j+1][0]) == 255){
		val = val + 2;
	} 
	if (int(arr[i+1][j][0]) == 255){
		val = val + 4;
	} 
	if (int(arr[i+1][j-1][0]) == 255){
		val = val + 8;	
	} 
	if (int(arr[i][j-1][0]) == 255){
		val = val + 16;
	} 
	if (int(arr[i-1][j-1][0]) == 255){
		val = val + 32;
	} 
	if (int(arr[i-1][j][0]) == 255){
		val = val + 64;
	} 
	if (int(arr[i-1][j+1][0]) == 255){
		val = val + 128;
	}
	
	return val;
}

// Find hite holes and report the coordinates of the corners
// @params image arr (arr), height of image (Size1), 
//         width of image (Size), number of bytes per pixel.
// @return Display the output on the screen.
void white_corners(unsigned char*** arr, int Size1, int Size, int BytesPerPixel){
	
	int val;
	// Determine the left top corner
	for (int i = 1; i < Size1-1 ; i++){
	    for (int j = 1; j < Size-1 ; j++){
	        if (int(arr[i][j][0]) == 255 && int(arr[i][j][1]) == 255 && int(arr[i][j][2]) == 255){
		if (int(arr[i][j+1][0]) == 255 && int(arr[i][j+1][1]) == 255 && int(arr[i][j+1][2]) == 255){
			cout << "LEFT TOP is i: " << i << " j: " << j << endl;
			goto Loop1;
		} 
		}
	    }
	}

	// Determine the left down corner
Loop1:  for (int i = Size - 1; i > 1 ; i--){
	    for (int j = 1; j < Size-1 ; j++){
	        if (int(arr[i][j][0]) == 255 && int(arr[i][j][1]) == 255 && int(arr[i][j][2]) == 255){
		if (int(arr[i][j+1][0]) == 255 && int(arr[i][j+1][1]) == 255 && int(arr[i][j+1][2]) == 255){
			cout << "LEFT DOWN is i: " << i << " j: " << j << endl;
			goto Loop2;
		} 
		}
	    }
	}

	// Determine the right down corner
Loop2:  for (int i = Size - 1; i > 1 ; i--){
	    for (int j = Size - 1; j > 1 ; j--){
	        if (int(arr[i][j][0]) == 255 && int(arr[i][j][1]) == 255 && int(arr[i][j][2]) == 255){
		if (int(arr[i][j-1][0]) == 255 && int(arr[i][j-1][1]) == 255 && int(arr[i][j-1][2]) == 255){
			cout << "RIGHT DOWN is i: " << i << " j: " << j << endl;
			goto Loop3;
		} 
		}
	    }
	}

	// Determine the right top corner
Loop3:  for (int i = 1; i < Size1-1 ; i++){
	    for (int j = Size-1; j > 1 ; j--){
	        if (int(arr[i][j][0]) == 255 && int(arr[i][j][1]) == 255 && int(arr[i][j][2]) == 255){
		if (int(arr[i][j-1][0]) == 255 && int(arr[i][j-1][1]) == 255 && int(arr[i][j-1][2]) == 255){
			cout << "RIGHT TOP is i: " << i << " j: " << j << endl;
			return;
		} 
		}
	    }
	}

}


// This is the main fucntion for puzzle matching
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

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size1*Size*BytesPerPixel, file);
	fclose(file);

	unsigned char Image1[512][512][3];
	unsigned char Image2[512][512][3];
	
	// Hillary's Image
	file = fopen("Hillary.raw", "rb");
	fread(Image1, sizeof(unsigned char), 512*512*3, file);
	fclose(file);

	// Trump's Image	
	file = fopen("Trump.raw", "rb");
	fread(Image2, sizeof(unsigned char), 512*512*3, file);
	fclose(file);

	////////////////// INSERT YOUR PROCESSING CODE HERE ///////////////////

	cout << "******************START*******************" << endl;

	// Initialize the pointers to the image arrays
	unsigned char ***piece = new unsigned char**[Size1];
	unsigned char ***arr3D = new unsigned char**[Size1];
	unsigned char ***image1 = new unsigned char**[Size1];
	unsigned char ***image2 = new unsigned char**[Size1];

	for(int i = 0; i < Size1; i++){

   	   piece[i] = new unsigned char*[Size];
	   arr3D[i] = new unsigned char*[Size];
	   image1[i] = new unsigned char*[Size];
	   image2[i] = new unsigned char*[Size];

   	   for(int j = 0; j < Size ; j++){

       	      piece[i][j] = new unsigned char[BytesPerPixel];
	      arr3D[i][j] = new unsigned char[BytesPerPixel];
	      image1[i][j] = new unsigned char[BytesPerPixel];
	      image2[i][j] = new unsigned char[BytesPerPixel];

               for(int k = 0; k < BytesPerPixel; k++){ 

		  arr3D[i][j][k] = Imagedata[i][j][k];
		  piece[i][j][k] = char(255);
		  image1[i][j][k] = Image1[i][j][k];
		  image2[i][j][k] = Image2[i][j][k];
               }
            }
        }

	///// HILLARY's PUZZLE

	// Find the corners of hole in Hillary's image
	cout << "The coordinate of hole corners in Hillary's image" << endl;
	white_corners(image1, Size1, Size,3);
	cout << endl;

	// Find the corners of the piece for Hillary's image
	cout << "The corners of Hillary's image piece are: " << endl;
	find_corners(arr3D, 0,0, 250, 250,BytesPerPixel);
	
	double delx, dely, theta;
	int x, y;
	int *arr;

	// Rotate the piece to a flat position
	for (int j = 55; j < 245; j++){ 
	    for (int i = 55; i < 245; i++){
	   
	        theta = asin(39.0/150.0);

	        delx = (cos(theta)*(i-57)) - (sin(theta)*(j-96));
	        dely = (sin(theta)*(i-57)) + (cos(theta)*(j-96));

 	        x = int(round(delx)) ;
	        y = int(round(dely)) ;

	        piece[50+x][50+y][0] =  Imagedata[i][j][0];
	        piece[50+x][50+y][1] =  Imagedata[i][j][1];
	        piece[50+x][50+y][2] =  Imagedata[i][j][2];	
	    }
	}
	
	unsigned char Imagedata1[Size1][Size][BytesPerPixel];
	piece = fill_up(piece, 50, 50, 150, 150,BytesPerPixel);

        ///// TRUMP's PUZZLE

	// Find the corners of hole in Trump's image
	cout << "The coordinate of hole corners in Hillary's image" << endl;
	white_corners(image2, Size1, Size,3);
	cout << endl;

	// Find the corners of the piece for Trump's image
	cout << "The corners of Trump's image piece are: " << endl;
	find_corners(arr3D,280,280,500,500, BytesPerPixel);

	for (int j = 280; j < 390; j++){ 
	for (int i = 280; i < 390; i++){
	   
	    theta = -1*atan(7.0/75.0) - asin(1);

	    delx = (cos(theta)*(i-371)) - (sin(theta)*(j-311));
	    dely = (sin(theta)*(i-371)) + (cos(theta)*(j-311));

 	    x = int(round(delx)) ;
	    y = int(round(dely)) ;
	    piece[125+x][300+y][0] =  Imagedata[i][j][0];
	    piece[125+x][300+y][1] =  Imagedata[i][j][1];
	    piece[125+x][300+y][2] =  Imagedata[i][j][2];	
	}
	}
	piece = fill_up(piece,122,300,75, 75,BytesPerPixel);
	

        ///// SCALE THE PIECES AND INSERT BACK TO THE CORRECT LOCATION 

	// Scaling the images to desired size
	float step = 149.0/100.0;
	float step1 = 74.0/101.0;

	int val;
	for(int i = 0; i < 100; i++){
	   for(int j = 0; j < 100; j++){
	      for(int k = 0; k < BytesPerPixel; k++){

		 // Hillary image insertion
	         val = bilinear_interpolate(50+(i*step), 50+(j*step), k, piece);
	         piece[300 + i][50 + j][k] = char(val);
		 Image1[135 + i][173+j][k] = char(val);

		 // Trump image insertion
	         val = bilinear_interpolate(122+(i*step1), 300+(j*step1), k, piece);
	         piece[300 + i][300 + j][k] = char(val);
		 Image2[236 + i][163+j][k] = char(val);
		 
	      }
	   }
	}	

	///// Write image data (filename specified by second argument) from image data matrix
	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Image1, sizeof(unsigned char), 512*512*BytesPerPixel, file);
	fclose(file);

	///// Write image data (filename specified by last argument) from image data matrix
	if (!(file=fopen(argv[6],"wb"))) {
		cout << "Cannot open file: " << argv[6] << endl;
		exit(1);
	}
	fwrite(Image2, sizeof(unsigned char), 512*512*BytesPerPixel, file);
	fclose(file);

	
	return 0;
}



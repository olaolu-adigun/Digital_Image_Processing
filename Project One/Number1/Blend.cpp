// This sample code reads in image data from a RAW image file and 
// writes it into another file

// NOTE:	The code assumes that the image is of size 256 x 256 and is in the
//			RAW format. You will need to make corresponding changes to
//			accommodate images of different sizes and/or types

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <typeinfo>
#include <string>
#include <math.h>

using namespace std;

// This function applies multipy blend to 2 pixels
// params pixel values
// Returns the output 
// PRE -- 0 <= a <= 255
//     -- 0 <= b <= 255
int multiply_blend(int a, int b){
 
	float f = (a/255.0) * (b/255.0);
	return round(f*255);
}


// This function applies screen blend to 2 pixels
// params pixel values
// Returns the output 
// PRE -- 0 <= a <= 255
//     -- 0 <= b <= 255   
int screen_blend(int a, int b){
   
	float a1 = a/255.0;
	float b1 = b/255.0;

	float f = 1.0 - ((1.0 -a1)*(1.0 - b1));
	return round(f*255);
}


// This function applies overlay blend to 2 pixels
// params pixel values
// Returns the output 
// PRE -- 0 <= a <= 255
//     -- 0 <= b <= 255   
int overlay_blend(int a, int b){
 
    float a1 = a/255.0;
    float b1 = b/255.0;
    float f;

    if (a1 < 0.5){
	f = 2*a1*b1;
    } 
    else {
	f = 1.0 - (2*(1.0-a1)*(1.0-b1));
    }
    
    return round(f*255); 
}


// This function applies hardlight blend to 2 pixels
// params pixel values
// Returns the output 
// PRE -- 0 <= a <= 255
//     -- 0 <= b <= 255  
int hardlight_blend(int a, int b){
 
    float a1 = a/255.0;
    float b1 = b/255.0;
    float f;

    if (b1 < 0.5){
	f = 2*a1*b1;
    } 
    else {
	f = 1.0 - (2*(1.0-a1)*(1.0-b1));
    }
    return round(f*255); 
}

// CREATE FILE_NAME FOR OUTPUT FILE
char* file_name(char *name1, string name2 ){

     int len1 = string(name1).length();
     int len2 = string(name2).length();

     char *ptr = new char [len1+len2];
     for(int i = 0; i < len1 + len2 ; i++){
	if (i < len2){
	   ptr[i] = name2[i];
	}
	else {
	   ptr[i] = name1[i-len2];
	}
     }
     return ptr; 
}

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
	unsigned char BottomLay[Size1][Size][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size1*Size*BytesPerPixel, file);
	fclose(file);
	
	// Read background image (filename specified by sixth argument) into image data matrix
        if (!(file=fopen(argv[6],"rb"))) {
		cout << "Cannot open file: " << argv[6] <<endl;
		exit(1);
	}
	fread(BottomLay, sizeof(unsigned char), Size1*Size*BytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	int val;
	
 	for(int i = 0; i < Size1; i++){
   	    for(int j = 0; j < Size; j++){
	        for(int k = 0; k < BytesPerPixel; k++){

		    //Blend the images together
		    val = hardlight_blend(int(BottomLay[i][j][k]), int(Imagedata[i][j][k]) );
		    Imagedata[i][j][k] = char(val);
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



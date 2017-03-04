// EE569   Homework Assignment #2
// Date:   Feb 7, 2017
// Name:   Olaoluwa Adigun
// ID:     2543-8895-69
// Email:  adigun@usc.edu   

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <typeinfo>
#include <string>
#include <math.h>

// Image class
//    stores the array of an image and performs some 
//    tasks on the image.

#ifndef HELPER_FXN2_H
#define HELPER_FXN2_H

#endif

using namespace std;

class Image
{
public:
	// Create an empty image without any image array.
	Image();

	// Create an image with a given image array and the correponding
	//     size of the image.
	Image(unsigned char*** arr, int Size1, int Size, int bytesPerPixel);
	
	// Performs shrinking of the image and returns the shrinked image.	
	unsigned char*** Shrinking ();		

	// Remove the holes from an image
	unsigned char*** Remove_holes(unsigned char*** arr1);

	// Test the shape to see if its a square or not.
	unsigned char*** Shape_Test(unsigned char*** arr1);

private:
	
	// Define the image array
	unsigned char*** arr2D;

	// Define the augmented image array
	unsigned char*** arr2D_aug; 
    
	// The size of the image
	int size1;
	int size;
	int BytesPerPixel;

	// This method augments the image array
	// returns the augmented image array
	unsigned char*** augment_arr();

	// This method converts the char array to bool array
	// returns the boolean array after conversion
	bool*** char_to_bool();
	
	// This method converts the bool array to char array
	// returns the char array  
	unsigned char*** bool_to_char(bool*** arr);

	// This method removes the padded rows and columns of the image
	// returns the padded char image array
	unsigned char*** remove_augment(unsigned char*** arr);
	
	// This method handles the conditional mask pattern of shrinking
	//    for a given pixel in an image array.
	// returns the marked value for the pixels
	bool mask_pattern_shrink_conditional(bool*** arr, int i, int j);
	
	// This method handles the unconditional mask pattern for shrinking
	//    and thinning for a given pixel in an image array.
	// returns the marked value for the pixels
	bool mask_pattern_shrink_and_thin_unconditional(bool*** arr, int i, int j);

	// Count the number of dots in an image array
	// returns the number of dots in a given image.
	int dot_count (bool*** arr);
	
	// Check if the shrinking is done or not
	// return true or false
	bool check_shrink(bool*** arr);

	// Check if the thinning is done or not
	// return true or false
	bool check_thin (bool*** arr_before, bool*** arr_after);	
};

//////////////////// Helper Functions/////////////////////////////////////////

// Compute a value based on the bit values of 
//   neighboring pixels.
// returns an intger value 
int compute_value(bool*** arr, int i, int j);

// Create a new boolean array
// return the boolean array
bool*** create_array(int Size1, int Size2);

// Crate an array of neighboring pixels
// returns a 1D array of bit values
bool* value_array(bool***arr, int i, int j);

// Determine if a shap centered at pixel (i,j) is a square 
//    or not.
// return true or false 
bool find_Square(unsigned char***, int i, int j);

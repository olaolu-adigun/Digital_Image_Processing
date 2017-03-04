// EE569   Homework Assignment #2
// Date:   Feb 7, 2017
// Name:   Olaoluwa Adigun
// ID:     2543-8895-69
// Email:  adigun@usc.edu    

#include <typeinfo>
#include "helper_fxn2.h"

using namespace std;

/* REPRESENTATION INVARIANT:
  -- The arr2D array holds pointers to image pixels.
  -- BytesPerPixel == 1
  -- For every arr[i][j][k], 0 <= i <= size1
  --    and 0 <= j <= size. 
*/


// Default Constructor
// Displays an instruction if no input is provided 
Image::Image() {
	cout << "Load the image matrix" << endl;  
}


// Constructor
// Creates the image array and defines the pointers to the pixels
// Specify the dimension of the image
Image::Image(unsigned char*** arr, int Size1, int Size, int bytesPerPixel)
{
	arr2D = arr;
	size1 = Size1;
	size  = Size;
	BytesPerPixel = bytesPerPixel;
}

// Shrink the image array stored in arr2D
// Update the stored image array shrinking
// @return shrinked image array
unsigned char*** Image:: Shrinking(){

	// Augment th image array
	arr2D_aug = augment_arr();

	// Convert the char pixel value to boolean
	bool*** arr2D_aug_bool = char_to_bool();
	
	// Declare local variables
	int iter = 0;
	int count, check;
	bool done, val, val1;
	bool*** M = create_array(size1+2, size+2);
	bool*** G = create_array(size1+2, size+2);
	bool*** F = arr2D_aug_bool;
	
	// Count the dots
	count = dot_count (arr2D_aug_bool);

	// Check if shrink is impossible
	done = check_shrink (arr2D_aug_bool);
	
	while(done != true){

	    iter = iter + 1;
	    // Apply conditional mask pattern for shrinking
     	    for (int i = 1; i < size1+1 ; i++){
	        for (int j = 1; j < size +1 ; j++){
		    // Test for hit or miss
		    if (F[i][j][0] == true){	
		    	M[i][j][0] = mask_pattern_shrink_conditional(F, i, j);
		     }else{
			M[i][j][0] = false;
		     }
	        }
	    }
	    
	    // Apply unconditional mask pattern
	    for (int i = 1; i < size1+1 ; i++){
	        for (int j = 1; j < size+1 ; j++){
		    val = M[i][j][0];
		    val1 = mask_pattern_shrink_and_thin_unconditional(M, i, j);
		    G[i][j][0] = (F[i][j][0] && (!val || val1));
		    
	        }
	    }
	    // Count the number of dots
	    check = dot_count (G);

	    done = check_shrink(G);
	    F = G;
	    
	    if (iter == 100){
		done = true;
	    }
	}
	
	// Convert the bool pixelvalues to char 
	arr2D_aug = bool_to_char(G);

	// Remove the augmented rows and columns
	arr2D = remove_augment(arr2D_aug);
	cout << ": "<< check << endl; 
	
	return arr2D;

}

// Remove the black holes in an image array
// @param image array arr1
// @return image after the hole removal
unsigned char*** Image:: Remove_holes(unsigned char*** arr1){

	// Use the identified dots from shrinking to remove the holes 
	// Convert to bool pixel value to char
	bool*** arr2D_aug_bool = char_to_bool();

	for (int i = 1; i < size1+1; i++){
	    for (int j = 1; j < size+1   ; j++){
		// Identify the holes 
	        if ((compute_value(arr2D_aug_bool, i, j) == 0)&&(arr2D_aug_bool[i][j][0] == true) ){
		    // Block the holes
		    for (int a = i-13; a < i+13; a++){
		        for (int b = j-13; b < j+13; b++){
		            arr1[a][b][0] = char(255);
		        }
		    }
	        }
	    }
	}
	// Update the object array
	arr2D = arr1;
	arr2D_aug = augment_arr();
	return arr2D;
}


// Test the shapes on an image array
// @param the image array arr1
// @return the image array after hole removal

unsigned char*** Image:: Shape_Test(unsigned char*** arr1){

	int numOfSquare = 0;
	int numOfCircle = 0;
	bool status;

	// Use the identified dots from shrinking to identify the shapes.
	// Convert to boolean pixel values to char. 
	bool*** arr2D_aug_bool = char_to_bool();

	for (int i = 1; i < size1+1; i++){
	    for (int j = 1; j < size+1   ; j++){
		// Identify the centre of the shapes
	        if ((compute_value(arr2D_aug_bool, i, j) == 0)&&(arr2D_aug_bool[i][j][0] == true) ){
		    // Test whether the shape is a square or circle
		    status = find_Square(arr1, i-1, j-1);
		    // Count the number of shapes
		    if (status == false){
			numOfCircle = numOfCircle + 1;
		    }
		    else{
			numOfSquare = numOfSquare + 1;
		    }
	        }
	    }
	}
	// Display the output
	cout << "NUMBER OF WHITE CIRCLES: " << numOfCircle << endl;
	cout << "NUMBER OF WHITE SQUARES: " << numOfSquare << endl;	

	return arr1;
}


// Augment the stored image array
// Stored the augmented image array
// @return augmented image array

unsigned char*** Image:: augment_arr(){
	
	unsigned char*** array = new unsigned char**[size1 + 2];
	// Initialize the pointers for the augmented image array
	for (int i = 0; i < size1 + 2; i++){
	    array[i] = new unsigned char*[size+2];
	    for (int j = 0; j < size + 2; j++){
	        array[i][j] = new unsigned char[BytesPerPixel];
	    }
	}

	//Copy the main image array
	for (int i = 1; i < size1 + 1; i++){
	    for (int j = 1; j < size + 1; j++){
	       for (int k = 0;  k < BytesPerPixel; k++){
		   array[i][j][k] = arr2D[i-1][j-1][k];
	       }
	    }
	}

	// Pad the rows of the image array
	for (int i = 1; i < size1+1; i++){
	    for (int k = 0; k < BytesPerPixel; k++){
	        array[i][0][k] = char(0); 
		array[i][size + 1][k] = char(0);
	    }
	}

	// Pad the columns of the image array
	for (int j = 0; j < size1 + 2; j++){
	    for (int k = 0; k < BytesPerPixel; k++){
	        array[0][j][k] = char(0); 
		array[size1+1][j][k] = char(0);
	    }
	}
	return array;
}


// Convert the image array of char value
//    to image array ith boolean values
// @return image array of boolean values. 

bool*** Image:: char_to_bool(){
	
	bool*** arrayB = new bool**[size+2];

	// Initialize the pointers to the boolean array 
	for (int i = 0; i < size1+2; i++){
	    arrayB[i] = new bool*[size+2];
	    for (int j = 0; j < size + 2; j++){
	        arrayB[i][j] = new bool[1];
		// Convert the char value to boolean values
		if (int(arr2D_aug[i][j][0]) < 100){
		    arrayB[i][j][0] = false;
		}else{
		    arrayB[i][j][0] = true;
		}
	     }
	}
	return arrayB;
}

// Convert the image array of boolean values 
// to image array of char values
// @return image array with char values

unsigned char*** Image:: bool_to_char(bool*** arr){

	unsigned char*** arrayC = new unsigned char**[size1+2];
	// Initialize the pointers to the char array
	for (int i = 0; i < size1+2; i++){
	    arrayC[i] = new unsigned char*[size+2];
	    for (int j = 0; j < size+2; j++){
	        arrayC[i][j] = new unsigned char[BytesPerPixel];
		for (int k = 0;  k < BytesPerPixel; k++){
		   // Convert the boolean values to char value
		   if (arr[i][j][k] == true){
		      arrayC[i][j][k] = char(255);
	 	   }
		   else{
		     arrayC[i][j][k] = char(0);
		   }
	        }
	    }
	}
	return arrayC;
}

// Remove the extra rows and  columns of an augmented array
// @param the augmented array
// @return the array after removing padded rows and columns
unsigned char*** Image::remove_augment(unsigned char*** arr){

	unsigned char*** arrayC = new unsigned char**[size1];

	// Remove the pads added to the image array. 
	for (int i = 0; i < size1; i++){
	    arrayC[i] = new unsigned char*[size];
	    for (int j = 0; j < size; j++){
		arrayC[i][j] = new unsigned char[1];
		arrayC[i][j][0] = arr[i+1][j+1][0];
	    }
	}
	return arrayC;
}


// Apply the conditional mask pattern to a pixel 
//    for shrinking
// @param the image array, index for the pixel (i, j)
// @return true if there is a hit and a false for a miss.
bool Image:: mask_pattern_shrink_conditional(bool*** arr, int i, int j){

	int Bytesnum = BytesPerPixel;
	int val = compute_value(arr,i,j);

	// Define the conditional mask pattern for shrinking. 
	// Bond 1 
	if ((val == 128) || (val == 32) || (val == 8) || (val == 2)){
	    return true;
	} 
	// Bond 2 
	if ((val == 1) || (val == 64) || (val == 16) || (val == 4)){
	    return true;
	}
	// Bond 3 
	if ((val == 129) || (val == 192) || (val == 96) || (val == 48) || (val == 24) || (val == 12) || (val == 6) || (val == 3)){
	    return true;
	}
	// Bond 4 
	if ((val == 131) || (val == 224) || (val == 56) || (val == 14)){
	    return true;
	}
	// Bond 5 
	if ((val == 97) || (val == 67) || (val == 208) || (val == 133) || (val == 193) || (val == 112) || (val == 28) || (val == 7)){
	    return true;
	}
	// Bond 6 
	if ((val==99)||(val==216)||(val==225)||(val==195)||(val == 240)||(val == 120)||(val == 60)||(val == 30)||(val == 15)||(val == 135)){
	    return true;
	}
	// Bond 7 
	if ((val == 227)||(val == 248)||(val == 62)||(val == 143)){
	    return true;
	}
	// Bond 8
	if ((val == 199)|| (val == 241) || (val == 124) || (val == 31)){
	    return true;
	}

	// Bond 9
	if ((val == 231) || (val == 207) || (val == 249) || (val == 243) || (val == 252)||(val == 126)||(val == 63)||(val == 159)){
	    return true;
	}
	// Bond 10
	if ((val == 239)||(val == 251)||(val == 254)||(val == 191)){
	    return true;
	}
	return false;
}



// Apply the unconditional mask pattern to a pixel 
//    for shrinking and thinning
// @param the image array, index for the pixel (i, j)
// @return true if there is a hit and false otherwise
bool Image:: mask_pattern_shrink_and_thin_unconditional(bool*** arr, int i, int j){

	
	bool M = arr[i][j][0];
	int val = compute_value(arr,i,j);
	bool* arr1D =  value_array(arr, i, j);

	// Define the uncoditional mask pattern for shrinking and thinning
	//Spur
	if ((M == true) &&((val == 128)||(val == 32)|| (val == 4) || (val == 1))){
		return true;
	}
	// L Cluster
	if ((M == true) &&((val == 129)||(val == 192)||(val== 96)||(val ==48)||(val == 24)||(val == 12)||(val == 6)||(val == 3))){
		return true;
	}
	// 4-Connected Offset
	if ((M == true) &&((val == 208)||(val == 97)||(val == 67) || (val == 133))){
		return true;
	}
	// Spur Corner Cluster
	if ((M == true) &&((val == 137)||(val == 200)||(val == 201))){	
		return true;
	}
	if ((M == true) &&((val == 50)||(val == 98)|(val == 114))){
		return true;
	}
	if ((M == true) &&((val == 140) ||(val == 152)||(val == 156))){
		return true;
	}
	if ((M == true) &&((val == 35)||(val == 38)||(val == 39))){
		return true;
	}
	// Corner Cluster
	if ((M == true) && (arr1D[4] == true) && (arr1D[5] == true) && (arr1D[6] == true)){
		return true;
	}
	// Tee Branch
	if ((M == true) && (arr1D[0] == true) && (arr1D[4] == true) && (arr1D[6] == true)){
	   if ((arr1D[1] == false) && (arr1D[2] == false) && (arr1D[7]== false)){
		return true;
	   }
	   if ((arr1D[2] == false) && (arr1D[3] == false) && (arr1D[5]== false)){
		return true;
	   }
	}
	if ((M == true) && (arr1D[0] == true) && (arr1D[2] == true) && (arr1D[4] == true)){
	   if ((arr1D[3] == false) && (arr1D[5] == false) && (arr1D[6]== false)){
		return true;
	   }
	   if ((arr1D[1] == false) && (arr1D[6] == false) && (arr1D[7]== false)){
		return true;
	   }
	}
	if ((M == true) && (arr1D[2] == true) && (arr1D[4] == true) && (arr1D[6] == true)){
	   if ((arr1D[0] == false) && (arr1D[1] == false) && (arr1D[3]== false)){
		return true;
	   }
	   if ((arr1D[0] == false) && (arr1D[5] == false) && (arr1D[7]== false)){
		return true;
	   }
	}
	if ((M == true) && (arr1D[0] == true) && (arr1D[2] == true) && (arr1D[6] == true)){
	   if ((arr1D[4] == false) && (arr1D[5] == false) && (arr1D[7]== false)){
		return true;
	   }
	   if ((arr1D[1] == false) && (arr1D[3] == false) && (arr1D[4]== false)){
		return true;
	   }
	}
	// Vee Branch
	if ((M == true) && (arr1D[5] == true) && (arr1D[7] == true)){
	   if ((arr1D[1] || arr1D[2] || arr1D[3]) == true){
		return true;
	   }
	}
	if ((M == true) && (arr1D[3] == true) && (arr1D[5] == true)){
	   if ((arr1D[0] || arr1D[1] || arr1D[7]) == true){
		return true;
	   }
	}
	if ((M == true) && (arr1D[1] == true) && (arr1D[3] == true)){
	   if ((arr1D[5] || arr1D[6] || arr1D[7]) == true){
		return true;
	   }
	}
	if ((M == true) && (arr1D[1] == true) && (arr1D[7] == true)){
	   if ((arr1D[3] || arr1D[4] || arr1D[5]) == true){
		return true;
	   }
	}
	// Diagonal Branch
	if ((M == true) && (arr1D[0] == true) && (arr1D[3] == true)&& (arr1D[6] == true)){
	   if ((arr1D[1] || arr1D[5]) == true){
		return true;
	   }
	}
	if ((M == true) && (arr1D[1] == true) && (arr1D[4] == true)&& (arr1D[6] == true)){
	   if ((arr1D[3] || arr1D[7]) == true){
		return true;
	   }
	}
	if ((M == true) && (arr1D[2] == true) && (arr1D[4] == true)&& (arr1D[7] == true)){
	   if ((arr1D[1] || arr1D[5]) == true){
		return true;
	   }
	}
	if ((M == true) && (arr1D[5] == true) && (arr1D[0] == true)&& (arr1D[2] == true)){
	   if ((arr1D[3] || arr1D[7]) == true){
		return true;
	   }
	}
	return false;
}


// Check if the shrinking is done
// @param the image array
// @return true if shrinking is done and false otherwise
bool Image:: check_shrink(bool*** arr){

	int val;
	// Check if the shrinking is complete
	for (int i = 1; i < size1 + 1; i++){
	    for (int j = 1; j < size + 1; j++){
	        if  (arr[i][j][0] == true){
		    val = compute_value(arr,i, j);
		    // Test all the dots
		    if (val != 0){
		        return false;
		    }	
	        }
	    }
	}
	return true;
}


// Count the number of isolated dots in an image arr
// @param image array
// @return number of dots in the image
int Image:: dot_count (bool*** arr){

	int count = 0;
	int val;
	// Count the number of dots in the image 
	for (int i = 1; i < size1 + 1; i++){
	    for (int j = 1; j < size + 1; j++){
	        if  (arr[i][j][0] == true){
		    val = compute_value(arr,i, j);
		    if  (val == 0){
		        count  = count + 1;
		    }	
	        }
	    }
	}
	return count;
}

// Check if 2 image arrays are the same 
// @param the image arr_before, arr_after
// @return true if the arrays are the same and false otherwise
bool Image:: check_thin (bool*** arr_before, bool*** arr_after){
	// Check if the image array is unchanged.
	// Compare the two images array 
	for (int i = 1; i < size1 + 2; i++){
	    for (int j = 1; j < size + 2; j++){
		// Compare the image arrays
	        if (arr_before[i][j][0] != arr_after[i][j][0]){
		   return false;
		}
	    }
	}
	return true;
}


////////////////////////////////////////////////////////////////////////////
//////////////////////////HELPER FUNCTIONS//////////////////////////////////


// Extract the neigboring values of an image pixel
//    and write to a 1D array.
// @params the image array and the pixel location (i,j)
// @ return the boolean array

bool* value_array(bool***arr, int i, int j){

	bool* arr1D = new bool [8];

	// Store the neighboring marks in an array	
	arr1D[0] = arr[i][j+1][0];
	arr1D[1] = arr[i+1][j+1][0];
	arr1D[2] = arr[i+1][j][0] ;
	arr1D[3] = arr[i+1][j-1][0] ; 
	arr1D[4] = arr[i][j-1][0] ; 
	arr1D[5] = arr[i-1][j-1][0] ;
	arr1D[6] = arr[i-1][j][0];
	arr1D[7] = arr[i-1][j+1][0] ;

	return arr1D;
}


// Extract the neigboring values of boolean imag representation
// Convert to a single integer using base 2.
// @params the image array arr, and the pixel location (i,j) 
// @return the integer value

int compute_value(bool*** arr, int i, int j){

	// Convert the neighboring mark values to an integer
	int val = 0;

	if (arr[i][j+1][0] == true){
		val = val + 1;
	} 
	if (arr[i+1][j+1][0] == true){
		val = val + 2;
	} 
	if (arr[i+1][j][0] == true){
		val = val + 4;
	} 
	if (arr[i+1][j-1][0] == true){
		val = val + 8;	
	} 
	if (arr[i][j-1][0] == true){
		val = val + 16;
	} 
	if (arr[i-1][j-1][0] == true){
		val = val + 32;
	} 
	if (arr[i-1][j][0] == true){
		val = val + 64;
	} 
	if (arr[i-1][j+1][0] == true){
		val = val + 128;
	}
	return val;
}


// Create an empty 2D array with boolean values
// @parmas the number of rows and columns of the desired array
// @return the created array

bool*** create_array(int row, int col){

	bool*** arr = new bool**[row];
	// Create a new array
	for (int i = 0; i < row; i++){
	    arr[i] = new bool*[col];
	    for (int j = 0; j < col;  j++){
	        arr[i][j] = new bool[1];
	        arr[i][j][0] = false;
	    }
	}
	return arr;
}


// Check if the shape centered at a pixel vale is a square or not
// @params image array arr, and pixel location (i,j)
// @return true if the shape is square and false otherwise.

bool find_Square(unsigned char*** arr2D, int i, int j){

	bool done = false;
	int iter = 0;

	// Test the shape of the image for square  
	while (done != true){
	    // Determine the length
	    if (int(arr2D[i][j][0]) == 255){
		j = j+1;
		iter = iter + 1;
	    }
	    else{
		done = true;
	    }
	}
	// Test if the width equals the length
	for (int a = i; a < i+iter-1; a++){
	    if (int(arr2D[a][j-1][0]) != 255){
		return false;
	    }
	}
	return true;
}

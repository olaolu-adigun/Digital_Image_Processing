1.Compiler and platform:
   
   I used C++ to implement the algorithms
   I worked on Linux with gedit 
   Image Viewer: http://rawpixels.net/

2. How to run and compile 
   Compile: Type g++ -o source_code.cpp object
   Run: Type ./object input.raw output.raw 3 (for gray level image, type 1)  

   NOTE: The instruction for compiling the codes varies depending on the question.
	 Listed below are the instructions to compile and run the code. 


*****************************************************************
*****************************************************************
**************  COMPILE AND RUN THE CODE         ****************
*****************************************************************
*****************************************************************

P2)  HISTOGRAM EQUALIZATION 

a) Transfer Function Based Histogram Equalization Method

  1). ** Compile -- g++ Transfer_Function_grayscale.cpp -o object
      ** Run -- ./object tulip_dark.raw output.raw 1 400 366

      ** Compile -- g++ Transfer_Function_grayscale.cpp -o object
      ** Run -- ./object tulip_bright.raw output.raw 1 400 366

      ** Compile -- g++ CDF_Approach_grayscale.cpp -o object
      ** Run -- ./object tulip_dark.raw output.raw 1 400 366

      ** Compile -- g++ CDF_Approach_grayscale.cpp -o object
      ** Run -- ./object tulip_bright.raw output.raw 1 400 366


  2). ** Compile -- g++ Transfer_Function_grayscale.cpp -o object
      ** Run -- ./object tulip_mix.raw output.raw 1 400 366

      ** Compile -- g++ CDF_Approach_grayscale.cpp -o object
      ** Run -- ./object tulip_mix.raw output.raw 1 400 366

      ** Compile -- g++ Digital_Implementation_grayscale.cpp -o object
      ** Run -- ./object tulip_dark.raw output.raw 1 400 366

      ** Compile -- g++ Digital_Implementation_grayscale.cpp -o object
      ** Run -- ./object tulip_bright.raw output.raw 1 400 366

      ** Compile -- g++ Digital_Implementation_grayscale.cpp -o object
      ** Run -- ./object tulip_mix.raw output.raw 1 400 366

(b) Histogram Equalization for Color Images.

      ** Compile -- g++ Transfer_Function_RGB.cpp -o object
      ** Run -- ./object bedroom.raw output.raw 1 940 400

      ** Compile -- g++ CDF_Approach_RGB.cpp -o object
      ** Run -- ./object bedroom.raw output.raw 1 940 400

      ** Compile -- g++ Transfer_Function_HSL.cpp -o object
      ** Run -- ./object bedroom.raw output.raw 1 940 400

      ** Compile -- g++ CDF_Approach_HSL.cpp -o object
      ** Run -- ./object bedroom.raw output.raw 1 940 400


c). Histogram Transform

      ** Compile -- g++ Gaussian_Transform_RGB.cpp -o object
      ** Run -- ./object forest_1.raw output.raw 1 960 600
      
      ** Compile -- g++ Gaussian_Transform_RGB.cpp -o object
      ** Run -- ./object forest_2.raw output.raw 1 550 413










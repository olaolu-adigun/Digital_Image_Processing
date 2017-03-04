# EE569   Homework Assignment #1
# Date:   Jan 20, 2017
# Name:   Olaoluwa Adigun
# ID:     2543-8895-69
# Email:  adigun@usc.edu


# Makefile for EE569 PA1
# --- make clean
# --- make prob2a
# --- make prob2b
# --- make prob2c
#        
# need to use make

CC = gcc
LN = gcc
All : prob2a prob2b prob2c

prob2a:
	@echo "****** Problem 2.a ******"
	@g++ Transfer_Function_grayscale.cpp -o TF_approach
	@g++ CDF_Approach_grayscale.cpp -o CDF_approach
	@g++ Digital_Implementation_grayscale.cpp -o DIGITAL_approach
	@echo "USAGE: ./prob1a [inputImageName] [outputImageName] 3 300 300"
	./TF_approach tulip_dark.raw tulip_dark_TF.raw 1 400 366
	./TF_approach tulip_bright.raw tulip_bright_TF.raw 1 400 366
	./TF_approach tulip_mix.raw tulip_mix_TF.raw 1 400 366
	./CDF_approach tulip_dark.raw tulip_dark_CDF.raw 1 400 366
	./CDF_approach tulip_bright.raw tulip_bright_CDF.raw 1 400 366
	./CDF_approach tulip_mix.raw tulip_mix_CDF.raw 1 400 366
	./DIGITAL_approach tulip_dark.raw tulip_dark_DIGITAL.raw 1 400 366
	./DIGITAL_approach tulip_bright.raw tulip_bright_DIGITAL.raw 1 400 366
	./DIGITAL_approach tulip_mix.raw tulip_mix_DIGITAL.raw 1 400 366

prob2b:
	@echo "****** Problem 2.b ******"
	@g++ Transfer_Function_RGB.cpp -o TF_rgb
	@g++ CDF_Approach_RGB.cpp -o CDF_rgb
	@g++ Transfer_Function_HSL.cpp -o TF_hsl
	@g++ CDF_Approach_HSL.cpp -o CDF_hsl
	@echo "USAGE: ./prob1b [inputName] [outputName] 3 500 500"
	./TF_rgb bedroom.raw output1.raw 1 940 400
	./CDF_rgb bedroom.raw output2.raw 1 940 400
	./TF_hsl bedroom.raw output3.raw 1 940 400
	./CDF_hsl bedroom.raw output4.raw 1 940 400


prob2c:
	@echo "****** Problem 2.c ******"
	@g++ Blend.cpp -o blend
	@echo "USAGE: ./prob1c [TopImageName] [BaseImageName] 3 890 890"
	./blend top_layer.raw blend.raw 3 890 890 bottom_layer.raw

 clean:
	rm -f *.o TF_approach CDF_approach DIGITAL_approach TF_hsl TF_rgb CDF_hsl CDF_rgb
	rm -f tulip_dark_TF.raw tulip_bright_TF* tulip_mix_TF* tulip_dark_CDF* tulip_bright_CDF* tulip_mix_CDF* tulip_dark_DIGITAL* tulip_bright_DIGITAL* tulip_mix_DIGITAL* Image_Before_Equalization* Image_After_Equalization* Transfer_Function.csv output1.raw output2.raw output3.raw output4.raw Light_Unequalized.csv Light_Equalized.csv

# EE569   Homework Assignment #2
# Date:   Feb 7, 2017
# Name:   Olaoluwa Adigun
# ID:     2543-8895-69
# Email:  adigun@usc.edu


# Makefile for EE569 PA2
# --- make clean
# --- make prob2a
# --- make prob2b
#        
# need to use make


CC = gcc
LN = gcc
All : prob2a prob2b

prob2a:
	@echo "****** Problem 2.a ******"
	@g++ -c prob2a.cpp
	@g++ -o prob2a prob2a.o
	@echo "USAGE: ./prob1a [inputImageName] [outputImageName] 1 512 512"
	@echo "OTHER OUTPUT: [BayerC2.raw] [BayerC4.raw] [BayerC8.raw][Bayer_A4.raw]"
	./prob2a man.raw ProposedMethod.raw 1 512 512

prob2b:
	@echo "****** Problem 2.b ******"
	@g++ -c prob2b_1.cpp
	@g++ -o prob2b_1 prob2b_1.o
	@g++ -c prob2b_2.cpp
	@g++ -o prob2b_2 prob2b_2.o
	@g++ -c prob2b_3.cpp
	@g++ -o prob2b_3 prob2b_3.o
	@echo "USAGE: ./prob1a [inputImageName] [outputImageName] 1 512 512"
	./prob2b_1 man.raw FloydStein.raw 1 512 512
	./prob2b_2 man.raw JJN.raw 1 512 512
	./prob2b_3 man.raw Stuki.raw 1 512 512

clean:
	rm -f *.o prob2a prob2b_1 prob2b_2 prob2b_3
	rm -f BayerC2.raw BayerC4.raw BayerC8.raw BayerA4.raw ProposedMethod.raw FloydStein.raw JJN.raw Stuki.raw




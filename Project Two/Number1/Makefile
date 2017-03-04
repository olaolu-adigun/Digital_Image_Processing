# EE569   Homework Assignment #2
# Date:   Feb 7, 2017
# Name:   Olaoluwa Adigun
# ID:     2543-8895-69
# Email:  adigun@usc.edu


# Makefile for EE569 PA2
# --- make clean
# --- make prob1a
# --- make prob1b
# --- make prob1c
#        
# need to use make

CC = gcc
LN = gcc
All : prob1a prob1b prob1c

	
prob1a:
	@echo "****** Problem 1.a ******"
	@g++ -c prob1a.cpp
	@g++ -o prob1a prob1a.o
	@echo "USAGE: ./prob1a [inputImageName] [outputImageName] 3 500 500"
	./prob1a cup1.raw cup1_warp.raw 3 500 500
	./prob1a cup2.raw cup2_warp.raw 3 500 500

prob1b:
	@echo "****** Problem 1.b ******"
	@g++ -c prob1b.cpp
	@g++ -o prob1b prob1b.o
	@echo "USAGE: ./prob1b [inputImageName] [outputImageName] 3 500 500 [outputImageName2]"
	./prob1b piece.raw HillaryFull.raw 3 500 500 TrumpFull.raw

prob1c:
	@echo "****** Problem 1.c ******"
	@g++ -c prob1c.cpp
	@g++ -o prob1c prob1c.o
	@echo "USAGE: ./prob1c [inputImageName] [outputImageName] 3 972 648"
	./prob1c field.raw trojanField.raw 3 972 648

clean:
	rm -f *.o prob1a prob1b prob1c
	rm -f cup1_warp.raw cup2_warp.raw HillaryFull.raw TrumpFull.raw trojanField.raw




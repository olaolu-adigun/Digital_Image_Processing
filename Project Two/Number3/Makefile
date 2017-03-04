# EE569   Homework Assignment #2
# Date:   Feb 7, 2017
# Name:   Olaoluwa Adigun
# ID:     2543-8895-69
# Email:  adigun@usc.edu


# Makefile for EE569 PA2
# --- make clean
# --- make prob3a
# --- make prob3b
# --- make prob3c
# --- make prob3d
#        
# need to use make

CC = gcc
LN = gcc
All : prob3a prob3b prob3c prob3d

prob3a:
	@echo "****** Problem 3.a ******"
	@g++ -c prob3a.cpp
	@g++ -c helper_fxn.cpp
	@g++ -o prob3a prob3a.o helper_fxn.o
	@echo "USAGE: ./prob1a [inputImageName] [outputImageName] 1 480 480"
	./prob3a squares.raw squares_shrink.raw 1 480 480

prob3b:
	@echo "****** Problem 3.b ******"
	@g++ -c prob3b.cpp
	@g++ -c helper_fxn.cpp
	@g++ -o prob3b prob3b.o helper_fxn.o
	@echo "USAGE: ./prob3b [inputImageName] [outputImageName] 1 480 480"
	./prob3b letterE.raw letterE_thin.raw 1 480 480

prob3c:
	@echo "****** Problem 3.c ******"
	@g++ -c prob3c.cpp
	@g++ -c helper_fxn.cpp
	@g++ -o prob3c prob3c.o helper_fxn.o
	@echo "USAGE: ./prob3c [inputImageName] [outputImageName] 1 480 480"
	./prob3c letterE.raw letterE_skeleton.raw 1 480 480


prob3d:
	@echo "****** Problem 3.d ******"
	@g++ -c prob3d.cpp
	@g++ -c helper_fxn2.cpp
	@g++ -o prob3d prob3d.o helper_fxn2.o
	@echo "USAGE: ./prob3d [inputImageName] [outputImageName] 1 480 480"
	./prob3d board.raw board_process.raw 1 480 480

clean:
	rm -f *.o prob3a prob3b prob3c prob3d
	rm -f squares_shrink.raw letterE_thin.raw letterE_skeleton.raw board_process.raw Without_Holes.raw Black_Hole_Spots.raw Image_Complement.raw 




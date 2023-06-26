#Makefile for “raytracer.cpp” C++11 application
#Created by Kiefer Aland 04/21/21

CXX = g++

raytracer : raytracer.o
	 $(CXX) raytracer.cpp -o raytracer
	 ./raytracer testAmbient.txt
	 open scene.ppm

clean:
	 rm raytracer raytracer.o scene.ppm

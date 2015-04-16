#!/usr/bin/env bash

mkdir -p bin # for silencing the next line, in case the folder does not exist
rm -r bin
mkdir -p build
touch build/touched # for silencing the next line, in case the folder was just created
rm -r build/*
cd build

echo "Building CEF3SimpleSample::Independente"
g++ -c ../CEF3SimpleSample/Independente/*.cpp \
	-I ../CEF3SimpleSample/Independente \
	-I ../CEF3SimpleSample/CEF/Linux \
	-I ../CEF3SimpleSample/CEF/Linux/include \
	`pkg-config --cflags --libs gtk+-2.0`

echo "Building CEF3SimpleSample::Linux"
g++ -c ../CEF3SimpleSample/Linux/*.cpp \
	-I ../CEF3SimpleSample/Independente \
	-I ../CEF3SimpleSample/CEF/Linux \
	-I ../CEF3SimpleSample/CEF/Linux/include \
	`pkg-config --cflags --libs gtk+-2.0`

echo "Linking..."
g++ *.o \
	-o CEF3SimpleSample \
	`pkg-config --cflags --libs gtk+-2.0` \
	-L ../CEF3SimpleSample/CEF/Linux/lib \
	-ldl \
	-lcef_dll_wrapper \
	-lcef \
	-Wl,-R. -Wl,-R/usr/lib

echo "Copying files around..."
rm *.o
mkdir bin
mv CEF3SimpleSample bin/
cp -r ../CEF3SimpleSample/CEF/Linux/Resources/* bin/
cp -r ../CEF3SimpleSample/CEF/Linux/lib/*.so bin/
cp -r ../CEF3SimpleSample/Independente/html bin/html
mv bin ..
cd ..
rm -r build


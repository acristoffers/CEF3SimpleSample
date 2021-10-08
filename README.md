Chromium Embedded Framework Simple Application
==============================================
A simple hello world application to get you started with CEF3.

It setups a browser, loads a local file and binds JS methods.

Lib folders
-----------

The lib folders containing CEF binaries were removed from the repository because
GitHub does not allow files bigger than 100MB, and some of them are. They are
available as a [Release in GitHub](https://github.com/acristoffers/CEF3SimpleSample/releases). 

Download and unpack in CEF3SimpleSample/CEF/Linux/, CEF3SimpleSample/CEF/Mac/ or
CEF3SimpleSample/CEF/Win/

Windows
-------
Requires VS 2019 and cmake.

From a Native VS 2019 x64 command prompt, Change directory to the root of the repository:

```
cd CEF3SimpleSample 
```
Download https://github.com/acristoffers/CEF3SimpleSample/releases/tag/90.6.5 and extract to CEF3SimpleSample\CEF3SimpleSample\CEF\Win\Lib as mentioned above. Then:
```
mkdir build
cd build
mkdir bin
cmake .. 
```
Download the cef standard distribution from: https://cef-builds.spotifycdn.com/cef_binary_90.6.5%2Bg7a604aa%2Bchromium-90.0.4430.93_windows64.tar.bz2, extract it and copy resource.pak, chrome_100_percent.pak and chrome_200_percent.pak from \Resources to build\bin
```
msbuild /p:Configuration=Release CEF3SimpleSample.sln
```

To run go to build\bin and execute
```
CEF3SimpleSample.exe
```
You should see a window with a label and a button that if presed would change the label from Hello to "Hello, world!". If not check if debug.log contains any useful info.

**If you need to use Visual Studio, open `.../build/ALL_BUILD.vcxproj` project that were generated using cmake, because compiling the `.../CEF3SimpleSample.sln` project throws an error.**

LICENSE
-------
(MIT)

Copyright (c) 2013 Álan Crístoffer

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

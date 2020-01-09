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

If using CMake, generate with `cmake -A x64 ..` and build with `msbuild
/p:Configuration=Release`. You will need to have Visual Studio installed and its
console.

LICENSE
-------
(MIT)

Copyright (c) 2013 Álan Crístoffer

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

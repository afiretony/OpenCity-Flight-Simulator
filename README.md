# 24780-Engineers-Republic

## Summary

## Getting Started
In case you haven't installed cmake on windows, you can use [this link](https://github.com/Kitware/CMake/releases/download/v3.22.0-rc2/cmake-3.22.0-rc2-windows-x86_64.msi
) to download and install cmake, make sure you selected "add to system PATH".

To clone this repo:
```bash
git clone --recursive https://github.com/afiretony/24780-Engineers-Republic.git
cd 24780-Engineers-Republic/Glitter
mkdir Build
cd Build
```

Now generate a project file or makefile for your Windows Visual Studio 2019.

```bash
# Microsoft Windows
cmake -G "Visual Studio 16 2019" ..
```

After build, sln file is generated at `/24780-Engineers-Republic/Glitter/Build/Glitter.sln`, you can open the solution and start the project.

### First run


## Documentation
Many people overlook how frustrating it is to install dependencies, especially in environments lacking package managers or administrative privileges. For beginners, just getting set up properly set up can be a huge challenge. Glitter is meant to help you overcome that roadblock.

Glitter provides the most basic windowing example. It is a starting point, and tries very hard not to enforce any sort of directory structure. Feel free to edit the include paths in `CMakeLists.txt`. Glitter bundles most of the dependencies needed to implement a basic rendering engine. This includes:

Functionality           | Library
----------------------- | ------------------------------------------
Mesh Loading            | [assimp](https://github.com/assimp/assimp)
Physics                 | [bullet](https://github.com/bulletphysics/bullet3)
OpenGL Function Loader  | [glad](https://github.com/Dav1dde/glad)
Windowing and Input     | [glfw](https://github.com/glfw/glfw)
OpenGL Mathematics      | [glm](https://github.com/g-truc/glm)
Texture Loading         | [stb](https://github.com/nothings/stb)

If you started the tutorials by installing [SDL](https://www.libsdl.org/), [GLEW](https://github.com/nigels-com/glew), or [SOIL](http://www.lonesock.net/soil.html), *stop*. The libraries bundled with Glitter supersede or are functional replacements for these libraries.

I have provided sample implementations of an intrusive tree mesh and shader class, if you're following along with the tutorials and need another reference point. These were used to generate the screenshot above, but will not compile out-of-the-box. I leave that exercise for the reader. :smiley:

## License
>The MIT License (MIT)

>Copyright (c) 2021 Carnegie Mellon University

>Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

>The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

>THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

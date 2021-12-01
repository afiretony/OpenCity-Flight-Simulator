<img width="200" img src="https://raw.githubusercontent.com/afiretony/24780-Engineers-Republic/main/team_logo.png" alt="team_logo"/>

# OpenCity Flight Simulator
Carnegie Mellon University - 24780B Engineering Computation - 21fall

TEAM: ENGINEERS REPUBLIC

## Summary
This repository is the final project of team Engineers Republic. Flight simulator is a trending topic in both the entertainment and professional area. In this
project, the group is planning to develop a software product based on the C++ and OpenGL to realize a customizable and lightweight flight simulator.

## Basic Function
### Entertainment
* Users can explore and enjoy a city landscape in the FPV / TPV view display.
* Users can experience controlling multiple kinds of aircrafts easily using a keyboard or joystick. 
* Ariel random obstructions avoidance.
  
### Professional
* Related developers can use this product as a virtual testing tool by importing the building models of a real region or aircraft parameters.
* AI engineers can adopt this product as their simulation or testing tool (i.e. path planning algorithm implementation).
* This product can even service some mapping or navigation projects. 

## Special Features
The aircraft has the function of automatic obstacle avoidance, and the city looks realistic with reasonable dimensions (or can be dimensions of real city buildings). Users can switch between different aircraft including UAVs and different helicopters. Users can switch between first-person and third-person point of views.
    
## Getting Started
In case you haven't installed cmake on windows, you can use [this link](https://github.com/Kitware/CMake/releases/download/v3.22.0-rc2/cmake-3.22.0-rc2-windows-x86_64.msi
) to download and install cmake, make sure that you added cmake to system PATH.

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

### [Important] First run
Main file is located at `24780-Engineers-Republic\Glitter\Glitter\Sources\main.cpp`, at first run you need to build all solutions(by default solution configuration), Visual Studio may pop-up permission error after build. Then you should change Solution Glitter properties from `All_BUILD` to `Glitter`.

![Screenshot](/figures/vs_setup.png)

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

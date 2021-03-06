# Overview
I-Cube 3D Engine (IcEng) is an open source, light weight OpenGL/C++ 3D graphics/VR Engine. It's designed with simple structure and interface to fit small cross platform 3D application. It can also serve as OpenGL example, or foundation code base that you can develop your own 3D engine upon.
The source or binary are provided with free BSD license, so you can use this engine on personal or commercial project with no restriction. See License section.
For more information check website http://www.simviu.com/dev.

# Features
* C++ 0x11 code, OpenGL 2.0 shader.
* Cross Platform ( Mac/Windows/Linux/iOS/Android ), project file provided.
* Auto memory and resource management with C++ 0x11 shared pointer.
* Model loading with Waveform .OBJ file. ( More format in future )
* Basic lighting, material, and fog.
* Render to texture.
* Abstract C++ wrapper class with OpenGL function hidden.
* VR rendering with lense distortion, for Google Cardboard, or VR device as PC extended monitor.

# Architecture
* The IcEng applications are wrapped in IcApp/IcWindow/IcScene structure. 
* There is only one IcApp instance(singleton). 
* An IcApp application can contain several IcWindow. 
* Each window has it's own OpenGL context. 
* Each window has one or more IcScene. 
* Each IcScene contain one IcCamera, and many IcObject.
* An IcObject has it's IcModel, loaded from OBJ file or created dynamically.
* IcModel are constructed by IcMesh, IcTexture and IcMaterial.

In most case your simple 3d application has one IcWindow and one IcScene. For iOS/Android mobile App, there is only one window.

# Document
More info check IcEng/doc/IcEng_Manual.pdf
(... under construction ... )

# Pre-Build Binary
Pre-Build binary are provided in :
https://github.com/simviu/IcEngBuild

# VR
VR demo can be found in project IcVRDemo, in all platforms.
On iOS/Android, the VR demo can be put into Google Cardboard or similar headset VR box.
For Occulus or other PC VR headset, the device need to support external monitor mode,
and the orientation of headset should feed in via their API, by calling 
IcWindow::onDeviceRot() function.

# License
Check BSD license :
https://en.wikipedia.org/wiki/BSD_licenses

# Dependencies
IcEng have a few dependencies, but all included in this repo.
You can update them with latest version by yourself.
* glm : GLM is a C++ mathematics library for graphics software, MIT license(Free). ( https://www.opengl.org/sdk/libs/GLM/ ) 
* Lodepng : LodePNG is a PNG image decoder and encoder. It's own license(Free). (http://lodev.org/lodepng/)
* FreeGlut Binary Lib: For windows only, provided as binary in IcEngBuild, X-Consortium license(Free).(http://freeglut.sourceforge.net/)
* glew Binary Lib: For windows only, BSD/MIT license(Free). (http://glew.sourceforge.net/)
* android OpenGL Surface View and JNI wrapper : derived from Android Open Source Project, OpenGL/JNI samples, Apache License.

Note: For windows dependencies (FreeGlut and Glew), you can download the binary winDepLib.zip 
from IcEngBuild([https://github.com/simviu/IcEngBuild]), unzip and put inside IcEng/proj/windows/.
Or you can download yourself, but re-orgnized the files in the same directory structures. 
This is for convinience of Visual Studio Project Setting.

# Contact
support@simviu.com

# ScreenShots
| ...                                    | ...                                    |
| ---------------------------------------|:--------------------------------------:|
| ![alt tag](doc/ScreenShots/1.png)      | ![alt tag](doc/ScreenShots/2.png)      |
| ![alt tag](doc/ScreenShots/3.png)      | ![alt tag](doc/ScreenShots/4.png)      | 
| ![alt tag](doc/ScreenShots/vr1.JPG)    | ![alt tag](doc/ScreenShots/vr2.JPG)    | 




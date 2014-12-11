godot-opencv-gpu-perspective
====================

This is a simple demonstration of using OpenCV to track the head of the user
from the web cam, and adjusting the camera's position and angle accordingly in
the 3d environment in Godot; giving a feeling of perspective to the user when
they move their head. You can find several examples of this concept on the
internet.

The problem with this concept is that detecting the position of the face in a
frame, using the CPU can be slow and the user can experience lag when moving
their head. Therefore in this code I have used the GPU module of OpenCV which
uses CUDA to process each frame using Haar cascades. It is still not perfect,
but compared to using the CPU it is significantly better.

## Dependencies
This is a module written for the Godot engine, which can be found [here](https://github.com/okamstudio/godot/).

You need to have a recent version of OpenCV which was compiled with GPU support.

The GPU module of OpenCV is implemented using Nvidia's CUDA so unfortunately you
are going to need a CUDA compatible GPU.

I have OpenCV 2.4.10 which I built with CUDA 6.5. I'm using the proprietary
Nvidia driver 340.65 on Debian GNU/Linux. I had problems when I tried to build
OpenCV with older versions of CUDA so be adviced.

You can find more information about the OpenCV GPU module [here](http://docs.opencv.org/modules/gpu/doc/introduction.html).

## Usage
Copy the `cvperspective` folder into the `modules` directory under the Godot
engine's root directory. Compile Godot as usual, open the example project
included in the `example_project` directory.

**Note:** The code uses haar cascades to detect the position of the face. The
path to the xml file for the cascade should be specified correctly in the
`OpenCVPerspective.cpp` file.

When the `start()` function is called in Godot, a new thread is created which
connects to the web cam and gets the detected face's position each frame. Using
the `get_horizontal_offset()` and `get_vertical_offset()` functions, you can
get the offset of the user's head from the middle of the screen for the latest
processed frame. Using the offset values, you can modify the camera's transform
values accordingly.


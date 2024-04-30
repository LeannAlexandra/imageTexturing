# Image Texturing Project

This project applies texture to an input image using OpenCV and C++.

## Table of Contents

- [Overview](#overview)
- [Prerequisites](#prerequisites)
- [Setup](#setup)
- [Usage](#usage)
- [Makefile](#makefile)
- [Functionality](#functionality)
- [Contributing](#contributing)
- [License](#license)

## Overview

<table align="center">
  <tr>
    <td align="center">
      <img src="https://github.com/LeannAlexandra/imageTexturing/blob/main/inputImage.jpg?raw=true" alt="Input Image" style="max-width: 100%; max-height: 200px; margin-bottom: 10px;">
      <br>
      <figcaption>Input Image: Original image used for texture mapping. - <a href="https://unsplash.com/@lucabravo">Luca Bravo</a> @<a href="https://unsplash.com/photos/brown-house-near-body-of-water-zAjdgNXsMeg">Unsplash</a></figcaption>
    </td>
    <td align="center">
      <img src="https://github.com/LeannAlexandra/imageTexturing/blob/main/canvas.jpg?raw=true" alt="Texture" style="max-width: 100%; max-height: 200px; margin-bottom: 10px;">
      <br>
      <figcaption>Texture: Image used as a texture overlay. -<a href="https://unsplash.com/@anniespratt">Annie Sprat</a> <a href="https://unsplash.com/photos/white-textile-with-black-line-xz485Eku8O4">Unsplash</a></figcaption>
    </td>
    <td align="center">
      <img src="https://github.com/LeannAlexandra/imageTexturing/blob/main/exampleoutput.jpg?raw=true" alt="Final Image" style="max-width: 100%; max-height: 200px; margin-bottom: 10px;">
      <br>
      <figcaption>Final Image: Resulting image with applied texture.</figcaption>
    </td>
  </tr>
</table>


## Prerequisites

Before running the program, ensure you have the following installed:

- C++ compiler
- OpenCV (version 4.x) installed

## Setup

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/LeannAlexandra/imageTexturing.git
   cd imageTexturing
2. **Compile the Program:**
    ```bash
    g++ -std=c++11 -o addTexture main.cpp `pkg-config --cflags --libs opencv4`
    ```
    **OR,** 
    use the makefile: 
    ```bash
    make
    ```

## Makefile
    # Makefile for Image Texturing Project
    CXX = g++
    CXXFLAGS = -std=c++11 `pkg-config --cflags opencv4`
    LIBS = `pkg-config --libs opencv4`

    SRCS = main.cpp
    OBJS = $(SRCS:.cpp=.o)
    TARGET = addTexture

    all: $(TARGET)

    $(TARGET): $(OBJS)
	    $(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

    %.o: %.cpp
	    $(CXX) $(CXXFLAGS) -c -o $@ $<

    clean:
	    rm -f $(OBJS) $(TARGET)

## Usage 

1. **Run the Program:**
    ```bash
    ./addTexture
2. **Expected Output:**
- The program will display the final texture in a window.
- if the saveImage flag is set to true, the output image'output.jpg' will be saved in the project directory.

## Functionality

    The program reads an input image (inputImage.jpg) and a texture image (canvas.jpg) from the project directory.
    It overlays the texture onto the input image and adjusts brightness based on texture intensity.
    The resulting image is displayed and optionally saved to disk (exampleoutput.jpg).

## Contributing

Contributions are welcome! If you have ideas for improvements or find any issues, please open an issue or submit a pull request.

## License

This project is licensed under the MIT License.

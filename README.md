<!---
Title:       |  Write text on the image
Subtitle:    |  Text color will be selected automatically based on the given image
Project:     |  text-on-image
Author:      Ashok Kumar Pant 
Affiliation: Tribhuvan University, Kathmandu 
Web:         http://ashokpant.github.io
Date:        July 27, 2016 
-->
# **Write on the image**
Write on the image with automatic text color. Color is selected based on the input image.

## Prerequisites
* [OpenCV](http://opencv.org/)

## Compilation
	$ git clone https://github.com/ashokpant/text-on-image.git
	$ cd text-on-image
	$ mkdir build
	$ cd build
	$ cmake ..
	$ make 
	
## Uses Example
	$ cd text-on-image/build
	$ ./text-on-image ../flower.jpg "Love from Nepal"  
	
	Note: [Uses: ./text-on-image <image_path> [<text>]]
	Left mouse button click for specific location
	CTRL + Mouse Move for free text move
![ ](https://github.com/ashokpant/text-on-image/blob/master/demo/text-on-image.gif)
	
## Other Examples

![ ](https://github.com/ashokpant/text-on-image/blob/master/demo/text-on-image1.gif)

![ ](https://github.com/ashokpant/text-on-image/blob/master/demo/text-on-image2.gif)

![ ](https://github.com/ashokpant/text-on-image/blob/master/demo/text-on-image3.gif)

# ZED - OpenCV - API

## Getting started

- First, download the latest version of the ZED SDK on [stereolabs.com](https://www.stereolabs.com).
- For more information, read the ZED [API documentation](https://www.stereolabs.com/developers/documentation/API/).

### Prerequisites

- Windows 7 64bits or later, Ubuntu 16.04 or 18.04
- [ZED SDK](https://www.stereolabs.com/developers/) and its dependencies ([CUDA](https://developer.nvidia.com/cuda-downloads), [OpenCV](https://github.com/opencv/opencv/releases))

### Usage

In camera api, you can see the left,right and depth image, and you can choose different modes:

- press a:auto save stereo image in ./images;
- press b:manual mode->prss s:save side-by-side images;p:Point Cloud;d:depth image;k:get the left_right image
- press c:auto save stereo image and depth image in ./images and ./depth;
- press d:setting mode:set parameters->input exposure,brightness,gain,whitebalance.
- press z:quit the current mode.(In setting mode, after updating the parameters, quit the setting mode immediately without 'z')

In camera-control

- This table lists keyboard shortcuts that you can use. Press the key to change the setting and the result can be seen. 
- Parameters can be changed: Exposure,Gain,Saturation,Hue,Contrast,Brightness
- Setting mode(d) in camera api can also change the parameters.

Parameter             | Description                   |   Hotkey
---------------------|------------------------------------|-------------------------------------------------
Switch settings | Toggle between camera settings | 's'
Increase settings value | Increase current settings value | '+'
Decrease settings value | Decrease current settings value | '-'
Reset | Reset all parameters to default values | 'r'
Exit         | Quit the application             | 'q'



### C++

Read the [guide](./cpp) to learn how to build and launch this sample in C++.




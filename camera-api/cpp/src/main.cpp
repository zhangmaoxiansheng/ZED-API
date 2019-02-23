///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2018, STEREOLABS.
//
// All rights reserved.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

/***********************************************************************************************
 ** This sample demonstrates how to use the ZED SDK with OpenCV. 					  	      **
 ** Depth and images are captured with the ZED SDK, converted to OpenCV format and displayed. **
 ***********************************************************************************************/

 // ZED includes
#include <sl_zed/Camera.hpp>

// OpenCV includes
#include <opencv2/opencv.hpp>

// Sample includes
#include <SaveDepth.hpp>

#include <stdlib.h>
#include <string>
#include <stdio.h>

using namespace sl;
CAMERA_SETTINGS camera_settings_ = CAMERA_SETTINGS_EXPOSURE;
std::string setting = "Exposure";
cv::Mat slMat2cvMat(Mat& input);
void printHelp();

void resetCameraSettings(sl::Camera &zed) {
    std::cout << "Reset all settings to default" << std::endl;
    zed.setCameraSettings(sl::CAMERA_SETTINGS_BRIGHTNESS, -1, true);
    zed.setCameraSettings(sl::CAMERA_SETTINGS_CONTRAST, -1, true);
    zed.setCameraSettings(sl::CAMERA_SETTINGS_HUE, -1, true);
    zed.setCameraSettings(sl::CAMERA_SETTINGS_SATURATION, -1, true);
    zed.setCameraSettings(sl::CAMERA_SETTINGS_GAIN, -1, true);
    zed.setCameraSettings(sl::CAMERA_SETTINGS_EXPOSURE, -1, true);
    zed.setCameraSettings(sl::CAMERA_SETTINGS_WHITEBALANCE, -1, true);
}

void setCameraSettings(sl::Camera &zed,int ex = -1,int br = 4,int ga = 1,int w = 4600){
    //attention: expusure -1 means auto exposure
    zed.setCameraSettings(sl::CAMERA_SETTINGS_EXPOSURE, ex);
    zed.setCameraSettings(sl::CAMERA_SETTINGS_BRIGHTNESS, br);
    zed.setCameraSettings(sl::CAMERA_SETTINGS_GAIN, ga);
    zed.setCameraSettings(sl::CAMERA_SETTINGS_WHITEBALANCE, w);
}
void printSettings(sl::Camera &zed){
    std::cout<<"Exposure:"<<zed.getCameraSettings(CAMERA_SETTINGS_EXPOSURE)<<std::endl;
    std::cout<<"Brightness:"<<zed.getCameraSettings(CAMERA_SETTINGS_BRIGHTNESS)<<std::endl;
    std::cout<<"Gain:"<<zed.getCameraSettings(CAMERA_SETTINGS_GAIN)<<std::endl;
    std::cout<<"Whitebalance:"<<zed.getCameraSettings(CAMERA_SETTINGS_WHITEBALANCE)<<std::endl;
}

int main(int argc, char **argv) {

    // Create a ZED camera object
    Camera zed;
    //setting param
    int ex = 50;
    int br = 4;
    int ga = 1;
    int w = 4600;
    // Set configuration parameters
    InitParameters init_params;
    init_params.camera_resolution = RESOLUTION_HD1080;
    init_params.depth_mode = DEPTH_MODE_PERFORMANCE;
    init_params.coordinate_units = UNIT_METER;
    //if (argc > 1) init_params.svo_input_filename.set(argv[1]);
    
    // Open the camera
    ERROR_CODE err = zed.open(init_params);
    if (err != SUCCESS) {
        printf("%s\n", toString(err).c_str());
        zed.close();
        return 1; // Quit if an error occurred
    }

    // Display help in console
    //printHelp();

    // Set runtime parameters after opening the camera
    RuntimeParameters runtime_parameters;
    runtime_parameters.sensing_mode = SENSING_MODE_STANDARD;

    // Prepare new image size to retrieve half-resolution images
    Resolution image_size = zed.getResolution();
    int new_width = image_size.width;
    int new_height = image_size.height;

    // To share data between sl::Mat and cv::Mat, use slMat2cvMat()
    // Only the headers and pointer to the sl::Mat are copied, not the data itself
    Mat image_zed(new_width, new_height, MAT_TYPE_8U_C4);
    cv::Mat image_ocv = slMat2cvMat(image_zed);
    Mat depth_image_zed(new_width, new_height, MAT_TYPE_8U_C4);
    cv::Mat depth_image_ocv = slMat2cvMat(depth_image_zed);
    Mat point_cloud;
    
    int frame_num = 0;
    // Loop until 'q' is pressed
    char key = ' ';
    int flag = 0;
    int res;
    std::cout<<"Please choose the mode. a:auto save stereo img, b:manual mode, c:auto save stereo img and depth, d:set the parameter, z:quit the current mode"<<std::endl;
    while (key != 'q') {

        if (zed.grab(runtime_parameters) == SUCCESS) {

            cv::namedWindow("left",0);
            cv::namedWindow("right",0);
            cv::namedWindow("depth",0);

            Mat buffer_sl;
            cv::Mat buffer_cv;
            cv::Mat left_image;
            cv::Mat right_image;
            
            zed.retrieveImage(buffer_sl, VIEW_LEFT,MEM_CPU,image_size.width,image_size.height);
            buffer_cv = cv::Mat(buffer_sl.getHeight(), buffer_sl.getWidth(), CV_8UC4, buffer_sl.getPtr<sl::uchar1>(MEM_CPU));
            buffer_cv.copyTo(left_image);
            
            zed.retrieveImage(buffer_sl, VIEW_RIGHT,MEM_CPU,image_size.width,image_size.height);
            buffer_cv = cv::Mat(buffer_sl.getHeight(), buffer_sl.getWidth(), CV_8UC4, buffer_sl.getPtr<sl::uchar1>(MEM_CPU));
            buffer_cv.copyTo(right_image);
            
            zed.retrieveImage(depth_image_zed, VIEW_DEPTH, MEM_CPU, new_width, new_height);
                    
            cv::cvtColor(left_image,left_image,cv::COLOR_RGBA2RGB);
            cv::cvtColor(right_image,right_image,cv::COLOR_RGBA2RGB);
            
           
            //saveDepth(zed,"./depth/" + std::to_string(frame_num));
            cv::imshow("left",left_image);
            cv::imshow("right",right_image);
            cv::imshow("depth",depth_image_ocv);
            switch (key){
                case 'a':
                {
                    std::cout<<"auto save stereo image"<<std::endl;
                    res = system("mkdir ./images");
                    flag = 1;
                }
                break;
                case 'z':
                {
                    std::cout<<"quit the current mode,please choose the mode again"<<std::endl;
                    flag = 0;
                }
                break;
                case 'b':
                {
                    std::cout<<"manual mode"<<std::endl;
                    printHelp();
                    res = system("mkdir ./images_manual");
                    flag = 2;
                }
                break;
                case 'c':
                {
                    std::cout<<"auto save stereo image and depth"<<std::endl;
                    res = system("mkdir ./images");
                    res = system("mkdir ./depth");
                    flag = 3;
                }
                case 'd':
                {
                    std::cout<<"setting mode,please input exposure,brightness,gain,whitebalance"<<std::endl;
                    printSettings(zed);
                    std::cin>>ex>>br>>ga>>w;
                    setCameraSettings(zed,ex,br,ga,w);
                    std::cout<<"now"<<std::endl;
                    printSettings(zed);
                }
            }

            if(flag==1)
            {
                cv::imwrite("./images/" + std::to_string(frame_num) + "_l.png",left_image);
                cv::imwrite("./images/" + std::to_string(frame_num) + "_r.png",right_image);
                frame_num++;
            }
            if(flag==2)
            {
                processKeyEvent(zed,key);
            }
            if(flag == 3)
            {
                cv::imwrite("./images/" + std::to_string(frame_num) + "_l.png",left_image);
                cv::imwrite("./images/" + std::to_string(frame_num) + "_r.png",right_image);
                cv::imwrite("./depth/" + std::to_string(frame_num) + "_d.png",depth_image_ocv);
                frame_num++;
            }
            
            
            // Handle key event
            
            key = cv::waitKey(10);

        }
    }
    zed.close();
    return 0;
}

/**
* Conversion function between sl::Mat and cv::Mat
**/
cv::Mat slMat2cvMat(Mat& input) {
    // Mapping between MAT_TYPE and CV_TYPE
    int cv_type = -1;
    switch (input.getDataType()) {
        case MAT_TYPE_32F_C1: cv_type = CV_32FC1; break;
        case MAT_TYPE_32F_C2: cv_type = CV_32FC2; break;
        case MAT_TYPE_32F_C3: cv_type = CV_32FC3; break;
        case MAT_TYPE_32F_C4: cv_type = CV_32FC4; break;
        case MAT_TYPE_8U_C1: cv_type = CV_8UC1; break;
        case MAT_TYPE_8U_C2: cv_type = CV_8UC2; break;
        case MAT_TYPE_8U_C3: cv_type = CV_8UC3; break;
        case MAT_TYPE_8U_C4: cv_type = CV_8UC4; break;
        default: break;
    }

    // Since cv::Mat data requires a uchar* pointer, we get the uchar1 pointer from sl::Mat (getPtr<T>())
    // cv::Mat and sl::Mat will share a single memory structure
    return cv::Mat(input.getHeight(), input.getWidth(), cv_type, input.getPtr<sl::uchar1>(MEM_CPU));
}

/**
* This function displays help in console
**/
void printHelp() {
    std::cout << " Press 's' to save Side by side images" << std::endl;
    std::cout << " Press 'p' to save Point Cloud" << std::endl;
    std::cout << " Press 'd' to save Depth image" << std::endl;
    std::cout << " Press 'm' to switch Point Cloud format" << std::endl;
    std::cout << " Press 'n' to switch Depth format" << std::endl;
    std::cout << " press 'k' to get the left and right image" <<std::endl;
}

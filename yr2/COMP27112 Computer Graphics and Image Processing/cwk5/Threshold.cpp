#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.hpp>
using namespace cv;

cv::Mat pic,raw,des;
int theta = 0;

void onChangeTheta(int, void*){

  cv::threshold(raw, des, theta, 255, CV_THRESH_BINARY);
  namedWindow("rawpic",CV_WINDOW_NORMAL);
  namedWindow("thrpic",CV_WINDOW_NORMAL);
  imshow("rawpic",raw);
  imshow("thrpic",des);
}

int main (int argc, char* argv[]){
// Print the OpenCV version
    pic = imread("coyote.bmp", CV_LOAD_IMAGE_COLOR);
    if(pic.data==NULL){
        printf("read error\n");   return -1;
    }
    cvtColor(pic,raw,CV_RGB2GRAY);
    cv::threshold(raw, des, theta, 255, CV_THRESH_BINARY);
    namedWindow("rawpic",CV_WINDOW_NORMAL);
    namedWindow("thrpic",CV_WINDOW_NORMAL);
    imshow("rawpic",raw);
    imshow("thrpic",des);
    createTrackbar("theta", "thrpic", &theta, 255, onChangeTheta);
    waitKey(0);
    return 0;
}

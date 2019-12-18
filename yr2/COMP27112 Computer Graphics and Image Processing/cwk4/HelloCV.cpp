#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;
int main (int argc, char* argv[]){
// Print the OpenCV version
    printf( "OpenCV version : %d.%d\n" , CV_MAJOR_VERSION,CV_MINOR_VERSION) ;
    cv::Mat pic = imread("coyote.bmp", CV_LOAD_IMAGE_COLOR);;
    if(pic.data==NULL){
        printf("read error\n");   return -1;
    }
    namedWindow("testpic",CV_WINDOW_NORMAL);
    imshow("testpic",pic);
    waitKey(0);
    return 0;
}

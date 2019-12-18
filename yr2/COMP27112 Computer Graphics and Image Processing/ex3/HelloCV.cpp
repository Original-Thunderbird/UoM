#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
int rad=0, histSize=255;
int sig;
float range[]={0,255};
const float* histRange = {range};
cv::Mat pic,raw_gry,blr_gry,dif_gry,raw_hist,res_hist;

cv::Mat drawHist(cv::Mat hist, int scale){
  double mx = 0;
  cv::minMaxLoc(hist, 0, &mx, 0, 0);
  cv::Mat result = cv::Mat::zeros(256*scale, 256*scale, CV_8UC1);
  for(int i=0; i<255; i++){
    //Get the  histogram  values
    float histValue = hist.at<float>(i,0);
    float nextValue = hist.at<float>(i+1,0);
    // Create  4  points  f o r  the  poly
    cv::Point p1 = cv::Point(i*scale, 256*scale);
    cv::Point p2 = cv::Point(i*scale+scale, 256*scale);
    cv::Point p3 = cv::Point(i*scale+scale, (256-nextValue*256/mx)*scale);
    cv::Point p4 = cv::Point(i*scale, (256-nextValue*256/mx)*scale);
    //Draw the  poly ( Ending  in  p1 )
    int numPoints = 5;
    cv::Point points [] = {p1, p2, p3, p4, p1};
    cv::fillConvexPoly(result, points, numPoints, cv::Scalar::all(256), 0, 0);
  }
  return result;
}

void onChangeRadius(int, void*){
  GaussianBlur(raw_gry, blr_gry, cv::Size(2*rad+1,2*rad+1), 0, 0);
  //printf("%d\n",rad);
  dif_gry = raw_gry - blr_gry + 128;
  calcHist(&dif_gry,1,0,Mat(),raw_hist,1,&histSize,&histRange,true,false);
  //subtract(raw_gry,blr_gry+128,dif_gry);
  imshow("blrpic",blr_gry);
  imshow("difpic",dif_gry);

  //drawHist(raw_hist,histSize);
  res_hist = drawHist(raw_hist, 2);
  imshow("histogram",res_hist);
}


int main (int argc, char* argv[]){
// Print the OpenCV version
    printf( "OpenCV version : %d.%d\n" , CV_MAJOR_VERSION,CV_MINOR_VERSION) ;
    pic = imread("noise4.jpg", CV_LOAD_IMAGE_COLOR);
    if(pic.data==NULL){
        printf("read error\n");   return -1;
    }
    cvtColor(pic,raw_gry,CV_RGB2GRAY);
    GaussianBlur(raw_gry, blr_gry, cv::Size(2*rad+1,2*rad+1), 0, 0);
    dif_gry = raw_gry - blr_gry + 128;
    calcHist(&dif_gry,1,0,Mat(),raw_hist,1,&histSize,&histRange,true,false);
    namedWindow("difpic",CV_WINDOW_AUTOSIZE);
    namedWindow("rawpic",CV_WINDOW_AUTOSIZE);
    namedWindow("blrpic",CV_WINDOW_AUTOSIZE);
    namedWindow("oripic",CV_WINDOW_AUTOSIZE);
    imshow("difpic",dif_gry);
    imshow("rawpic",raw_gry);
    imshow("blrpic",blr_gry);
    imshow("oripic",pic);
    namedWindow("histogram",CV_WINDOW_AUTOSIZE);
    res_hist = drawHist(raw_hist, 2);
    imshow("histogram",res_hist);
    createTrackbar("radius", "difpic", &rad, 30, onChangeRadius);
    // createTrackbar("radius", "testpic", &rad, 100, onChangeRadius);
    // onChangeRadius(rad,0);

    waitKey(0);
    return 0;
}

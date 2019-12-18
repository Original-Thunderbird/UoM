#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

Mat dst, can_gry, pic, cln, raw;
int can_thr = 1, rad=0, radius = 1, min_pts = 0, gap_pts = 0;

vector<double> coff;
Vec4i l;

vector<double> fitPoly(vector<Point> points, int n){
    int nPoints = points.size();
    // Vectors  for all the points��?xs and ys
    vector<float> xValues = vector<float>();
    vector<float> yValues = vector<float>();
    // Split the points into two vectors for x and y values
    for (int i = 0; i < nPoints; i++){
        xValues.push_back(points[i].x);
        yValues.push_back(points[i].y);
    }
    //Augmented matrix
    double matrixSystem[n+1][n+2];
    for (int row=0; row<n+1; row++){
        for (int col=0; col<n+1; col++){
            matrixSystem[row][col] = 0;
            for (int i=0; i < nPoints; i++)
                matrixSystem[row][col] += pow(xValues[i], row + col);
        }
        matrixSystem[row][n + 1] = 0;
        for (int i=0; i < nPoints; i++)
            matrixSystem[row][n+1] += pow(xValues[i], row)*yValues[i];
    }
    //Array  that  holds all the coefficients
    double coeffVec[n+2];
    //Gauss  reduction
    for (int i=0; i<=n-1; i++)
        for (int k=i+1; k<=n; k++){
            double t=matrixSystem[k][i]/matrixSystem[i][i];
            for (int j=0; j<=n+1; j++)
                matrixSystem[k][j]=matrixSystem[k][j]-t*matrixSystem[i][j];
        }
    //Back substitution
    for (int i=n; i>=0; i--){
        coeffVec[i] = matrixSystem[i][n+1];
        for (int j=0; j<=n+1; j++)
            if (j!=i)
                coeffVec[i]=coeffVec[i]-matrixSystem[i][j]*coeffVec[j];
        coeffVec[i]=coeffVec[i]/matrixSystem[i][i];
    }
    // Construct the cv vector and return it
    vector<double> result=vector<double>();
    for (int i=0; i<n+1; i++)
        result.push_back(coeffVec[i]);
    return result;
}

// Returns  the  point  for  the  equation  determined
//by a vector of  coefficents, at a certain x location
Point pointAtX(vector<double> coeff, double x){
    double y=0;
    for (int i=0; i<coeff.size(); i++)
        y += pow(x,i)*coeff[i];
    return Point(x,y);
}

void HoughandRegre(int, void *){
    vector<Vec4i> lines;
    vector<Point> rem_pts;
    HoughLinesP(dst, lines, 1, CV_PI / 180, 1, min_pts, gap_pts);
    for (int i = 0; i < lines.size(); i++){
        l = lines[i];
        double angle = atan2(l[3] - l[1], l[2] - l[0]) * 180.0 / CV_PI;
        if (angle > 100 || angle < 80){
            line(dst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);
            rem_pts.push_back(Point(l[0], l[1]));
            rem_pts.push_back(Point(l[2], l[3]));
        }
    }
    imshow("for hough", dst);
    coff = fitPoly(rem_pts, 2);
    cln = raw.clone();
    for(int i = 0; i < rem_pts.size(); i++){
        circle(cln, pointAtX(coff, i), radius, (0, 255, 255));
    }
    imshow("original image", cln);
}

void CannyThreshold(int, void *){
    GaussianBlur(pic, can_gry, cv::Size(2*rad+1,2*rad+1), 0, 0);
    Canny(can_gry, can_gry, can_thr, can_thr*3, 3);
    //dst = Scalar::all(0);
    pic.copyTo(dst, can_gry);
    imshow("for canny", dst);
    HoughandRegre(0, 0);
}

int main(int argc, char *argv[]){
    raw = imread(argv[1]);
    if (raw.data==NULL){
        printf("read error\n");   return -1;
    }
    if(!strcmp(argv[1],"horizon0.jpg")||!strcmp(argv[1],"horizon1.jpg"))
      resize(raw, raw, Size(), 0.15, 0.15, CV_INTER_LINEAR);
    imshow("original image", raw);
    cvtColor(raw, pic, CV_BGR2GRAY);
    namedWindow("for canny", CV_WINDOW_AUTOSIZE);

    createTrackbar("Canny Thres", "for canny", &can_thr, 100, CannyThreshold);
    createTrackbar("Blur Ratio:", "for canny", &rad, 100, CannyThreshold);
    CannyThreshold(0, 0);
    createTrackbar("MinLen:", "for hough", &min_pts, 50, HoughandRegre);
    createTrackbar("MaxGap:", "for hough", &gap_pts, 50, HoughandRegre);
    waitKey(0);
    return 0;
}

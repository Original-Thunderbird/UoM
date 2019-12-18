#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

Mat src1, result1;
Mat src, src_gray;
Mat dst, detected_edges;
Mat cdst;
int lowThreshold = 1;
int maxThreshold = 100;
int ratio = 3;
int kernel_size = 3;

int edgeThresh = 1, rad=0;
int draw_circle_radius = 1;
int const max_lowThreshold = 100;
char window_name[30] = "Apply Canny Detector";
char window_name_2[50] = "Apply Hough Line Transformation";
char window_name_3[50] = "Detect horizon";
int min_num_intersection = 10, min_num_points = 10, max_gap_points = 10, bias_angle = 10.0;
char picname[13]="horizon3.jpg";

/*Resources*/
// https://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/canny_detector/canny_detector.html
// https://docs.opencv.org/2.4/doc/tutorials/imgproc/imgtrans/hough_lines/hough_lines.html

vector<double> fitPoly(vector<Point> points, int n);
Point pointAtX(vector<double> coeff, double x);
void CannyThreshold(int, void *);
void ApplyHoughLinesP(int, void *);

int main(int argc, char *argv[])
{
    // Convert to greyscale
    // src1 = imread("./horizon0.jpg", CV_LOAD_IMAGE_COLOR);
    // src1 = imread("./horizon0.jpg", IMREAD_GRAYSCALE);

    /* Maybe a better way to convert to greyscale image */
    src = imread(picname);
    if (src.empty())
    {
        return -1;
    }

    // cvtColor(src1, src1, CV_GRAY2BGR);

    // src2 = imread("./noise3.jpg", IMREAD_GRAYSCALE);

    /* Resize the image so whole image can be viewed on the screen */
    // namedWindow("lab4_horizon0", CV_WINDOW_AUTOSIZE);
    dst.create(src.size(), src.type());
    cvtColor(src, src, CV_BGR2GRAY);
    if(!strcmp(picname,"horizon0.jpg")||!strcmp(picname,"horizon1.jpg"))
      resize(src, src, Size(), 0.2, 0.2, CV_INTER_LINEAR);
    namedWindow(window_name, CV_WINDOW_AUTOSIZE);
    imshow("source image", src);
    /* Apply Canny filter */

    createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);
    createTrackbar("Blur:", window_name, &rad, 100, CannyThreshold);
    CannyThreshold(0, 0);
    createTrackbar("Threshold:", window_name_2, &min_num_intersection, max_lowThreshold, ApplyHoughLinesP);
    createTrackbar("MinLen:", window_name_2, &min_num_points, max_lowThreshold, ApplyHoughLinesP);
    createTrackbar("MaxGap:", window_name_2, &max_gap_points, max_lowThreshold, ApplyHoughLinesP);
    createTrackbar("Bias_angle:", window_name_2, &bias_angle, 90, ApplyHoughLinesP);

    waitKey(0);
    // imwrite( "./Difference_Image.jpg", result1);
    return 0;
}

void CannyThreshold(int, void *)
{
    //optional step, reduce noise
    /// Reduce noise with a kernel 3x3
    GaussianBlur(src, detected_edges, cv::Size(2*rad+1,2*rad+1), 0, 0);

    /// Canny detector
    Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * ratio, kernel_size);

    /// Using Canny's output as a mask, we display our result
    dst = Scalar::all(0);

    src.copyTo(dst, detected_edges);
    imshow(window_name, dst);
    ApplyHoughLinesP(0, 0);
}

void ApplyHoughLinesP(int, void *)
{

    /* Apply probabilistic Hough transformation */
    vector<Vec4i> lines;
    vector<Point> filtered_lines_points;
    // 1 means resolution of parameter is 1 pixel
    // CV_PI/180 means resolution is 1 degree
    // The first param means the minimum number of intersections to detect a line
    // The second param means the minimum number of points to form a line, and lines with less points are disregarded
    // The third param means te maximm gap between two points to be considered in the same line
    if (min_num_intersection < 1)
    {
        min_num_intersection = 1;
    }
    if (min_num_points < 1)
    {
        min_num_points = 1;
    }
    if (max_gap_points < 1)
    {
        max_gap_points = 1;
    }

    HoughLinesP(dst, lines, 1, CV_PI / 180, min_num_intersection, min_num_points, max_gap_points);
    for (int i = 0; i < lines.size(); i++)
    {
        Vec4i l = lines[i];
        double angle = atan2(l[3] - l[1], l[2] - l[0]) * 180.0 / CV_PI;
        if (angle >= 90.0 + bias_angle || angle <= 90.0 - bias_angle)
        {
            // filter out vertical lines
            /*lines is vector storing (Xstart, Ystart, Xend, Yend */
            line(dst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);
            filtered_lines_points.push_back(Point(l[0], l[1]));
            filtered_lines_points.push_back(Point(l[2], l[3]));
        }
    }
    imshow(window_name_2, dst);
    vector<double> draw_curve_coeff = fitPoly(filtered_lines_points, 2);
    for(int num_draw_point = 0; num_draw_point < filtered_lines_points.size(); num_draw_point++)
    {
        circle(dst, pointAtX(draw_curve_coeff, num_draw_point), draw_circle_radius, (0, 255, 255));
    }
    imshow(window_name_3, dst);
}

vector<double> fitPoly(vector<Point> points, int n)
{
    int nPoints = points.size();

    // Vectors  for all the points��?xs and ys
    vector<float> xValues = vector<float>();
    vector<float> yValues = vector<float>();
    // Split the points into two vectors for x and y values

    for (int i = 0; i < nPoints; i++)
    {
        xValues.push_back(points[i].x);
        yValues.push_back(points[i].y);
    }
    //Augmented matrix
    double matrixSystem[n + 1][n + 2];
    for (int row = 0; row < n + 1; row++)
    {
        for (int col = 0; col < n + 1; col++)
        {
            matrixSystem[row][col] = 0;
            for (int i = 0; i < nPoints; i++)
                matrixSystem[row][col] += pow(xValues[i], row + col);
        }

        matrixSystem[row][n + 1] = 0;

        for (int i = 0; i < nPoints; i++)
            matrixSystem[row][n + 1] += pow(xValues[i], row) * yValues[i];
    }

    //Array  that  holds all the coefficients
    double coeffVec[n + 2];

    //Gauss  reduction
    for (int i = 0; i <= n - 1; i++)
        for (int k = i + 1; k <= n; k++)
        {
            double t = matrixSystem[k][i] / matrixSystem[i][i];

            for (int j = 0; j <= n + 1; j++)
                matrixSystem[k][j] = matrixSystem[k][j] - t * matrixSystem[i][j];
        }

    //Back substitution
    for (int i = n; i >= 0; i--)
    {
        coeffVec[i] = matrixSystem[i][n + 1];
        for (int j = 0; j <= n + 1; j++)
            if (j != i)
                coeffVec[i] = coeffVec[i] - matrixSystem[i][j] * coeffVec[j];
        coeffVec[i] = coeffVec[i] / matrixSystem[i][i];
    }
    // Construct the cv vector and return it
    vector<double> result = vector<double>();
    for (int i = 0; i < n + 1; i++)
        result.push_back(coeffVec[i]);
    return result;
}

// Returns  the  point  for  the  equation  determined
//by a vector of  coefficents, at a certain x location
Point pointAtX(vector<double> coeff, double x)
{
    double y = 0;
    for (int i = 0; i < coeff.size(); i++)
        y += pow(x, i) * coeff[i];
    return Point(x, y);
}

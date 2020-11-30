#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <ctime>

using namespace cv;
using namespace std;

Mat create_noise(const Mat &src) {
    if (src.empty()) {
        cout << "Upload image" << endl;
        exit(-1);
    }
    Mat noise(src.size(), src.type());
    Mat result(src.size(), src.type());
    float m = (10, 12, 34);
    float sigma = (1, 5, 50);
    randn(noise, m, sigma); //mean and variance
    result = src + noise;
    return result;
}

Mat MyMedianFilter(const Mat& src) {
    if (src.empty()) {
        cout << "Upload image" << endl;
        exit(-1);
    }
    Mat result = src.clone();
    int Rmatrix[9];
    int Gmatrix[9];
    int Bmatrix[9];
    for (int x = 1; x < src.rows - 1; x++) {
        for (int y = 1; y < src.cols - 1; y++) {
            Rmatrix[0] = src.at<Vec3b>(x - 1, y - 1)[0];
            Rmatrix[1] = src.at<Vec3b>(x, y - 1)[0];
            Rmatrix[2] = src.at<Vec3b>(x + 1, y - 1)[0];
            Rmatrix[3] = src.at<Vec3b>(x - 1, y)[0];
            Rmatrix[4] = src.at<Vec3b>(x, y)[0];
            Rmatrix[5] = src.at<Vec3b>(x + 1, y)[0];
            Rmatrix[6] = src.at<Vec3b>(x - 1, y + 1)[0];
            Rmatrix[7] = src.at<Vec3b>(x, y + 1)[0];
            Rmatrix[8] = src.at<Vec3b>(x + 1, y + 1)[0];
            
            Gmatrix[0] = src.at<Vec3b>(x - 1, y - 1)[1];
            Gmatrix[1] = src.at<Vec3b>(x, y - 1)[1];
            Gmatrix[2] = src.at<Vec3b>(x + 1, y - 1)[1];
            Gmatrix[3] = src.at<Vec3b>(x - 1, y)[1];
            Gmatrix[4] = src.at<Vec3b>(x, y)[1];
            Gmatrix[5] = src.at<Vec3b>(x + 1, y)[1];
            Gmatrix[6] = src.at<Vec3b>(x - 1, y + 1)[1];
            Gmatrix[7] = src.at<Vec3b>(x, y + 1)[1];
            Gmatrix[8] = src.at<Vec3b>(x + 1, y + 1)[1];

            Bmatrix[0] = src.at<Vec3b>(x - 1, y - 1)[2];
            Bmatrix[1] = src.at<Vec3b>(x, y - 1)[2];
            Bmatrix[2] = src.at<Vec3b>(x + 1, y - 1)[2];
            Bmatrix[3] = src.at<Vec3b>(x - 1, y)[2];
            Bmatrix[4] = src.at<Vec3b>(x, y)[2];
            Bmatrix[5] = src.at<Vec3b>(x + 1, y)[2];
            Bmatrix[6] = src.at<Vec3b>(x - 1, y + 1)[2];
            Bmatrix[7] = src.at<Vec3b>(x, y + 1)[2];
            Bmatrix[8] = src.at<Vec3b>(x + 1, y + 1)[2];

            sort(Rmatrix, Rmatrix + 9);
            sort(Gmatrix, Gmatrix + 9);
            sort(Bmatrix, Bmatrix + 9);
            
            result.at<Vec3b>(x, y)[0] = Rmatrix[4];
            result.at<Vec3b>(x, y)[1] = Gmatrix[4];
            result.at<Vec3b>(x, y)[2] = Bmatrix[4];
        }
    }
    return result;
}

int kernel[3][3] = { 1, 2, 1,
                   2, 4, 2,
                   1, 2, 1 };

int getPixel(unsigned char* arr, int col, int row, int k, int width, int height)
{
    int sum = 0;
    int sumKernel = 0;

    for (int j = -1; j <= 1; j++)
    {
        for (int i = -1; i <= 1; i++)
        {
            if ((row + j) >= 0 && (row + j) < height && (col + i) >= 0 && (col + i) < width)
            {
                int color = arr[(row + j) * 3 * width + (col + i) * 3 + k];
                sum += color * kernel[i + 1][j + 1];
                sumKernel += kernel[i + 1][j + 1];
            }
        }
    }

    return sum / sumKernel;
}

void MyGaussFilter(unsigned char* arr, unsigned char* result, int width, int height)
{
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            for (int k = 0; k < 3; k++)
            {
                result[3 * row * width + 3 * col + k] = getPixel(arr, col, row, k, width, height);
            }
        }
    }
}

int main() {
   // Task 1
   Mat img1 = imread("D:/OI_lab2/Image_processing_lab2/Lenna.png", CV_LOAD_IMAGE_COLOR);
   imshow("Original image", img1);
   Mat img2(img1.size(), img1.type());
   img2 = create_noise(img1);
   imshow("Image with noise", img2);
   // Task 2
   Mat img3 = img2.clone();
   unsigned int start_my = clock();
   img3 = MyMedianFilter(img2);
   unsigned int end_my = clock();
   imshow("MedianFilter", img3);
   cout << "Time of my MedianFilter :" << end_my - start_my << endl;

   Mat img4 = img2.clone();
   start_my = clock();
   MyGaussFilter(img2.data, img4.data, img2.cols, img2.rows);
   end_my = clock();
   imshow("MyGaussFilter", img4);
   cout << "Time of my GaussFilter :" << end_my - start_my << endl;
   // Task 3
   Mat img5(img2.size(), img2.type());
   unsigned int start_cv = clock();
   medianBlur(img2, img5, 3);
   unsigned int end_cv = clock();
   imshow("CV MedianFilter", img5);
   cout << "Time of CV MedianFilter :" << end_cv - start_cv << endl;

   Mat img6(img2.size(), img2.type());
   start_cv = clock();
   GaussianBlur(img2, img6, Size(3, 3), 0);
   end_cv = clock();
   imshow("CV GaussFilter", img6);
   cout << "Time of CV GaussFilter :" << end_cv - start_cv << endl;

   waitKey(999999);
   return 0;
}
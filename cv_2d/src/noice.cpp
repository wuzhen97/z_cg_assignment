#include "noice.hpp"
#include <iostream>
#include <random>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;

// 添加Gussia噪声
void addGaussianNoise(cv::Mat &m, int mu, int sigma)
{
    // 产生高斯分布随机数发生器
    // std::random_device rd;
    std::mt19937 gen(1);

    std::normal_distribution<> d(mu, sigma);

    auto rows = m.rows;                // 行数
    auto cols = m.cols * m.channels(); // 列数

    for (int i = 0; i < rows; i++)
    {
        auto p = m.ptr<uchar>(i); // 取得行首指针
        for (int j = 0; j < cols; j++)
        {
            auto tmp = p[j] + d(gen);
            tmp = tmp > 255 ? 255 : tmp;
            tmp = tmp < 0 ? 0 : tmp;
            p[j] = tmp;
        }
    }
}

// 添加椒盐噪声
void addSaltNoise(cv::Mat &img, const int num)
{
    if (img.empty())
    {
        cout << "img is empty!" << endl;
        exit(0);
    }

    // 随机数生成器
    default_random_engine generator;
    uniform_int_distribution<int> randomRow(0, img.rows - 1);
    uniform_int_distribution<int> randomCol(0, img.cols - 1);

    // 添加噪声
    int i, j;
    for (int k = 0; k < num; ++k)
    {
        i = randomRow(generator);
        j = randomCol(generator);
        if (img.type() == CV_8UC1) //灰度图
        {
            img.at<uchar>(i, j) = 255;
        }
        if (img.type() == CV_8UC3) // RGB图像
        {
            img.at<cv::Vec3b>(i, j)[0] = 255;
            img.at<cv::Vec3b>(i, j)[1] = 255;
            img.at<cv::Vec3b>(i, j)[2] = 255;
        }
    }
}

// 峰值信噪比
// 该部分代码来自opencv示例，此外还有cuda加速版本
// https://docs.opencv.org/4.3.0/dd/d3d/tutorial_gpu_basics_similarity.html
double getPSNR(const cv::Mat &I1, const cv::Mat &I2)
{
    cv::Mat s1;
    cv::absdiff(I1, I2, s1);                     // |I1 - I2|
    s1.convertTo(s1, CV_32F);                    // cannot make a square on 8 bits
    s1 = s1.mul(s1);                             // |I1 - I2|^2
    cv::Scalar s = sum(s1);                      // sum elements per channel
    double sse = s.val[0] + s.val[1] + s.val[2]; // sum channels
    if (sse <= 1e-10)                            // for small values return zero
        return 0;
    else
    {
        double mse = sse / (double)(I1.channels() * I1.total());
        double psnr = 10.0 * log10((255 * 255) / mse);
        return psnr;
    }
}
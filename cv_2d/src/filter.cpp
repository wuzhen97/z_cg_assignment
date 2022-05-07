#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

cv::Vec3b GGFp(cv::Mat ROI, const double sigma);
cv::Vec3b WGGFp(cv::Mat ROI, const double sigma, const double lambda);

/*
    引导高斯滤波（颜色域）
 */
void GGFfilter(const cv::Mat &srcImg, cv::Mat &dstImg, cv::Mat &guideImg, const int r, const double sigma)
{
    dstImg = cv::Mat::zeros(srcImg.size(), srcImg.type());
    // 边界填充
    cv::Mat imgWithBorder;
    cv::copyMakeBorder(guideImg, imgWithBorder, r, r, r, r, cv::BORDER_REPLICATE);
    // 遍历每一个像素
    for (int i = 0; i < srcImg.rows; ++i)
    {
        for (int j = 0; j < srcImg.cols; ++j)
        {
            cv::Mat ROI = imgWithBorder(cv::Rect(j, i, 2 * r + 1, 2 * r + 1)); //注意RECT实参为坐标而不是行列值
            dstImg.at<cv::Vec3b>(i, j) = GGFp(ROI, sigma);
        }
    }
}

/*
    像素集感知的引导高斯滤波（颜色域）
 */
void WGGFfilter(const cv::Mat &srcImg, cv::Mat &dstImg, cv::Mat &guideImg, const int r, const double sigma, const double lambda)
{
    dstImg = cv::Mat::zeros(srcImg.size(), srcImg.type());
    // 边界填充
    cv::Mat imgWithBorder;
    cv::copyMakeBorder(guideImg, imgWithBorder, r, r, r, r, cv::BORDER_REPLICATE);
    // 遍历每一个像素
    for (int i = 0; i < srcImg.rows; ++i)
    {
        for (int j = 0; j < srcImg.cols; ++j)
        {
            cv::Mat ROI = imgWithBorder(cv::Rect(j, i, 2 * r + 1, 2 * r + 1)); //注意RECT实参为坐标而不是行列值
            dstImg.at<cv::Vec3b>(i, j) = WGGFp(ROI, sigma, lambda);
        }
    }
}

cv::Vec3b GGFp(cv::Mat ROI, const double sigma)
{
    cv::Vec3b bgr_cneter = ROI.at<cv::Vec3b>((ROI.rows - 1) / 2, (ROI.cols - 1) / 2);
    cv::Vec3d result = 0, tau = 0;
    // 依据窗口ROI计算像素值
    for (int u = 0; u < ROI.rows; ++u)
    {
        for (int v = 0; v < ROI.cols; ++v)
        {
            cv::Vec3b bgr = ROI.at<cv::Vec3b>(v, u);
            for (int c = 0; c < ROI.channels(); ++c)
            {
                double nut = pow(bgr[c] - bgr_cneter[c], 2); //分子
                double det = 2 * pow(sigma, 2);              //分母
                double g = exp(-(nut / det));                //核函数g(.)
                result[c] += g * bgr[c];                     //加权平均
                tau[c] += g;                                 //归一化系数
            }
        }
    }
    // 归一化
    for (int c = 0; c < ROI.channels(); ++c)
    {
        result[c] /= tau[c];
    }

    return static_cast<cv::Vec3b>(result);
}

cv::Vec3b WGGFp(cv::Mat ROI, const double sigma, const double lambda)
{
    cv::Vec3b bgr_cneter = ROI.at<cv::Vec3b>((ROI.rows - 1) / 2, (ROI.cols - 1) / 2);
    cv::Vec3d result = 0, tau = 0;

    cv::Mat ROI_tmp = ROI.clone();
    // 像素集感知：判断是否满足临界条件
    int count = 0;
    for (int u = 0; u < ROI.rows; ++u)
    {
        for (int v = 0; v < ROI.cols; ++v)
        {
            cv::Vec3b &bgr = ROI_tmp.at<cv::Vec3b>(v, u);
            bool a = abs(bgr[0] - bgr_cneter[0]) > lambda;
            bool b = abs(bgr[1] - bgr_cneter[1]) > lambda;
            bool c = abs(bgr[2] - bgr_cneter[2]) > lambda;
            if (a && b && c)
            {
                bgr = {0, 0, 0};
                count++;
            }
        }
    }

    // 像素集感知：决定对该像素采用中值滤波，还是GGF
    if (count == ROI.rows * ROI.cols - 1) //-1 去除自身pix
    {
        cv::Mat tmp;
        cv::medianBlur(ROI, tmp, ROI.rows);
        result = tmp.at<cv::Vec3d>((ROI.rows - 1) / 2, (ROI.cols - 1) / 2);
    }
    else
        result = GGFp(ROI_tmp, sigma);

    return static_cast<cv::Vec3b>(result);
}
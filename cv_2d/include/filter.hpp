#ifndef FILTER_HPP
#define FILTER_HPP

#include <opencv2/core.hpp>

void GGFfilter(const cv::Mat &srcImg, cv::Mat &dstImg, cv::Mat &guideImg, const int r, const double sigma);
void WGGFfilter(const cv::Mat &srcImg, cv::Mat &dstImg, cv::Mat &guideImg, const int r, const double sigma, const double lambda);

#endif
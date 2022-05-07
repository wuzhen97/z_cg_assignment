#ifndef NOICE_HPP
#define NOICE_HPP

#include <opencv2/core.hpp>

void addGaussianNoise(cv::Mat &m, int mu, int sigma);

/*
 *  添加椒盐噪声
 *  随机生成 num 个白点
 */
void addSaltNoise(cv::Mat &img, const int num);

/*
 *  峰值信噪比(Peak Signal to Noise Ratio, PSNR)
 *  2个图像之间PSNR值越大，则越相似。普遍基准为30dB，30dB以下的图像劣化较为明显
 */
double getPSNR(const cv::Mat &I1, const cv::Mat &I2);

#endif
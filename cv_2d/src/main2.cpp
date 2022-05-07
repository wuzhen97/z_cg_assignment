#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "noice.hpp"
#include "filter.hpp"

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "usage: main settingFilePath 0/1 " << endl;
        return 1;
    }

    //---------------------------------------------配置-------------------------------------------//
    FileStorage fsSettings(argv[1], FileStorage::READ);
    if (!fsSettings.isOpened())
    {
        cerr << "Failed to open settings file at: " << argv[1] << endl;
        exit(-1);
    }

    int addNoise = fsSettings["io.addNoise"];
    // IO参数
    string iImagePath = fsSettings["io.iImagePath"];
    string oSeqGS = fsSettings["io.oSeqGS"];
    string oResultGS = fsSettings["io.oResultGS"];
    string oSeqGF = fsSettings["io.oSeqGF"];
    string oResultGF = fsSettings["io.oResultGF"];
    string oSeqGGF = fsSettings["io.oSeqGGF"];
    string oResultGGF = fsSettings["io.oResultGGF"];
    string oSeqWGGF = fsSettings["io.oSeqWGGF"];
    string oResultWGGF = fsSettings["io.oResultWGGF"];
    // 噪声参数
    int mu_gn = fsSettings["GaussianNoise.mu"];
    int sigma_gn = fsSettings["GaussianNoise.sigma"];
    string oPath_gn = fsSettings["GaussianNoise.oPath"];
    // 算法参数
    int minWin_gs = fsSettings["GS.minWin"];
    int maxWin_gs = fsSettings["GS.maxWin"];
    double minSigma_gs = fsSettings["GS.minSigma"];
    double maxSigma_gs = fsSettings["GS.maxSigma"];
    double stepSigma_gs = fsSettings["GS.stepSigma"];

    int minWin_gf = fsSettings["GF.minWin"];
    int maxWin_gf = fsSettings["GF.maxWin"];
    double minSigma_gf = fsSettings["GF.minSigma"];
    double maxSigma_gf = fsSettings["GF.maxSigma"];
    double stepSigma_gf = fsSettings["GF.stepSigma"];

    int minWin_ggf = fsSettings["GGF.minWin"];
    int maxWin_ggf = fsSettings["GGF.maxWin"];
    double minSigma_ggf = fsSettings["GGF.minSigma"];
    double maxSigma_ggf = fsSettings["GGF.maxSigma"];
    double stepSigma_ggf = fsSettings["GGF.stepSigma"];

    int win_ggf_gs = fsSettings["GGFGS.win"];
    double sigma_ggf_gs = fsSettings["GGFGS.sigma"];

    int minWin_wggf = fsSettings["WGGF.minWin"];
    int maxWin_wggf = fsSettings["WGGF.maxWin"];
    double minSigma_wggf = fsSettings["WGGF.minSigma"];
    double maxSigma_wggf = fsSettings["WGGF.maxSigma"];
    double stepSigma_wggf = fsSettings["WGGF.stepSigma"];

    int win_wggf_gs = fsSettings["WGGFGS.win"];
    double sigma_wggf_gs = fsSettings["WGGFGS.sigma"];

    double minLambda = fsSettings["WGGF.minLambda"];
    double maxLambda = fsSettings["WGGF.maxLambda"];
    double stepLambda = fsSettings["WGGF.stepLambda"];

    fsSettings.release();

    //————————————————————————————————————————下面才开始测试算法————————————————————————————————————————//
    //---------------------------------------------准备-------------------------------------------//
    // 读取图像
    Mat img = imread(iImagePath, IMREAD_COLOR);
    if (img.empty())
    {
        cerr << "Failed to read image at:" << iImagePath << endl;
        exit(-1);
    }

    // 添加高斯噪声
    Mat img_noise = img.clone();
    if (addNoise != 0)
    {
        addGaussianNoise(img_noise, mu_gn, sigma_gn);
    }
    imwrite(oPath_gn + "/img_noise.png", img_noise);
    double psnr_noise = getPSNR(img, img_noise);

    //---------------------------------------------GS-------------------------------------------//
    // 存储结果
    ofstream ofResult_gs(oResultGS);
    if (!ofResult_gs)
    {
        cerr << "Failed to load file at:" << oResultGS << endl;
        exit(-1);
    }
    ofResult_gs << "PSNR_noise: " << psnr_noise << endl;
    ofResult_gs << setw(6) << "window" << setw(6) << "sigma" << setw(8) << "PSNR" << endl;
    // 遍历滤波参数
    for (int i = minWin_gs; i <= maxWin_gs; i += 2)
    {
        cout << "gs-ing..." << endl;
        double sigma = minSigma_gs;
        while (sigma < maxSigma_gs)
        {
            Mat mask;
            Mat img_filter = img_noise.clone();
            GaussianBlur(img_noise, img_filter, Size(i, i), sigma);
            double psnr_filter = getPSNR(img, img_filter);
            ofResult_gs << setw(6) << i << setw(6) << sigma << setw(12) << psnr_filter << endl;
            string filename = oSeqGS + "/" + to_string(i) + "-" + to_string(sigma) + ".png";
            imwrite(filename, img_filter);
            sigma += stepSigma_gs;
        }
    }
    cout << "----gs-over----" << endl;
    ofResult_gs.close();

    //---------------------------------------------GF-------------------------------------------//
    // 存储结果
    ofstream ofResult_gf(oResultGF);
    if (!ofResult_gf)
    {
        cerr << "Failed to load file at:" << oResultGF << endl;
        exit(-1);
    }
    ofResult_gf << "PSNR_noise: " << psnr_noise << endl;
    ofResult_gf << setw(6) << "window" << setw(6) << "sigma" << setw(8) << "PSNR" << endl;
    // 遍历滤波参数
    for (int i = minWin_gf; i <= maxWin_gf; i += 2)
    {
        cout << "gf-ing..." << endl;
        double sigma = minSigma_gf;
        while (sigma < maxSigma_gf)
        {
            Mat img_filter;
            GGFfilter(img_noise, img_filter, img_noise, (i - 1) / 2, sigma); //以自身为引导图即为GF
            double psnr_filter = getPSNR(img, img_filter);
            ofResult_gf << setw(6) << i << setw(6) << sigma << setw(12) << psnr_filter << endl;
            string filename = oSeqGF + "/" + to_string(i) + "-" + to_string(sigma) + ".png";
            imwrite(filename, img_filter);
            sigma += stepSigma_gf;
        }
    }
    cout << "----gf-over----" << endl;
    ofResult_gf.close();

    //---------------------------------------------GGF-------------------------------------------//
    // 存储结果
    ofstream ofResult_ggf(oResultGGF);
    if (!ofResult_ggf)
    {
        cerr << "Failed to load file at:" << oResultGGF << endl;
        exit(-1);
    }
    ofResult_ggf << "PSNR_noise: " << psnr_noise << endl;
    ofResult_ggf << setw(6) << "window" << setw(6) << "sigma" << setw(8) << "PSNR" << endl;
    // GS
    Mat mask;
    Mat img_prefilter = img_noise.clone();
    GaussianBlur(img_noise, img_prefilter, Size(win_ggf_gs, win_ggf_gs), sigma_ggf_gs); // 生成引导图
    double psnr_prefilter = getPSNR(img, img_prefilter);
    cout << "PSNR_prefilter: " << psnr_prefilter << endl;
    // 遍历滤波参数
    for (int i = minWin_ggf; i <= maxWin_ggf; i += 2)
    {
        cout << "ggf-ing..." << endl;
        // GF
        double sigma = minSigma_ggf;
        while (sigma < maxSigma_ggf)
        {
            Mat img_filter;
            GGFfilter(img_noise, img_filter, img_prefilter, (i - 1) / 2, sigma); // 滤波
            double psnr_filter = getPSNR(img, img_filter);
            ofResult_ggf << setw(6) << i << setw(6) << sigma << setw(12) << psnr_filter << endl;
            string filename = oSeqGGF + "/" + to_string(i) + "-" + to_string(sigma) + ".png";
            imwrite(filename, img_filter);
            sigma += stepSigma_ggf;
        }
    }
    cout << "----ggf-over----" << endl;
    ofResult_ggf.close();

    //---------------------------------------------WGGF-------------------------------------------//
    // 存储结果
    ofstream ofResult_wggf(oResultWGGF);
    if (!ofResult_wggf)
    {
        cerr << "Failed to load file at:" << oResultWGGF << endl;
        exit(-1);
    }
    ofResult_wggf << "PSNR_noise: " << psnr_noise << endl;
    ofResult_wggf << setw(6) << "window" << setw(6) << "sigma" << setw(7) << "lambda" << setw(8) << "PSNR" << endl;
    // GS
    GaussianBlur(img_noise, img_prefilter, Size(win_ggf_gs, win_ggf_gs), sigma_ggf_gs); // 生成引导图
    psnr_prefilter = getPSNR(img, img_prefilter);
    cout << "PSNR_prefilter: " << psnr_prefilter << endl;
    // 遍历滤波参数
    for (int i = minWin_wggf; i <= maxWin_wggf; i += 2)
    {
        cout << "wggf-ing..." << endl;
        double sigma = minSigma_wggf;
        while (sigma < maxSigma_wggf)
        {
            Mat img_filter;
            double lambda = minLambda;
            while (lambda < maxLambda)
            {
                WGGFfilter(img_noise, img_filter, img_prefilter, (i - 1) / 2, sigma, lambda); // 滤波
                lambda += stepLambda;
                double psnr_filter = getPSNR(img, img_filter);
                ofResult_wggf << setw(6) << i << setw(6) << sigma << setw(7) << lambda << setw(12) << psnr_filter << endl;
                string filename = oSeqWGGF + "/" + to_string(i) + "-" + to_string(sigma) + "-" + to_string(lambda) + ".png ";
                imwrite(filename, img_filter);
            }

            sigma += stepSigma_wggf;
        }
    }
    cout << "----wggf-over----" << endl;
    ofResult_wggf.close();

    return 0;
}
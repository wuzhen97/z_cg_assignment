#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "filter.hpp"

using namespace cv;
int main()
{
    Mat boy, girl, building, dst_boy, dst_girl, dst_building;
    boy = imread("/home/wz/git_ws/cv_2d/img/boy.jpg");
    girl = imread("/home/wz/git_ws/cv_2d/img/girl.jpg");
    building = imread("/home/wz/git_ws/cv_2d/img/img_noise.png");

    GaussianBlur(boy, dst_boy, Size(3, 3), 0.7);
    imwrite("/home/wz/git_ws/cv_2d/img/boyGS.jpg", dst_boy);
    GaussianBlur(girl, dst_girl, Size(3, 3), 0.7);
    imwrite("/home/wz/git_ws/cv_2d/img/girlGS.jpg", dst_girl);
    GaussianBlur(building, dst_building, Size(3, 3), 0.7);
    imwrite("/home/wz/git_ws/cv_2d/img/buildingGS.jpg", dst_building);

    // boy
    Mat dst;
    GGFfilter(boy, dst, dst_boy, 5, 13.1);
    imwrite("/home/wz/git_ws/cv_2d/img/boyGGF.jpg", dst);

    WGGFfilter(boy, dst, dst_boy, 11, 21.1, 12);
    imwrite("/home/wz/git_ws/cv_2d/img/boyWGGF.jpg", dst);

    // girl
    GGFfilter(girl, dst, dst_girl, 5, 13.1);
    imwrite("/home/wz/git_ws/cv_2d/img/girlGGF.jpg", dst);

    WGGFfilter(girl, dst, dst_girl, 11, 21.1, 12);
    imwrite("/home/wz/git_ws/cv_2d/img/girlWGGF.jpg", dst);

    // building
    GGFfilter(building, dst, dst_building, 5, 13.1);
    imwrite("/home/wz/git_ws/cv_2d/img/buildingGGF.jpg", dst);

    WGGFfilter(building, dst, dst_building, 11, 21.1, 12);
    imwrite("/home/wz/git_ws/cv_2d/img/buildingWGGF.jpg", dst);
    return 0;
}
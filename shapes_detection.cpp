#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

#define COR std::vector<std::vector<cv::Point>> 

cv::Mat imgGray, imgDil, imgCanny, imgBlur;

int is_circle(COR contours, COR tempContours, int i)
{
    int         edgePointCount;
    int         lineCount;
    cv::Moments M;

    edgePointCount = tempContours[i].size();
    M = cv::moments(contours[i]);
    if (M.m00 == 0)
        return (1);
    cv::Point center(M.m10 / M.m00, M.m01 / M.m00);
    lineCount = 0;
    for (const cv::Point& pt : contours[i])
        lineCount++;
    if ((lineCount) > edgePointCount * 0.4)
        return (3);
    return(0);
}

int is_Square(std::vector<cv::Rect> boundingRect, int i)
{
    float aspRatio;

    aspRatio = (float)boundingRect[i].width / (float)boundingRect[i].height;
    if (aspRatio > 0.95 && aspRatio < 1.05)
        return (1);
    return (0);
}

int    which_shape(COR conPloy, std::vector<cv::Rect> boundingRect, int i, std::string& objType, 
                    COR contours, COR tempContours)
{
    int     objCor;
    int     ret;

    objCor = conPloy[i].size();
    if (objCor == 3)
        objType = "Triangle";
    else if (objCor == 4)
        if (is_Square(boundingRect, i))
            objType = "Square";
        else
            objType = "Rectangle";
    else if (objCor == 5)
        objType = "Pentagon";
    else if (objCor == 6)
        objType = "Hexagon";
    else if (objCor == 7) 
        objType = "Heptagon";
    else if (objCor > 7) 
    {
        ret = is_circle(contours, tempContours, i);
        if (ret == 1)
            return (1);
        if (ret == 3)
            objType = "Circle";
        else
            objType = "Polygon";
    }
    return (0);
}

void getContours(cv::Mat imgDil, cv::Mat img)
{
    COR contours;
    COR tempContours;
    std::string objType;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<cv::Vec4i> hierarchy_2;

    cv::findContours(imgDil, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    cv::findContours(imgDil, tempContours, hierarchy_2, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    COR conPloy(contours.size());
    std::vector<cv::Rect> boundingRect(contours.size());
    for(int i = 0; i < contours.size(); i++)
    {
        int area = cv::contourArea(contours[i]);
        if (area > 500)
        {
            float peri = cv::arcLength(contours[i], true);
            cv::approxPolyDP(contours[i], conPloy[i], 0.01 * peri, true);
            boundingRect[i] = cv::boundingRect(conPloy[i]);
            if (which_shape(conPloy, boundingRect, i, objType, contours, tempContours))
                continue ;
            cv::drawContours(img, conPloy, i, cv::Scalar(255, 0, 255), 2);
            cv::rectangle(img, boundingRect[i].tl(), boundingRect[i].br(), cv::Scalar(0, 255, 0), 2);
            cv::putText(img, objType, {boundingRect[i].x, boundingRect[i].y - 5}, 
                        cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 255, 0), 2);
        }
    }

}

void    image_process(cv::Mat &img)
{
    cv::resize(img, img, cv::Size(), 0.5, 0.5);
    std::cout<< img.size() << std::endl; 
    cv::cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(imgGray, imgBlur, cv::Size(3,3), 3, 0);
    cv::Canny(imgBlur, imgCanny, 15, 55);
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::dilate(imgCanny, imgDil, kernel);
}

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <image_path>" << std::endl;
        return (1);
    }
    std::string imagePath = argv[1];
    cv::Mat img = cv::imread(imagePath);
    image_process(img);
    getContours(imgDil, img);   
    cv::imshow("image Dil",  img);
    cv::waitKey(0);
    return (0);
}
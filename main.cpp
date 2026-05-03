#include "opencv2/videoio.hpp"
#include <filesystem>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

int totalFrames = 0;

void video2png(const std::string& path)
{
    if (!std::filesystem::exists(".cache")) {
        std::filesystem::create_directory(".cache");
    }

    std::cout << "开始处理视频文件：" << path << std::endl;
    cv::VideoCapture video(path);
    cv::Mat frame;
    int frameCount = 0;
    std::string* filename = new std::string;

    // 获取总帧数
    totalFrames = static_cast<int>(video.get(cv::CAP_PROP_FRAME_COUNT));

    while (video.read(frame)) {
        *filename = ".cache/frame_" + std::to_string(frameCount) + ".png";
        cv::imwrite(*filename, frame);
        frameCount++;

        std::clog << "\r处理进度: " << frameCount;
        if (totalFrames > 0) {
            std::clog << "/" << totalFrames;
        }
        std::clog << std::flush;
    };
    std::clog << std::endl
              << "处理完成，共 " << frameCount << " 帧" << std::endl;
    delete filename;
}

std::string image2text(const cv::Mat& img, float x, float y)
{
    cv::Mat gray, small;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::resize(gray, small, cv::Size(), x, y);
    std::string txt;

    for (int i = 0; i < small.rows; i++) {
        for (int j = 0; j < small.cols; j++)
            txt += small.at<uchar>(i, j) > 128 ? '@' : ' ';
        txt += '\n';
    }
    return txt;
}

int main(int argc, const char* argv[])
{
    video2png(argv[1]);

    cv::Mat img;
    for (int i = 1; i < totalFrames; i++) {
        img = cv::imread(".cache/frame_" + std::to_string(i) + ".png", cv::IMREAD_COLOR);
        std::cout << "\033[2J\033[H"; // 清屏并光标归位
        std::cout << image2text(img, std::stof(argv[2]), std::stof(argv[3])) << std::flush;
    }
    std::cout<<"清理中\n";
    std::filesystem::remove_all(".cache");
    std::cout<<"清理完毕\n";
}


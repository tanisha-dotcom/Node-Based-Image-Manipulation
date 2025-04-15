#pragma once
#include "../graph/Node.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

class BlurNode : public Node {
private:
    cv::Mat inputImage;
    cv::Mat outputImage;
    int radius = 3;
    bool directional = false;
    float angle = 0.0f;

    cv::Mat generateDirectionalKernel(int radius, float angle);
    cv::Mat generateGaussianKernel(int radius);

public:
    BlurNode(const std::string& name);
    void setInput(const cv::Mat& input) override;
    void process() override;
    void renderUI() override;
    cv::Mat getOutput() const override;
    void setRadius(int newRadius);
    void setAngle(float newAngle);
    void setDirectional(bool isDirectional);
};

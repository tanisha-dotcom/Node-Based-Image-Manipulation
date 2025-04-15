#pragma once
#include "../graph/Node.hpp"
#include <opencv2/opencv.hpp>
#include <imgui.h>

class BrightnessContrastNode : public Node {
private:
    cv::Mat inputImage;
    cv::Mat outputImage;
    double alpha = 1.0;
    int beta = 0;

public:
    BrightnessContrastNode(const std::string& name);
    BrightnessContrastNode(const std::string& name, double alpha, int beta);
    
    void setInput(const cv::Mat& input) override;
    void setParams(double contrast, int brightness);
    void process() override;
    void renderUI() override;
    cv::Mat getOutput() const override;
    void resetParams();
};

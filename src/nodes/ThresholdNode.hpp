#pragma once
#include "../graph/Node.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

class ThresholdNode : public Node {
private:
    cv::Mat inputImage;
    cv::Mat outputImage;
    int thresholdValue = 128; 
    int maxThresholdValue = 255;  
    int blockSize = 11;  
    int C = 2;

    enum ThresholdType {
        BINARY,
        ADAPTIVE,
        OTSU
    };
    ThresholdType thresholdType = BINARY; 

public:
    ThresholdNode(const std::string& name);

    void setInput(const cv::Mat& input) override;

    void process() override;

    void renderUI() override;

    cv::Mat getOutput() const override;

    void setThresholdType(ThresholdType type);

    void setThresholdValue(int value);

    void setBlockSize(int size);

    void setC(int constant);
};

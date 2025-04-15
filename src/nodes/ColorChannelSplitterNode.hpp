#pragma once
#include "../graph/Node.hpp"
#include <opencv2/opencv.hpp>
#include <vector>

class ColorChannelSplitterNode : public Node {
public:
    ColorChannelSplitterNode(const std::string& name, bool outputGrayscale = false);

    void setInput(const cv::Mat& input) override;
    void process() override;
    void renderUI() override;
    cv::Mat getOutput() const override;
    cv::Mat mergeChannels();

    void setOutputGrayscale(bool enable);
    void resetParams();
    cv::Mat inputImage; 
    cv::Mat redChannel;
    cv::Mat greenChannel;
    cv::Mat blueChannel;
    cv::Mat alphaChannel;  
    bool outputGrayscale; 
};

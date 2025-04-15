#pragma once
#include "../graph/Node.hpp"
#include <opencv2/opencv.hpp>

class ImageInputNode : public Node {
public:
    ImageInputNode(const std::string& name, const std::string& filePath);

    void process() override;

    void convertToGrayscale();
    

    void setOutput(const cv::Mat& newOutput);

    cv::Mat getOutput() const override;

    void renderUI() override;

private:
    std::string name;
    std::string filePath;
    cv::Mat input; 
    cv::Mat output; 
};

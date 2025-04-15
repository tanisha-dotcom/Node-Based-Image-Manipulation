#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>
#include <memory>
#include "../graph/Node.hpp" 

class OutputNode : public Node {
private:
    cv::Mat inputImage;
    std::string savePath;
    std::string type;
    int quality;  

public:
    OutputNode(const std::string& name, const std::string& path, const std::string& type, int quality = 95);
    void setInput(const cv::Mat& input) override;
    void process() override;
    void renderUI() override;
    cv::Mat getOutput() const override;
};

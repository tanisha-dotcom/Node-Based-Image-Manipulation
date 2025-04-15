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
    int quality = 95;  // Default quality

public:
    // Constructor
    OutputNode(const std::string& name, const std::string& path, const std::string& type, int quality = 95);

    // Sets the input image for this node
    void setInput(const cv::Mat& input) override;

    // Processes the input and saves the image
    void process() override;

    // Renders the UI using ImGui
    void renderUI() override;

    // Gets the output (same as input for this node)
    cv::Mat getOutput() const override;

    // Sets the file type (e.g., jpg, png)
    void settype(const std::string& type);
};

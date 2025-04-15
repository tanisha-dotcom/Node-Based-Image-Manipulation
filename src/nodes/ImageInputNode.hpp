#pragma once

#include "../graph/Node.hpp"
#include <opencv2/opencv.hpp>
#include <string>

class ImageInputNode : public Node {
public:
    // Constructor: Takes a name and path to the image file
    ImageInputNode(const std::string& name, const std::string& filePath);

    // Load image and prepare for processing
    void process() override;

    // Convert current input image to grayscale and store in output
    void convertToGrayscale();

    // Manually set the output image (optional override)
    void setOutput(const cv::Mat& newOutput);

    // Retrieve the output image (used by downstream nodes)
    cv::Mat getOutput() const override;

    // Render GUI for this node (e.g. ImGui controls)
    void renderUI() override;

private:
    std::string name;        // Node's display name
    std::string filePath;    // Path to input image file
    cv::Mat input;           // Original loaded image
    cv::Mat output;          // Processed or modified image
};

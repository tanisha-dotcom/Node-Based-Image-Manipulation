#pragma once
#include "../graph/Node.hpp"  // Base class Node is included to inherit from it
#include <opencv2/opencv.hpp>  // OpenCV library for image processing
#include <imgui.h>  // ImGui for rendering user interface

// BrightnessContrastNode class: Inherits from Node, handles image brightness and contrast adjustments
class BrightnessContrastNode : public Node {
private:
    cv::Mat inputImage;   // Stores the input image
    cv::Mat outputImage;  // Stores the output image after processing
    double alpha = 1.0;   // Contrast factor (default: no contrast change)
    int beta = 0;         // Brightness offset (default: no brightness change)

public:
    // Constructor: Initializes the node with a name and sets default values for alpha and beta
    BrightnessContrastNode(const std::string& name);

    // Overloaded constructor: Initializes the node with a name, contrast (alpha), and brightness (beta)
    BrightnessContrastNode(const std::string& name, double alpha, int beta);
    
    // Set input image for processing
    void setInput(const cv::Mat& input) override;

    // Set custom contrast and brightness parameters
    void setParams(double contrast, int brightness);

    // Apply the contrast and brightness adjustments to the input image
    void process() override;

    // Render the user interface for adjusting contrast and brightness
    void renderUI() override;

    // Get the output image after applying brightness and contrast adjustments
    cv::Mat getOutput() const override;

    // Reset the contrast and brightness parameters to their default values
    void resetParams();
};

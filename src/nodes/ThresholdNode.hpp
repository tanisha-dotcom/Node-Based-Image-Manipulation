#pragma once
#include "../graph/Node.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

// Class representing a ThresholdNode in an image processing graph
class ThresholdNode : public Node {
public:
    // Input image (to be processed)
    cv::Mat inputImage;

    // Output image (processed after thresholding)
    cv::Mat outputImage;

    // Thresholding parameters
    int thresholdValue = 128; // Default threshold value for binary thresholding
    int maxThresholdValue = 255;  // Maximum threshold value (for binary and OTSU)
    int blockSize = 11;  // Block size for adaptive thresholding (should be odd)
    int C = 2; // Constant for adaptive thresholding (to adjust the result)

    // Enum representing the type of thresholding to apply
    enum ThresholdType {
        BINARY,    // Binary thresholding
        ADAPTIVE,  // Adaptive thresholding
        OTSU       // Otsu's thresholding
    };

    // Default thresholding method (set to Binary by default)
    ThresholdType thresholdType = BINARY; 

    // Constructor to initialize the node with a name
    ThresholdNode(const std::string& name);

    // Set the input image for processing
    void setInput(const cv::Mat& input) override;

    // Apply the chosen thresholding method to the input image
    void process() override;

    // Render the user interface (UI) for controlling thresholding settings
    void renderUI() override;

    // Get the processed output image
    cv::Mat getOutput() const override;

    // Set the thresholding type (BINARY, ADAPTIVE, or OTSU)
    void setThresholdType(ThresholdType type);

    // Set the threshold value (only used in binary thresholding)
    void setThresholdValue(int value);

    // Set the block size (only used in adaptive thresholding, must be odd)
    void setBlockSize(int size);

    // Set the constant (C) for adaptive thresholding
    void setC(int constant);
};

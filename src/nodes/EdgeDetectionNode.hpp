#pragma once
#include "../graph/Node.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

// EdgeDetectionNode performs either Sobel or Canny edge detection
class EdgeDetectionNode : public Node
{
private:
    cv::Mat inputImage;     // Input image
    cv::Mat outputImage;    // Processed output image
    bool overlayEdges = false; // If true, overlays edges on original image

    // Parameters for edge detection
    int sobelKernelSize = 3;        // Kernel size for Sobel (must be odd)
    int cannyThreshold1 = 50;       // Lower threshold for Canny
    int cannyThreshold2 = 150;      // Upper threshold for Canny

public:
    // Enum to switch between Sobel and Canny algorithms
    enum EdgeDetectionType
    {
        SOBEL,
        CANNY
    };
    EdgeDetectionType edgeDetectionType = SOBEL;

    // Constructor with node name
    EdgeDetectionNode(const std::string &name);

    // Set input image to be processed
    void setInput(const cv::Mat &input) override;

    // Perform edge detection based on selected method
    void process() override;

    // Display ImGui controls for interactive adjustment
    void renderUI() override;

    // Retrieve processed image
    cv::Mat getOutput() const override;

    // Manual configuration methods
    void setEdgeDetectionType(EdgeDetectionType type);
    void setSobelKernelSize(int size);
    void setCannyThresholds(int threshold1, int threshold2);
    void setOverlayEdges(bool overlay);
};

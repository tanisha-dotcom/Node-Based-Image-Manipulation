#include "EdgeDetectionNode.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <imgui.h>

// Constructor: sets the node's display name and generates a unique ID
EdgeDetectionNode::EdgeDetectionNode(const std::string &name)
{
    this->name = name;
    this->id = "edge_detection_" + name;
}

// Accepts the input image to be processed
void EdgeDetectionNode::setInput(const cv::Mat &input)
{
    inputImage = input;
}

// Main processing function to apply edge detection
void EdgeDetectionNode::process()
{
    if (inputImage.empty())
    {
        std::cerr << "No input image for EdgeDetectionNode: " << name << std::endl;
        return;
    }

    // Keep a copy of the original for optional overlay
    cv::Mat originalColor = inputImage.clone();

    // Convert to grayscale if needed
    cv::Mat grayImage;
    if (inputImage.channels() != 1)
    {
        cv::cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);
    }
    else
    {
        grayImage = inputImage;
    }

    // Apply edge detection based on selected method
    cv::Mat edges;
    if (edgeDetectionType == SOBEL)
    {
        cv::Sobel(grayImage, edges, CV_8U, 1, 1, sobelKernelSize); // Apply Sobel operator
    }
    else if (edgeDetectionType == CANNY)
    {
        cv::Canny(grayImage, edges, cannyThreshold1, cannyThreshold2); // Apply Canny algorithm
    }

    // If overlay is enabled, mix original color image with edge map
    if (overlayEdges)
    {
        cv::Mat colorEdges;
        cv::cvtColor(edges, colorEdges, cv::COLOR_GRAY2BGR);
        cv::addWeighted(originalColor, 0.7, colorEdges, 0.3, 0, outputImage);
    }
    else
    {
        outputImage = edges;
    }

    if (outputImage.empty())
    {
        std::cerr << "Failed to apply edge detection." << std::endl;
    }
}

// Renders the ImGui UI for selecting edge detection type and parameters
void EdgeDetectionNode::renderUI()
{
    std::cout << "[EdgeDetectionNode: " << name << "]" << std::endl;

    // Select between Sobel and Canny
    if (ImGui::RadioButton("Sobel", edgeDetectionType == SOBEL))
    {
        edgeDetectionType = SOBEL;
        process();
    }
    if (ImGui::RadioButton("Canny", edgeDetectionType == CANNY))
    {
        edgeDetectionType = CANNY;
        process();
    }

    // Adjustable parameter: kernel size for Sobel
    if (edgeDetectionType == SOBEL)
    {
        if (ImGui::SliderInt("Sobel Kernel Size", &sobelKernelSize, 1, 7))
        {
            process();
        }
    }

    // Adjustable parameters: thresholds for Canny
    if (edgeDetectionType == CANNY)
    {
        if (ImGui::SliderInt("Canny Threshold 1", &cannyThreshold1, 0, 255))
        {
            process();
        }
        if (ImGui::SliderInt("Canny Threshold 2", &cannyThreshold2, 0, 255))
        {
            process();
        }
    }

    // Option to overlay edges on original image
    if (ImGui::Checkbox("Overlay Edges", &overlayEdges))
    {
        process();
    }
}

// Returns the result image after edge detection
cv::Mat EdgeDetectionNode::getOutput() const
{
    return outputImage;
}

// Manual setters to change settings programmatically
void EdgeDetectionNode::setEdgeDetectionType(EdgeDetectionType type)
{
    edgeDetectionType = type;
    process();
}

void EdgeDetectionNode::setSobelKernelSize(int size)
{
    sobelKernelSize = size;
    process();
}

void EdgeDetectionNode::setCannyThresholds(int threshold1, int threshold2)
{
    cannyThreshold1 = threshold1;
    cannyThreshold2 = threshold2;
    process();
}

void EdgeDetectionNode::setOverlayEdges(bool overlay)
{
    overlayEdges = overlay;
    process();
}

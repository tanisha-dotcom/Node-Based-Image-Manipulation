#include "ImageInputNode.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

// Constructor initializes name and file path
ImageInputNode::ImageInputNode(const std::string& name, const std::string& filePath)
    : Node(), filePath(filePath), name(name) {}

// Load image from disk and prepare it for pipeline
void ImageInputNode::process() {
    input = cv::imread(filePath);  // Load image using OpenCV

    if (input.empty()) {
        std::cerr << "❌ Failed to load image: " << filePath << std::endl;
    } else {
        output = input.clone();  // Copy input to output for downstream nodes
    }
}

// Allow external override of output (optional feature)
void ImageInputNode::setOutput(const cv::Mat& newOutput) {
    output = newOutput;  
}

// Return the current output (original or modified)
cv::Mat ImageInputNode::getOutput() const {
    return output;
}

// Render ImGui UI (optional future use)
void ImageInputNode::renderUI() {
    std::cout << "🖼️ Rendering UI for Image Input Node: " << name << std::endl;
}

// Convert loaded image to grayscale
void ImageInputNode::convertToGrayscale() {
    if (!input.empty()) {
        cv::cvtColor(input, output, cv::COLOR_BGR2GRAY);
        std::cout << "✅ Image converted to grayscale." << std::endl;
    } else {
        std::cout << "⚠️ No image loaded to convert to grayscale." << std::endl;
    }
}

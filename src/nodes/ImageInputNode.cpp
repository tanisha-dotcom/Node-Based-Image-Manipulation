#include "ImageInputNode.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

ImageInputNode::ImageInputNode(const std::string& name, const std::string& filePath)
    : Node(), filePath(filePath), name(name) {}

void ImageInputNode::process() {
    input = cv::imread(filePath);
    if (input.empty()) {
        std::cerr << "Failed to load image: " << filePath << std::endl;
    } else {
        output = input.clone(); 
    }
}

void ImageInputNode::setOutput(const cv::Mat& newOutput) {
    output = newOutput;  
}

cv::Mat ImageInputNode::getOutput() const {
    return output;
}

void ImageInputNode::renderUI() {
    std::cout << "Rendering UI for Image Input Node: " << name << std::endl;
}

void ImageInputNode::convertToGrayscale() {
    if (!input.empty()) {
        cv::cvtColor(input, output, cv::COLOR_BGR2GRAY);
        std::cout << "Image converted to grayscale." << std::endl;
    } else {
        std::cout << "No image loaded to convert to grayscale." << std::endl;
    }
}

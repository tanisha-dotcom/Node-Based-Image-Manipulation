#include "EdgeDetectionNode.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <imgui.h>

EdgeDetectionNode::EdgeDetectionNode(const std::string& name) {
    this->name = name;
    this->id = "edge_detection_" + name;
}

void EdgeDetectionNode::setInput(const cv::Mat& input) {
    inputImage = input;
}

void EdgeDetectionNode::process() {
    if (inputImage.empty()) {
        std::cerr << "No input image for EdgeDetectionNode: " << name << std::endl;
        return;
    }

    cv::Mat edges;

    if (inputImage.channels() != 1) {
        cv::cvtColor(inputImage, inputImage, cv::COLOR_BGR2GRAY);
    }

    if (edgeDetectionType == SOBEL) {
        cv::Sobel(inputImage, edges, CV_8U, 1, 1, sobelKernelSize);
    }
    else if (edgeDetectionType == CANNY) {
        cv::Canny(inputImage, edges, cannyThreshold1, cannyThreshold2);
    }

    if (overlayEdges) {
        cv::Mat colorEdges;
        cv::cvtColor(edges, colorEdges, cv::COLOR_GRAY2BGR); 
        cv::addWeighted(inputImage, 0.7, colorEdges, 0.3, 0, outputImage);
    } else {
        outputImage = edges;
    }

    if (outputImage.empty()) {
        std::cerr << "Failed to apply edge detection." << std::endl;
    }
}

void EdgeDetectionNode::renderUI() {
    std::cout << "[EdgeDetectionNode: " << name << "]" << std::endl;

    if (ImGui::RadioButton("Sobel", edgeDetectionType == SOBEL)) {
        edgeDetectionType = SOBEL;
        process();
    }
    if (ImGui::RadioButton("Canny", edgeDetectionType == CANNY)) {
        edgeDetectionType = CANNY;
        process();
    }

    if (edgeDetectionType == SOBEL) {
        if (ImGui::SliderInt("Sobel Kernel Size", &sobelKernelSize, 1, 7)) {
            process();  
        }
    }

    if (edgeDetectionType == CANNY) {
        if (ImGui::SliderInt("Canny Threshold 1", &cannyThreshold1, 0, 255)) {
            process();  
        }
        if (ImGui::SliderInt("Canny Threshold 2", &cannyThreshold2, 0, 255)) {
            process(); 
        }
    }

    if (ImGui::Checkbox("Overlay Edges", &overlayEdges)) {
        process();  
    }
}

cv::Mat EdgeDetectionNode::getOutput() const {
    return outputImage;
}

void EdgeDetectionNode::setEdgeDetectionType(EdgeDetectionType type) {
    edgeDetectionType = type;
    process();
}

void EdgeDetectionNode::setSobelKernelSize(int size) {
    sobelKernelSize = size;
    process();
}

void EdgeDetectionNode::setCannyThresholds(int threshold1, int threshold2) {
    cannyThreshold1 = threshold1;
    cannyThreshold2 = threshold2;
    process();
}

void EdgeDetectionNode::setOverlayEdges(bool overlay) {
    overlayEdges = overlay;
    process();
}

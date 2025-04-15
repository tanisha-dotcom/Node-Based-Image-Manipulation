#include "ThresholdNode.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <imgui.h>
#include <algorithm>  

ThresholdNode::ThresholdNode(const std::string& name) {
    this->name = name;
    this->id = "threshold_" + name;
}

void ThresholdNode::setInput(const cv::Mat& input) {
    inputImage = input;
}

void ThresholdNode::process() {
    if (inputImage.empty()) {
        std::cerr << "No input image for ThresholdNode: " << name << std::endl;
        return;
    }

    if (inputImage.channels() != 1) {
        cv::cvtColor(inputImage, inputImage, cv::COLOR_BGR2GRAY);
    }

    switch (thresholdType) {
        case BINARY:
            cv::threshold(inputImage, outputImage, thresholdValue, maxThresholdValue, cv::THRESH_BINARY);
            break;
        case ADAPTIVE:
            cv::adaptiveThreshold(inputImage, outputImage, maxThresholdValue, cv::ADAPTIVE_THRESH_MEAN_C,
                                  cv::THRESH_BINARY, blockSize, C);
            break;
        case OTSU:
            cv::threshold(inputImage, outputImage, 0, maxThresholdValue, cv::THRESH_BINARY | cv::THRESH_OTSU);
            break;
        default:
            std::cerr << "Unknown thresholding method!" << std::endl;
            break;
    }

    if (outputImage.empty()) {
        std::cerr << "Failed to apply thresholding." << std::endl;
    } else {
        std::cout << "Threshold applied using " << (thresholdType == BINARY ? "Binary" : 
                                                     (thresholdType == ADAPTIVE ? "Adaptive" : "Otsu"))
                  << " method." << std::endl;
    }
}

void ThresholdNode::renderUI() {
    std::cout << "[ThresholdNode: " << name << "]" << std::endl;

    if (ImGui::RadioButton("Binary", thresholdType == BINARY)) {
        thresholdType = BINARY;
        process();
    }
    if (ImGui::RadioButton("Adaptive", thresholdType == ADAPTIVE)) {
        thresholdType = ADAPTIVE;
        process();
    }
    if (ImGui::RadioButton("Otsu", thresholdType == OTSU)) {
        thresholdType = OTSU;
        process();
    }

    if (thresholdType == BINARY) {
        if (ImGui::SliderInt("Threshold Value", &thresholdValue, 0, maxThresholdValue)) {
            process(); 
        }
    }

    if (thresholdType == ADAPTIVE) {
        if (ImGui::SliderInt("Block Size", &blockSize, 3, 21)) {
            if (blockSize % 2 == 0) blockSize++; 
            process();  
        }
        if (ImGui::SliderInt("C Constant", &C, 1, 10)) {
            process(); 
        }
    }

    if (!inputImage.empty()) {
        std::vector<int> histogram(256, 0);
        for (int i = 0; i < inputImage.rows; i++) {
            for (int j = 0; j < inputImage.cols; j++) {
                histogram[inputImage.at<uchar>(i, j)]++;
            }
        }

        std::vector<float> histogramFloat(histogram.begin(), histogram.end());

        ImGui::Text("Histogram");
        float maxVal = *std::max_element(histogramFloat.begin(), histogramFloat.end());
        ImGui::PlotHistogram("##Histogram", histogramFloat.data(), 256, 0, nullptr, 0.0f, maxVal, ImVec2(400, 150));
    }
}

cv::Mat ThresholdNode::getOutput() const {
    return outputImage;
}

void ThresholdNode::setThresholdType(ThresholdType type) {
    thresholdType = type;
    process();
}

void ThresholdNode::setThresholdValue(int value) {
    thresholdValue = value;
    process();
}

void ThresholdNode::setBlockSize(int size) {
    blockSize = size;
    process();
}

void ThresholdNode::setC(int constant) {
    C = constant;
    process();
}

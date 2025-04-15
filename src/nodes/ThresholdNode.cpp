#include "ThresholdNode.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <imgui.h>
#include <algorithm>  

// Constructor initializes the node with a given name
ThresholdNode::ThresholdNode(const std::string& name) {
    this->name = name;
    this->id = "threshold_" + name;
}

// Sets the input image for processing
void ThresholdNode::setInput(const cv::Mat& input) {
    inputImage = input;
}

// Processes the input image based on the selected thresholding method
void ThresholdNode::process() {
    if (inputImage.empty()) { // Check if input image is empty
        std::cerr << "No input image for ThresholdNode: " << name << std::endl;
        return;
    }

    // Convert the image to grayscale if it's not already
    if (inputImage.channels() != 1) {
        cv::cvtColor(inputImage, inputImage, cv::COLOR_BGR2GRAY);
    }

    // Apply the selected thresholding method
    switch (thresholdType) {
        case BINARY:
            // Apply binary thresholding
            cv::threshold(inputImage, outputImage, thresholdValue, maxThresholdValue, cv::THRESH_BINARY);
            break;
        case ADAPTIVE:
            // Apply adaptive thresholding
            cv::adaptiveThreshold(inputImage, outputImage, maxThresholdValue, cv::ADAPTIVE_THRESH_MEAN_C,
                                  cv::THRESH_BINARY, blockSize, C);
            break;
        case OTSU:
            // Apply Otsu's thresholding
            cv::threshold(inputImage, outputImage, 0, maxThresholdValue, cv::THRESH_BINARY | cv::THRESH_OTSU);
            break;
        default:
            // Handle invalid thresholding type
            std::cerr << "Unknown thresholding method!" << std::endl;
            break;
    }

    // Check if thresholding was successful
    if (outputImage.empty()) {
        std::cerr << "Failed to apply thresholding." << std::endl;
    } else {
        // Log the applied method
        std::cout << "Threshold applied using " << (thresholdType == BINARY ? "Binary" : 
                                                     (thresholdType == ADAPTIVE ? "Adaptive" : "Otsu"))
                  << " method." << std::endl;
    }
}

// Renders the user interface for thresholding settings
void ThresholdNode::renderUI() {
    std::cout << "[ThresholdNode: " << name << "]" << std::endl;

    // Radio buttons to select thresholding method
    if (ImGui::RadioButton("Binary", thresholdType == BINARY)) {
        thresholdType = BINARY;
        process(); // Reprocess image when method is changed
    }
    if (ImGui::RadioButton("Adaptive", thresholdType == ADAPTIVE)) {
        thresholdType = ADAPTIVE;
        process();
    }
    if (ImGui::RadioButton("Otsu", thresholdType == OTSU)) {
        thresholdType = OTSU;
        process();
    }

    // Show additional UI for binary thresholding
    if (thresholdType == BINARY) {
        if (ImGui::SliderInt("Threshold Value", &thresholdValue, 0, maxThresholdValue)) {
            process(); // Reprocess when threshold value is changed
        }
    }

    // Show additional UI for adaptive thresholding
    if (thresholdType == ADAPTIVE) {
        // Slider for block size, ensures it is an odd number
        if (ImGui::SliderInt("Block Size", &blockSize, 3, 21)) {
            if (blockSize % 2 == 0) blockSize++; // Ensure odd block size
            process();
        }
        if (ImGui::SliderInt("C Constant", &C, 1, 10)) {
            process(); // Reprocess when constant is changed
        }
    }

    // Display histogram of the input image
    if (!inputImage.empty()) {
        std::vector<int> histogram(256, 0);
        // Calculate histogram values
        for (int i = 0; i < inputImage.rows; i++) {
            for (int j = 0; j < inputImage.cols; j++) {
                histogram[inputImage.at<uchar>(i, j)]++;
            }
        }

        // Convert histogram to float for display
        std::vector<float> histogramFloat(histogram.begin(), histogram.end());

        // Display histogram in the UI
        ImGui::Text("Histogram");
        float maxVal = *std::max_element(histogramFloat.begin(), histogramFloat.end());
        ImGui::PlotHistogram("##Histogram", histogramFloat.data(), 256, 0, nullptr, 0.0f, maxVal, ImVec2(400, 150));
    }
}

// Returns the processed output image
cv::Mat ThresholdNode::getOutput() const {
    return outputImage;
}

// Setter for threshold type (e.g., Binary, Adaptive, Otsu)
void ThresholdNode::setThresholdType(ThresholdType type) {
    thresholdType = type;
    process(); // Reprocess when threshold type is changed
}

// Setter for threshold value (used in binary thresholding)
void ThresholdNode::setThresholdValue(int value) {
    thresholdValue = value;
    process(); // Reprocess when threshold value is changed
}

// Setter for block size (used in adaptive thresholding)
void ThresholdNode::setBlockSize(int size) {
    blockSize = size;
    process(); // Reprocess when block size is changed
}

// Setter for the C constant (used in adaptive thresholding)
void ThresholdNode::setC(int constant) {
    C = constant;
    process(); // Reprocess when C constant is changed
}

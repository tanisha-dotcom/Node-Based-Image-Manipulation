#include "BrightnessContrastNode.hpp"
#include <iostream>

BrightnessContrastNode::BrightnessContrastNode(const std::string& name) {
    this->name = name;
    this->id = "brightness_contrast_" + name;
}

BrightnessContrastNode::BrightnessContrastNode(const std::string& name, double alpha, int beta) {
    this->name = name;
    this->id = "bc_" + name;
    this->alpha = alpha;
    this->beta = beta;
}

void BrightnessContrastNode::setInput(const cv::Mat& input) {
    inputImage = input;
}

void BrightnessContrastNode::setParams(double contrast, int brightness) {
    this->alpha = contrast;
    this->beta = brightness;
}

void BrightnessContrastNode::resetParams() {
    this->alpha = 1.0;
    this->beta = 0;
    std::cout << "Reset parameters to default: α = " << alpha << ", β = " << beta << std::endl;
}

void BrightnessContrastNode::process() {
    if (inputImage.empty()) {
        std::cerr << "No input image for BrightnessContrastNode: " << name << std::endl;
        return;
    }

    inputImage.convertTo(outputImage, -1, alpha, beta);
    std::cout << "Applied Brightness/Contrast to: " << name << std::endl;
}

void BrightnessContrastNode::renderUI() {
    std::cout << "[BrightnessContrastNode: " << name 
              << "] α = " << alpha << ", β = " << beta << std::endl;

    float alphaFloat = static_cast<float>(alpha);
    if (ImGui::SliderFloat("Contrast (α)", &alphaFloat, 0.0f, 3.0f)) {
        alpha = static_cast<double>(alphaFloat);
    }

    if (ImGui::SliderInt("Brightness (β)", &beta, -100, 100)) {
    }

    if (ImGui::Button("Reset Parameters")) {
        resetParams();
        alphaFloat = static_cast<float>(alpha);
    }
}

cv::Mat BrightnessContrastNode::getOutput() const {
    return outputImage;
}

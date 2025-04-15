#include "BrightnessContrastNode.hpp"
#include <iostream>

// Constructor initializing the name and unique id for the node
BrightnessContrastNode::BrightnessContrastNode(const std::string& name) {
    this->name = name;
    this->id = "brightness_contrast_" + name;
}

// Overloaded constructor initializing name, alpha (contrast), and beta (brightness)
BrightnessContrastNode::BrightnessContrastNode(const std::string& name, double alpha, int beta) {
    this->name = name;
    this->id = "bc_" + name;
    this->alpha = alpha;  // Set contrast (alpha)
    this->beta = beta;    // Set brightness (beta)
}

// Method to set the input image for processing
void BrightnessContrastNode::setInput(const cv::Mat& input) {
    inputImage = input;
}

// Method to set new contrast (alpha) and brightness (beta) values
void BrightnessContrastNode::setParams(double contrast, int brightness) {
    this->alpha = contrast;  // Set new contrast value
    this->beta = brightness; // Set new brightness value
}

// Method to reset the parameters to default values: α = 1.0 (no contrast change), β = 0 (no brightness change)
void BrightnessContrastNode::resetParams() {
    this->alpha = 1.0;  // Default contrast is 1 (no change)
    this->beta = 0;     // Default brightness is 0 (no change)
    std::cout << "Reset parameters to default: α = " << alpha << ", β = " << beta << std::endl;
}

// Method to process the input image by applying brightness and contrast adjustments
void BrightnessContrastNode::process() {
    // Check if the input image is empty
    if (inputImage.empty()) {
        std::cerr << "No input image for BrightnessContrastNode: " << name << std::endl;
        return;
    }
    
    // Apply contrast and brightness using OpenCV's convertTo method
    inputImage.convertTo(outputImage, -1, alpha, beta);
    std::cout << "Applied Brightness/Contrast to: " << name << std::endl;
}

// Method to render the user interface for adjusting contrast (alpha) and brightness (beta) values
void BrightnessContrastNode::renderUI() {
    // Log the current contrast and brightness values to the console
    std::cout << "[BrightnessContrastNode: " << name 
              << "] α = " << alpha << ", β = " << beta << std::endl;

    // Convert alpha to a float for the slider widget
    float alphaFloat = static_cast<float>(alpha);

    // Render a slider for adjusting the contrast (α)
    if (ImGui::SliderFloat("Contrast (α)", &alphaFloat, 0.0f, 3.0f)) {
        alpha = static_cast<double>(alphaFloat);  // Update alpha with the new value from the slider
    }

    // Render a slider for adjusting the brightness (β)
    if (ImGui::SliderInt("Brightness (β)", &beta, -100, 100)) {
        // Nothing needed here since the slider directly updates the beta value
    }

    // Render a button to reset the contrast and brightness parameters to their defaults
    if (ImGui::Button("Reset Parameters")) {
        resetParams();  // Reset the parameters
        alphaFloat = static_cast<float>(alpha);  // Update the slider value for alpha
    }
}

// Method to get the processed (output) image after applying brightness and contrast
cv::Mat BrightnessContrastNode::getOutput() const {
    return outputImage;  // Return the output image
}

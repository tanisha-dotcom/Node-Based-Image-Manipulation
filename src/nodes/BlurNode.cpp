#include "BlurNode.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <imgui.h>

#define PI 3.14159265358979323846  // Define Pi for angle calculations

// Constructor to initialize the BlurNode with a name and unique ID
BlurNode::BlurNode(const std::string& name) {
    this->name = name;
    this->id = "blur_" + name;  // Generate a unique ID for this BlurNode
}

// Set the input image for the BlurNode
void BlurNode::setInput(const cv::Mat& input) {
    inputImage = input;  // Store the input image for processing
}

// Generate a directional kernel based on a given radius and angle in degrees
cv::Mat BlurNode::generateDirectionalKernel(int radius, float angleDegrees) {
    int size = radius * 2 + 1;  // The size of the kernel is based on the radius
    cv::Mat kernel = cv::Mat::zeros(size, size, CV_32F);  // Create a kernel filled with zeros

    // Convert the angle from degrees to radians
    float angleRad = angleDegrees * PI / 180.0f;
    float dx = cos(angleRad);  // Calculate the directional x offset based on the angle
    float dy = sin(angleRad);  // Calculate the directional y offset based on the angle

    // Normalize the directional vector
    float length = std::sqrt(dx * dx + dy * dy);
    dx /= length;
    dy /= length;

    float sum = 0.0f;
    // Generate the kernel by stepping in the direction of the angle
    for (int i = -radius; i <= radius; ++i) {
        float x = radius + i * dx;  // Calculate the x position for each step
        float y = radius + i * dy;  // Calculate the y position for each step

        int ix = static_cast<int>(std::round(x));  // Round the calculated positions to nearest integers
        int iy = static_cast<int>(std::round(y));

        // Only set kernel values inside the valid bounds of the kernel
        if (ix >= 0 && ix < size && iy >= 0 && iy < size) {
            kernel.at<float>(iy, ix) = 1.0f;  // Set the kernel value at the computed position
            sum += 1.0f;  // Track the sum of all kernel values
        }
    }

    kernel /= sum;  // Normalize the kernel so that the sum of all values equals 1
    return kernel;
}

// Generate a Gaussian kernel based on the given radius
cv::Mat BlurNode::generateGaussianKernel(int radius) {
    int size = 2 * radius + 1;  // Kernel size based on the radius
    cv::Mat kernel(size, size, CV_32F);  // Create an empty kernel matrix

    // Sigma value for Gaussian kernel, often set as radius/3 for reasonable blur
    float sigma = radius / 3.0f;
    float sum = 0.0f;
    // Calculate Gaussian values for each pixel in the kernel
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            kernel.at<float>(y + radius, x + radius) = std::exp(-(x * x + y * y) / (2 * sigma * sigma));  // Gaussian formula
            sum += kernel.at<float>(y + radius, x + radius);  // Sum of all kernel values
        }
    }

    kernel /= sum;  // Normalize the kernel to ensure the sum equals 1
    return kernel;
}

// Apply the blur effect to the input image using the selected kernel
void BlurNode::process() {
    // Check if the input image is valid
    if (inputImage.empty()) {
        std::cerr << "No input image for BlurNode: " << name << std::endl;
        return;
    }

    // Select the appropriate kernel depending on whether directional blur is enabled
    cv::Mat kernel;
    if (directional) {
        kernel = generateDirectionalKernel(radius, angle);  // Generate a directional kernel
        std::cout << "Generated Directional Kernel." << std::endl;
    } else {
        kernel = generateGaussianKernel(radius);  // Generate a Gaussian kernel
        std::cout << "Generated Gaussian Kernel." << std::endl;
    }

    // Apply the kernel to the input image using convolution
    cv::filter2D(inputImage, outputImage, -1, kernel);

    // Check if the output image is valid after the blur operation
    if (outputImage.empty()) {
        std::cerr << "Failed to apply blur to the image." << std::endl;
    } else {
        std::cout << "Blur applied with radius: " << radius << " and " << (directional ? "directional" : "uniform") << " blur." << std::endl;
    }
}

// Render the user interface for controlling blur properties like radius and blur type
void BlurNode::renderUI() {
    std::cout << "[BlurNode: " << name << "]" << std::endl;

    // ImGui slider for controlling the blur radius
    if (ImGui::SliderInt("Radius", &radius, 1, 20)) {
        process();  // Recalculate blur whenever the radius is changed
    }

    // ImGui checkbox to toggle directional blur on or off
    if (ImGui::Checkbox("Directional Blur", &directional)) {
        process();  // Recalculate blur whenever the directional blur option is toggled
    }

    // Generate and display a preview of the selected kernel (Gaussian or Directional)
    cv::Mat kernelPreview = generateGaussianKernel(radius);
    if (directional) {
        kernelPreview = generateDirectionalKernel(radius, 45.0f);  // Directional kernel preview with a fixed angle
    }

    // Display the kernel preview in a separate window
    if (!kernelPreview.empty()) {
        cv::imshow("Kernel Preview", kernelPreview);
        cv::waitKey(0);  // Wait for a key press before closing the preview window
    }
}

// Get the output image after the blur operation
cv::Mat BlurNode::getOutput() const {
    return outputImage;
}

// Set a new radius and trigger reprocessing of the blur effect
void BlurNode::setRadius(int newRadius) {
    radius = newRadius;
    process();  // Recalculate blur with the new radius
}

// Set a new angle for directional blur and trigger reprocessing
void BlurNode::setAngle(float newAngle) {
    angle = newAngle;
    process();  // Recalculate blur with the new angle
}

// Enable or disable directional blur and trigger reprocessing
void BlurNode::setDirectional(bool isDirectional) {
    directional = isDirectional;
    process();  // Recalculate blur with the new directional setting
}

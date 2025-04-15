#include "BlurNode.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <imgui.h>

#define PI 3.14159265358979323846

BlurNode::BlurNode(const std::string& name) {
    this->name = name;
    this->id = "blur_" + name;
}

void BlurNode::setInput(const cv::Mat& input) {
    inputImage = input;
}

cv::Mat BlurNode::generateDirectionalKernel(int radius, float angleDegrees) {
    int size = radius * 2 + 1;
    cv::Mat kernel = cv::Mat::zeros(size, size, CV_32F);

    float angleRad = angleDegrees * PI / 180.0f;
    float dx = cos(angleRad);
    float dy = sin(angleRad);

    float length = std::sqrt(dx * dx + dy * dy);
    dx /= length;
    dy /= length;

    float sum = 0.0f;
    for (int i = -radius; i <= radius; ++i) {
        float x = radius + i * dx;
        float y = radius + i * dy;

        int ix = static_cast<int>(std::round(x));
        int iy = static_cast<int>(std::round(y));

        if (ix >= 0 && ix < size && iy >= 0 && iy < size) {
            kernel.at<float>(iy, ix) = 1.0f;
            sum += 1.0f;
        }
    }

    kernel /= sum;
    return kernel;
}

cv::Mat BlurNode::generateGaussianKernel(int radius) {
    int size = 2 * radius + 1;
    cv::Mat kernel(size, size, CV_32F);

    float sigma = radius / 3.0f;
    float sum = 0.0f;
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            kernel.at<float>(y + radius, x + radius) = std::exp(-(x * x + y * y) / (2 * sigma * sigma));
            sum += kernel.at<float>(y + radius, x + radius);
        }
    }

    kernel /= sum;
    return kernel;
}

void BlurNode::process() {
    if (inputImage.empty()) {
        std::cerr << "No input image for BlurNode: " << name << std::endl;
        return;
    }

    cv::Mat kernel;
    if (directional) {
        kernel = generateDirectionalKernel(radius, angle);
        std::cout << "Generated Directional Kernel." << std::endl;
    } else {
        kernel = generateGaussianKernel(radius);
        std::cout << "Generated Gaussian Kernel." << std::endl;
    }

    cv::filter2D(inputImage, outputImage, -1, kernel);

    if (outputImage.empty()) {
        std::cerr << "Failed to apply blur to the image." << std::endl;
    } else {
        std::cout << "Blur applied with radius: " << radius << " and " << (directional ? "directional" : "uniform") << " blur." << std::endl;
    }
}

void BlurNode::renderUI() {
    std::cout << "[BlurNode: " << name << "]" << std::endl;

    if (ImGui::SliderInt("Radius", &radius, 1, 20)) {
        process();
    }

    if (ImGui::Checkbox("Directional Blur", &directional)) {
        process();
    }

    cv::Mat kernelPreview = generateGaussianKernel(radius);
    if (directional) {
        kernelPreview = generateDirectionalKernel(radius, 45.0f);
    }

    if (!kernelPreview.empty()) {
        cv::imshow("Kernel Preview", kernelPreview);
        cv::waitKey(0);
    }
}

cv::Mat BlurNode::getOutput() const {
    return outputImage;
}

void BlurNode::setRadius(int newRadius) {
    radius = newRadius;
    process();
}

void BlurNode::setAngle(float newAngle) {
    angle = newAngle;
    process();
}

void BlurNode::setDirectional(bool isDirectional) {
    directional = isDirectional;
    process();
}

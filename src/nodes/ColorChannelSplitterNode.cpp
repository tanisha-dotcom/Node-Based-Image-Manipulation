#include "ColorChannelSplitterNode.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <imgui.h>

ColorChannelSplitterNode::ColorChannelSplitterNode(const std::string& name, bool outputGrayscale)
    : Node(), outputGrayscale(outputGrayscale) {
    this->name = name;
    this->id = "color_splitter_" + name;
}

void ColorChannelSplitterNode::setInput(const cv::Mat& input) {
    inputImage = input;
}

void ColorChannelSplitterNode::process() {
    if (inputImage.empty()) {
        std::cerr << "No input image for ColorChannelSplitterNode: " << name << std::endl;
        return;
    }

    cv::Mat grayscale;
    if (outputGrayscale) {
        cv::cvtColor(inputImage, grayscale, cv::COLOR_BGR2GRAY);
        cv::imwrite("GrayScale.png", grayscale);
        std::cout << "Image converted to grayscale." << std::endl;
    }

    std::vector<cv::Mat> channels;
    if (inputImage.channels() == 3) {
        cv::split(inputImage, channels);
        redChannel = channels[2];
        greenChannel = channels[1];
        blueChannel = channels[0];
    } else if (inputImage.channels() == 4) {
        cv::split(inputImage, channels);
        redChannel = channels[2];
        greenChannel = channels[1];
        blueChannel = channels[0];
        alphaChannel = channels[3];
    }

    if (!redChannel.empty()) {
        cv::imwrite("Red_Channel.png", redChannel);
    }
    if (!greenChannel.empty()) {
        cv::imwrite("Green_Channel.png", greenChannel);
    }
    if (!blueChannel.empty()) {
        cv::imwrite("Blue_Channel.png", blueChannel);
    }

    if (outputGrayscale) {
        cv::imwrite("Red_Grayscale.png", redChannel);
        cv::imwrite("Green_Grayscale.png", greenChannel);
        cv::imwrite("Blue_Grayscale.png", blueChannel);
    }

    if (!redChannel.empty()) {
        cv::imshow("Red Channel", redChannel);
    }
    if (!greenChannel.empty()) {
        cv::imshow("Green Channel", greenChannel);
    }
    if (!blueChannel.empty()) {
        cv::imshow("Blue Channel", blueChannel);
    }
    if (!alphaChannel.empty()) {
        cv::imshow("Alpha Channel", alphaChannel);
    }

    cv::waitKey(0);
}

cv::Mat ColorChannelSplitterNode::mergeChannels() {
    if (redChannel.empty() || greenChannel.empty() || blueChannel.empty()) {
        std::cerr << "One or more channels are empty, cannot merge." << std::endl;
        return cv::Mat();
    }

    std::vector<cv::Mat> channels = {blueChannel, greenChannel, redChannel};
    cv::Mat mergedImage;
    cv::merge(channels, mergedImage);
    return mergedImage;
}

void ColorChannelSplitterNode::renderUI() {
    std::cout << "[ColorChannelSplitterNode: " << name << "]" << std::endl;

    if (ImGui::Checkbox("Output Grayscale", &outputGrayscale)) {
        process();
    }

    if (!redChannel.empty()) {
        ImGui::Text("Red Channel");
        cv::Mat redChannelDisplay;
        cv::cvtColor(redChannel, redChannelDisplay, cv::COLOR_GRAY2BGRA);
        ImTextureID redTexture = reinterpret_cast<ImTextureID>(redChannelDisplay.data);
        ImGui::Image(redTexture, ImVec2(redChannel.cols, redChannel.rows));
    }

    if (!greenChannel.empty()) {
        ImGui::Text("Green Channel");
        cv::Mat greenChannelDisplay;
        cv::cvtColor(greenChannel, greenChannelDisplay, cv::COLOR_GRAY2BGRA);
        ImTextureID greenTexture = reinterpret_cast<ImTextureID>(greenChannelDisplay.data);
        ImGui::Image(greenTexture, ImVec2(greenChannel.cols, greenChannel.rows));
    }

    if (!blueChannel.empty()) {
        ImGui::Text("Blue Channel");
        cv::Mat blueChannelDisplay;
        cv::cvtColor(blueChannel, blueChannelDisplay, cv::COLOR_GRAY2BGRA);
        ImTextureID blueTexture = reinterpret_cast<ImTextureID>(blueChannelDisplay.data);
        ImGui::Image(blueTexture, ImVec2(blueChannel.cols, blueChannel.rows));
    }

    if (!alphaChannel.empty()) {
        ImGui::Text("Alpha Channel");
        cv::Mat alphaChannelDisplay;
        cv::cvtColor(alphaChannel, alphaChannelDisplay, cv::COLOR_GRAY2BGRA);
        ImTextureID alphaTexture = reinterpret_cast<ImTextureID>(alphaChannelDisplay.data);
        ImGui::Image(alphaTexture, ImVec2(alphaChannel.cols, alphaChannel.rows));
    }
}

cv::Mat ColorChannelSplitterNode::getOutput() const {
    if (outputGrayscale) {
        return redChannel;
    } else {
        return inputImage;
    }
}

void ColorChannelSplitterNode::setOutputGrayscale(bool enable) {
    outputGrayscale = enable;
    process();
}

void ColorChannelSplitterNode::resetParams() {
    outputGrayscale = false;
    process();
}

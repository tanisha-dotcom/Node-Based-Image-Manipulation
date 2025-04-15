#include "ColorChannelSplitterNode.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <imgui.h>

// Constructor: Initializes the node with a name and an option to output grayscale image
ColorChannelSplitterNode::ColorChannelSplitterNode(const std::string& name, bool outputGrayscale)
    : Node(), outputGrayscale(outputGrayscale) {
    this->name = name;
    this->id = "color_splitter_" + name;
}

// Sets the input image for processing
void ColorChannelSplitterNode::setInput(const cv::Mat& input) {
    inputImage = input;
}

// Process the image by splitting it into color channels (Red, Green, Blue, and optionally Alpha)
// If grayscale output is enabled, the grayscale image will also be generated
void ColorChannelSplitterNode::process() {
    if (inputImage.empty()) {
        std::cerr << "No input image for ColorChannelSplitterNode: " << name << std::endl;
        return;
    }

    cv::Mat grayscale;
    if (outputGrayscale) {
        // Convert to grayscale if enabled
        cv::cvtColor(inputImage, grayscale, cv::COLOR_BGR2GRAY);
        cv::imwrite("GrayScale.png", grayscale);  // Save the grayscale image
        std::cout << "Image converted to grayscale." << std::endl;
    }

    std::vector<cv::Mat> channels;
    // Split the input image into its RGB (or RGBA) channels
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

    // Save each channel as separate images
    if (!redChannel.empty()) {
        cv::imwrite("Red_Channel.png", redChannel);
    }
    if (!greenChannel.empty()) {
        cv::imwrite("Green_Channel.png", greenChannel);
    }
    if (!blueChannel.empty()) {
        cv::imwrite("Blue_Channel.png", blueChannel);
    }

    // Save grayscale versions of each channel, if outputGrayscale is enabled
    if (outputGrayscale) {
        cv::imwrite("Red_Grayscale.png", redChannel);
        cv::imwrite("Green_Grayscale.png", greenChannel);
        cv::imwrite("Blue_Grayscale.png", blueChannel);
    }

    // Display each channel in a window for visualization
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

    cv::waitKey(0);  // Wait for a key press to close the display windows
}

// Merge the individual RGB (or RGBA) channels back into a single image
cv::Mat ColorChannelSplitterNode::mergeChannels() {
    if (redChannel.empty() || greenChannel.empty() || blueChannel.empty()) {
        std::cerr << "One or more channels are empty, cannot merge." << std::endl;
        return cv::Mat();  // Return an empty matrix if any channel is empty
    }

    // Merge channels in RGB order for a standard color image
    std::vector<cv::Mat> channels = {blueChannel, greenChannel, redChannel};
    cv::Mat mergedImage;
    cv::merge(channels, mergedImage);
    return mergedImage;
}

// Render the user interface for the ColorChannelSplitterNode to adjust settings and visualize channels
void ColorChannelSplitterNode::renderUI() {
    std::cout << "[ColorChannelSplitterNode: " << name << "]" << std::endl;

    // Checkbox to toggle grayscale output
    if (ImGui::Checkbox("Output Grayscale", &outputGrayscale)) {
        process();
    }

    // Display the Red, Green, Blue, and Alpha channels if available
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

// Get the output image based on the grayscale flag
cv::Mat ColorChannelSplitterNode::getOutput() const {
    if (outputGrayscale) {
        return redChannel;  // If grayscale, return the red channel (or any single channel)
    } else {
        return inputImage;  // Otherwise, return the original input image
    }
}

// Enable or disable grayscale output, and reprocess the image accordingly
void ColorChannelSplitterNode::setOutputGrayscale(bool enable) {
    outputGrayscale = enable;
    process();
}

// Reset parameters, disabling grayscale output and reprocessing the image
void ColorChannelSplitterNode::resetParams() {
    outputGrayscale = false;
    process();
}

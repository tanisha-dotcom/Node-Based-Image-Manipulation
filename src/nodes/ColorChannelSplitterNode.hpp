#pragma once
#include "../graph/Node.hpp"
#include <opencv2/opencv.hpp>
#include <vector>

// ColorChannelSplitterNode: A class for splitting the color channels (Red, Green, Blue, and optionally Alpha) of an image
class ColorChannelSplitterNode : public Node {
public:
    // Constructor: Initializes the node with a name and an option to output grayscale
    ColorChannelSplitterNode(const std::string& name, bool outputGrayscale = false);

    // Sets the input image for the node
    void setInput(const cv::Mat& input) override;

    // Processes the input image by splitting it into color channels
    void process() override;

    // Renders the UI for the node (to toggle grayscale output and display the channels)
    void renderUI() override;

    // Returns the processed image based on grayscale flag (either the input image or the red channel)
    cv::Mat getOutput() const override;

    // Merges the individual RGB (or RGBA) channels back into a single image
    cv::Mat mergeChannels();

    // Enables or disables grayscale output
    void setOutputGrayscale(bool enable);

    // Resets the parameters to default settings
    void resetParams();

    // Member variables to hold the input image and the individual color channels (Red, Green, Blue, Alpha)
    cv::Mat inputImage; 
    cv::Mat redChannel;
    cv::Mat greenChannel;
    cv::Mat blueChannel;
    cv::Mat alphaChannel;  

    // Flag to determine whether grayscale output is enabled
    bool outputGrayscale; 
};

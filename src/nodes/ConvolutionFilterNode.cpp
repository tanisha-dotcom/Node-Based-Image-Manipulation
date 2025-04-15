#include "ConvolutionFilterNode.hpp"
#include <iostream>

// Constructor: Initializes the node with an id and name, and sets the node type to Processing
ConvolutionFilterNode::ConvolutionFilterNode(const std::string &id, const std::string &name)
{
    this->id = id;
    this->name = name;
    this->nodeType = NodeType::Processing;
}

// Sets the kernel size for the convolution filter (either 3x3 or 5x5)
void ConvolutionFilterNode::setKernelSize(int size)
{
    if (size == 3 || size == 5)
    {
        kernelSize = size;
        kernelData.resize(size * size, 0.0f); // Initialize kernel data with zeroes
    }
}

// Sets a custom kernel (used when the user wants to define their own filter)
void ConvolutionFilterNode::setCustomKernel(const std::vector<float> &data)
{
    if (data.size() == kernelSize * kernelSize)
    {
        kernelData = data; // Store the custom kernel data
        preset = PresetType::Custom; // Mark this as a custom preset
    }
}

// Sets a predefined filter preset (Sharpen, Emboss, Edge Enhance)
void ConvolutionFilterNode::setPreset(PresetType type)
{
    preset = type;
    loadPreset(type); // Load the chosen preset kernel
}

// Sets the input image for processing
void ConvolutionFilterNode::setInput(const cv::Mat &input)
{
    inputImage = input.clone(); // Clone the input image to avoid modifying the original
}

// Applies the selected kernel to the input image and produces the output
void ConvolutionFilterNode::process()
{
    applyKernel(); // Apply the selected kernel to the image
}

// Renders the user interface (currently just a placeholder for rendering logic)
void ConvolutionFilterNode::renderUI()
{
    std::cout << "Rendering UI for Convolution Filter Node\n";
    // Add your ImGui UI rendering logic here
}

// Returns the resulting image after convolution
cv::Mat ConvolutionFilterNode::getOutput() const
{
    return outputImage; // Return the processed image
}

// Applies the chosen kernel to the input image using OpenCV's filter2D function
void ConvolutionFilterNode::applyKernel()
{
    if (inputImage.empty())
        return; // Ensure the input image is not empty

    // Create a CV_32F matrix for the kernel from the kernel data
    cv::Mat kernel(kernelSize, kernelSize, CV_32F, const_cast<float *>(kernelData.data()));

    // Apply the kernel using filter2D to perform the convolution
    cv::filter2D(inputImage, outputImage, -1, kernel);
}

// Loads the appropriate preset kernel based on the preset type
void ConvolutionFilterNode::loadPreset(PresetType type)
{
    switch (type)
    {
    case PresetType::Sharpen:
        kernelSize = 3; // Set kernel size to 3x3
        kernelData = {
            0, -1, 0,
            -1, 5, -1,
            0, -1, 0}; // Sharpen kernel
        break;
    case PresetType::Emboss:
        kernelSize = 3; // Set kernel size to 3x3
        kernelData = {
            -2, -1, 0,
            -1, 1, 1,
            0, 1, 2}; // Emboss kernel
        break;
    case PresetType::EdgeEnhance:
        kernelSize = 3; // Set kernel size to 3x3
        kernelData = {
            0, -1, 0,
            -1, 5, -1,
            0, -1, 0}; // Edge enhancement kernel
        break;
    default:
        break;
    }
}

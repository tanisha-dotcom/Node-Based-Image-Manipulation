#pragma once

#include "../graph/Node.hpp"
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

// A node that applies a convolution filter to an image using a custom or preset kernel
class ConvolutionFilterNode : public Node {
public:
    // Enum for the available preset filter types
    enum class PresetType { Custom, Sharpen, Emboss, EdgeEnhance };

    // Constructor: Initializes the node with a unique ID and display name
    ConvolutionFilterNode(const std::string& id, const std::string& name);

    // Sets the kernel size (only 3x3 or 5x5 supported)
    void setKernelSize(int size);

    // Sets a custom kernel (user-defined filter weights)
    void setCustomKernel(const std::vector<float>& kernelData);

    // Applies a predefined filter by setting the corresponding preset
    void setPreset(PresetType type);

    // Sets the input image for the convolution operation
    void setInput(const cv::Mat& input) override;

    // Applies the convolution filter using the selected kernel
    void process() override;

    // Renders UI elements for this node (e.g., kernel editor, preset selector)
    void renderUI() override;

    // Returns the processed (filtered) output image
    cv::Mat getOutput() const override;

private:
    // Internal method that applies the kernel to the input image using OpenCV
    void applyKernel();

    // Loads weights for a predefined kernel based on the selected preset
    void loadPreset(PresetType type);

    int kernelSize = 3;                       // Size of the kernel (3 or 5)
    std::vector<float> kernelData;           // Flat vector representing the kernel weights
    PresetType preset = PresetType::Custom;  // Currently selected preset type

    cv::Mat inputImage;   // Input image to apply the filter on
    cv::Mat outputImage;  // Resulting image after applying the kernel
};

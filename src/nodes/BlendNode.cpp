#include "BlendNode.hpp"
#include <opencv2/opencv.hpp>
#include <imgui.h>
#include <iostream>

// Constructor for BlendNode, initializing the node with a name and generating a unique ID.
BlendNode::BlendNode(const std::string &name)
{
    this->name = name;
    this->id = "blend_node_" + name; // Generate a unique ID for the node
}

// Sets the first input image (inputA) by cloning the provided image.
void BlendNode::setInputA(const cv::Mat &image)
{
    inputA = image.clone();
}

// Sets the second input image (inputB) by cloning the provided image.
void BlendNode::setInputB(const cv::Mat &image)
{
    inputB = image.clone();
}

// Sets the blending mode and triggers the process to apply the blending operation.
void BlendNode::setBlendMode(BlendMode mode)
{
    blendMode = mode;
    process(); // Recalculate the output based on the new blend mode
}

// Sets the opacity for the blend, clamping the value between 0.0 and 1.0, and triggers reprocessing.
void BlendNode::setOpacity(float value)
{
    opacity = std::clamp(value, 0.0f, 1.0f); // Ensure opacity is within the range [0.0, 1.0]
    process();                               // Recalculate the blend with the updated opacity
}

// Returns the final blended output image.
cv::Mat BlendNode::getOutput() const
{
    return outputImage;
}

// Processes the blending operation based on the selected mode and opacity.
void BlendNode::process()
{
    // If either input image is empty, output an error and stop processing
    if (inputA.empty() || inputB.empty())
    {
        std::cerr << "One or both input images are empty in BlendNode: " << name << std::endl;
        return;
    }

    // Resize the second image (inputB) to match the size of inputA
    cv::Mat resizedB;
    cv::resize(inputB, resizedB, inputA.size()); // Resize to ensure the images have the same size

    // Convert the input images to floating-point values for precise blending operations
    cv::Mat blendA, blendB;
    inputA.convertTo(blendA, CV_32F, 1.0 / 255.0);   // Normalize inputA to [0, 1]
    resizedB.convertTo(blendB, CV_32F, 1.0 / 255.0); // Normalize inputB to [0, 1]

    cv::Mat result; // The resulting blended image

    // Apply the selected blend mode
    switch (blendMode)
    {
    case NORMAL:
        result = blendB; // Normal mode: just return inputB
        break;
    case MULTIPLY:
        cv::multiply(blendA, blendB, result); // Multiply mode: element-wise multiplication
        break;
    case SCREEN:
        result = 1.0 - (1.0 - blendA).mul(1.0 - blendB); // Screen mode: inverse multiplication
        break;
    case OVERLAY:
        result = cv::Mat(blendA.size(), blendA.type());
        // Overlay mode: mix based on the brightness of blendA
        for (int y = 0; y < blendA.rows; y++)
        {
            for (int x = 0; x < blendA.cols; x++)
            {
                for (int c = 0; c < blendA.channels(); c++)
                {
                    float a = blendA.at<cv::Vec3f>(y, x)[c];
                    float b = blendB.at<cv::Vec3f>(y, x)[c];
                    result.at<cv::Vec3f>(y, x)[c] = (a < 0.5f) ? (2 * a * b) : (1 - 2 * (1 - a) * (1 - b));
                }
            }
        }
        break;
    case DIFFERENCE:
        result = cv::abs(blendA - blendB); // Difference mode: absolute difference between the images
        break;
    }

    // Blend the result with the specified opacity
    result = opacity * result + (1.0f - opacity) * blendA;

    // Convert the result back to an 8-bit image for display
    result.convertTo(outputImage, CV_8U, 255.0); // Convert to 8-bit image in the range [0, 255]
}

// Renders the user interface for controlling the blend mode and opacity using ImGui.
void BlendNode::renderUI()
{
    const char *blendNames[] = {"Normal", "Multiply", "Screen", "Overlay", "Difference"};

    // Create a combo box for selecting the blend mode
    if (ImGui::Combo("Blend Mode", reinterpret_cast<int *>(&blendMode), blendNames, IM_ARRAYSIZE(blendNames)))
    {
        process(); // Reprocess the blend if the mode is changed
    }

    // Create a slider for adjusting the opacity
    if (ImGui::SliderFloat("Opacity", &opacity, 0.0f, 1.0f))
    {
        process(); // Reprocess the blend if the opacity is changed
    }
}

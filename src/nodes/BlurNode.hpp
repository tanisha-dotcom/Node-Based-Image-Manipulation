#pragma once
#include "../graph/Node.hpp"  // Include the base Node class for inheritance
#include <opencv2/opencv.hpp>  // OpenCV for image processing
#include <iostream>  // For input-output operations

// BlurNode class that inherits from the Node class
class BlurNode : public Node {
private:
    cv::Mat inputImage;  // Input image to be processed
    cv::Mat outputImage;  // Output image after processing (blurred)
    int radius = 3;  // Radius for the blur effect, default is 3
    bool directional = false;  // Flag to determine if directional blur is used
    float angle = 0.0f;  // Angle for directional blur, default is 0 (horizontal)

    // Function to generate a directional kernel based on radius and angle
    cv::Mat generateDirectionalKernel(int radius, float angle);

    // Function to generate a Gaussian blur kernel based on the radius
    cv::Mat generateGaussianKernel(int radius);

public:
    // Constructor to initialize the BlurNode with a name
    BlurNode(const std::string& name);

    // Override method to set the input image for the blur node
    void setInput(const cv::Mat& input) override;

    // Override method to process the image using the selected blur method (directional or Gaussian)
    void process() override;

    // Override method to render the user interface for configuring the blur node (radius, directional option)
    void renderUI() override;

    // Override method to get the processed (blurred) output image
    cv::Mat getOutput() const override;

    // Method to set a new radius for the blur effect and apply the change
    void setRadius(int newRadius);

    // Method to set a new angle for directional blur and apply the change
    void setAngle(float newAngle);

    // Method to enable or disable directional blur and apply the change
    void setDirectional(bool isDirectional);
};

#pragma once
#include "../graph/Node.hpp"
#include <opencv2/opencv.hpp>

// The BlendNode class applies a blending effect to two input images using various blend modes.
class BlendNode : public Node
{
public:
    // Enum for the available blend modes
    enum BlendMode
    {
        NORMAL,    // Normal blending
        MULTIPLY,  // Multiply blending
        SCREEN,    // Screen blending
        OVERLAY,   // Overlay blending
        DIFFERENCE // Difference blending
    };

    // Constructor: Initializes the BlendNode with a given name.
    BlendNode(const std::string &name);

    // Sets the first input image (inputA) to be used in the blend.
    void setInputA(const cv::Mat &image);

    // Sets the second input image (inputB) to be used in the blend.
    void setInputB(const cv::Mat &image);

    // Sets the blend mode to one of the available modes from the enum.
    void setBlendMode(BlendMode mode);

    // Sets the opacity of the blend effect, where 0.0 is fully transparent and 1.0 is fully opaque.
    void setOpacity(float value); // 0.0 to 1.0

    // Processes the blend operation between inputA and inputB based on the selected blend mode and opacity.
    void process() override;

    // Renders the user interface (UI) for the BlendNode, allowing parameter adjustments in the GUI.
    void renderUI() override;

    // Returns the resulting blended image.
    cv::Mat getOutput() const override;

private:
    // The first input image (left operand for blending)
    cv::Mat inputA;

    // The second input image (right operand for blending)
    cv::Mat inputB;

    // The resulting image after blending
    cv::Mat outputImage;

    // The selected blend mode (default is NORMAL)
    BlendMode blendMode = NORMAL;

    // The opacity of the resulting image (default is 1.0, fully opaque)
    float opacity = 1.0f;
};

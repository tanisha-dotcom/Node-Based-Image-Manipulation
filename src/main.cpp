#include "graph/NodeGraph.hpp"
#include "nodes/EdgeDetectionNode.hpp"
#include "nodes/ImageInputNode.hpp"
#include "nodes/ColorChannelSplitterNode.hpp"
#include "nodes/OutputNode.hpp"
#include "nodes/BrightnessContrastNode.hpp"
#include "nodes/BlurNode.hpp"
#include "nodes/ThresholdNode.hpp"
#include "nodes/EdgeDetectionNode.hpp"
#include <iostream>
#include <imgui.h>

void showMenu();
void loadImage(std::shared_ptr<ImageInputNode> &inputNode, bool &imageLoaded);
void splitChannels(std::shared_ptr<ColorChannelSplitterNode> &splitterNode, std::shared_ptr<ImageInputNode> &inputNode, bool &channelsSplit, bool imageLoaded);
void convertToGrayscale(std::shared_ptr<ImageInputNode> &inputNode);
void adjustBrightnessContrast(std::shared_ptr<BrightnessContrastNode> &bcNode, bool imageLoaded);
void saveOutputImage(std::shared_ptr<OutputNode> &outputNode, bool channelsSplit, std::shared_ptr<ImageInputNode> &inputNode, std::shared_ptr<ColorChannelSplitterNode> &splitterNode);
void mergeChannels(std::shared_ptr<ColorChannelSplitterNode> &splitterNode);
void applyBlur(std::shared_ptr<BlurNode> &blurNode, std::shared_ptr<ImageInputNode> &inputNode);
void applyThreshold(std::shared_ptr<ThresholdNode> &thresholdNode, std::shared_ptr<ImageInputNode> &inputNode);
void applyEdgeDetection(std::shared_ptr<EdgeDetectionNode> &edgeNode, std::shared_ptr<ImageInputNode> &inputNode);

int main()
{
    NodeGraph graph;

    auto inputNode = std::make_shared<ImageInputNode>("MyImage", "../assets/a.jpg");
    auto splitterNode = std::make_shared<ColorChannelSplitterNode>("Splitter", false);
    auto outputNode = std::make_shared<OutputNode>("Result", "output_image", "jpg", 85);
    auto bcNode = std::make_shared<BrightnessContrastNode>("BrightnessContrast", 1.0, 0);
    auto blurNode = std::make_shared<BlurNode>("GaussianBlur");
    auto thresholdNode = std::make_shared<ThresholdNode>("ThresholdNode");
    auto edgedetectionNode = std::make_shared<EdgeDetectionNode>("EdgeDetection");

    graph.addNode(inputNode);
    graph.addNode(splitterNode);
    graph.addNode(outputNode);
    graph.addNode(bcNode);
    graph.addNode(blurNode);
    graph.addNode(thresholdNode);
    graph.addNode(edgedetectionNode);

    int choice = 0;
    bool imageLoaded = false;
    bool channelsSplit = false;

    while (true)
    {
        showMenu();
        std::cout << "Enter your choice (1-10): ";
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            loadImage(inputNode, imageLoaded);
            break;
        case 2:
            splitChannels(splitterNode, inputNode, channelsSplit, imageLoaded);
            break;
        case 3:
            convertToGrayscale(inputNode);
            break;
        case 4:
            adjustBrightnessContrast(bcNode, imageLoaded);
            break;
        case 5:
            saveOutputImage(outputNode, channelsSplit, inputNode, splitterNode);
            break;
        case 6:
            mergeChannels(splitterNode);
            break;
        case 7:
            applyBlur(blurNode, inputNode);
            break;
        case 8:
            applyThreshold(thresholdNode, inputNode);
            break;
        case 9:
            applyEdgeDetection(edgedetectionNode, inputNode);
            break;
        case 10:
            std::cout << "Exiting program." << std::endl;
            return 0;
        default:
            std::cout << "Invalid choice. Please select a valid option." << std::endl;
        }
    }

    return 0;
}

void showMenu()
{
    std::cout << "\nChoose an action:" << std::endl;
    std::cout << "1. Load Image" << std::endl;
    std::cout << "2. Split Color Channels" << std::endl;
    std::cout << "3. Convert to Grayscale" << std::endl;
    std::cout << "4. Adjust Brightness and Contrast" << std::endl;
    std::cout << "5. Save Output Image" << std::endl;
    std::cout << "6. Merge Channels" << std::endl;
    std::cout << "7. Apply Blur to Image" << std::endl;
    std::cout << "8. Apply Thresholding" << std::endl;
    std::cout << "9. Edge Detection" << std::endl;
    std::cout << "10. Exit" << std::endl;
}

void loadImage(std::shared_ptr<ImageInputNode> &inputNode, bool &imageLoaded)
{
    if (!imageLoaded)
    {
        std::string imagename;
        std::cout << "Enter the image file name: ";
        std::string s = "../assets/";
        std::cin >> imagename;
        std::string imagePath = s + imagename;

        inputNode = std::make_shared<ImageInputNode>("MyImage", imagePath);
        inputNode->process();
        std::cout << "Image loaded successfully!" << std::endl;
        imageLoaded = true;
    }
    else
    {
        std::cout << "Image is already loaded." << std::endl;
    }
}

void splitChannels(std::shared_ptr<ColorChannelSplitterNode> &splitterNode, std::shared_ptr<ImageInputNode> &inputNode, bool &channelsSplit, bool imageLoaded)
{
    if (imageLoaded && !channelsSplit)
    {
        splitterNode->setInput(inputNode->getOutput());
        splitterNode->process();
        std::cout << "Color channels split successfully!" << std::endl;
        channelsSplit = true;
    }
    else if (!imageLoaded)
    {
        std::cout << "Please load an image first." << std::endl;
    }
    else
    {
        std::cout << "Channels are already split." << std::endl;
    }
}

void convertToGrayscale(std::shared_ptr<ImageInputNode> &inputNode)
{
    inputNode->convertToGrayscale();
}

void adjustBrightnessContrast(std::shared_ptr<BrightnessContrastNode> &bcNode, bool imageLoaded)
{
    if (imageLoaded)
    {
        float alpha, beta;
        std::cout << "Enter contrast (alpha) value (1.0 is normal): ";
        std::cin >> alpha;
        std::cout << "Enter brightness (beta) value (0 is normal): ";
        std::cin >> beta;
        bcNode->setParams(alpha, beta);
        bcNode->process();
        std::cout << "Brightness and contrast adjusted." << std::endl;
    }
    else
    {
        std::cout << "Please load an image first." << std::endl;
    }
}

void saveOutputImage(std::shared_ptr<OutputNode> &outputNode, bool channelsSplit, std::shared_ptr<ImageInputNode> &inputNode, std::shared_ptr<ColorChannelSplitterNode> &splitterNode)
{
    if (!channelsSplit)
    {
        outputNode->setInput(inputNode->getOutput());
        outputNode->process();
        std::cout << "Output image saved (adjusted brightness/contrast)." << std::endl;
    }
    else
    {
        std::cout << "Please split the color channels first to save specific channels." << std::endl;
    }
}

void mergeChannels(std::shared_ptr<ColorChannelSplitterNode> &splitterNode)
{
    cv::Mat mergedImage = splitterNode->mergeChannels();
    if (!mergedImage.empty())
    {
        cv::imwrite("Merged_Image.png", mergedImage);
        std::cout << "Merged image saved." << std::endl;
    }
    else
    {
        std::cout << "Failed to merge channels." << std::endl;
    }
}

void applyBlur(std::shared_ptr<BlurNode> &blurNode, std::shared_ptr<ImageInputNode> &inputNode)
{
    blurNode->setInput(inputNode->getOutput());
    std::cout << "What type blur you require? (D/G)" << std::endl;
    std::string blurtype;
    std::cin >> blurtype;
    if (blurtype == "D" || blurtype == "d")
    {
        int radius;
        float angle;
        std::cout << "Specify Radius and Angle." << std::endl;
        std::cout << "Radius: ";
        std::cin >> radius;
        std::cout << "Angle: ";
        std::cin >> angle;
        blurNode->setDirectional(true);
        blurNode->setRadius(radius);
        blurNode->setAngle(angle);
        blurNode->process();
    }
    if (blurtype == "G" || blurtype == "g")
    {
        int radius;
        std::cout << "Specify Radius: ";
        std::cin >> radius;
        blurNode->setRadius(radius);
        blurNode->setDirectional(false);
        blurNode->process();
    }
    cv::Mat blurredImage = blurNode->getOutput();
    if (!blurredImage.empty())
    {
        cv::imwrite("Blurred_Image.png", blurredImage);
        std::cout << "Blurred image saved." << std::endl;
    }
    else
    {
        std::cout << "Failed to apply blur." << std::endl;
    }
}

void applyThreshold(std::shared_ptr<ThresholdNode> &thresholdNode, std::shared_ptr<ImageInputNode> &inputNode)
{
    thresholdNode->setInput(inputNode->getOutput());
    thresholdNode->process();

    cv::Mat thresholdedImage = thresholdNode->getOutput();
    cv::Mat originalImage = inputNode->getOutput();

    if (originalImage.channels() != 1)
    {
        cv::cvtColor(originalImage, originalImage, cv::COLOR_BGR2GRAY);
    }

    cv::imshow("Original Image", originalImage);
    cv::imshow("Thresholded Image", thresholdedImage);
    cv::waitKey(0);
    cv::Mat diffImage;
    cv::absdiff(originalImage, thresholdedImage, diffImage);

    // Show the difference image
    cv::imshow("Difference Image", diffImage);
    cv::waitKey(0);

    cv::imwrite("Difference_Image.png", diffImage);

    std::cout << "Difference image saved." << std::endl;
}

void applyEdgeDetection(std::shared_ptr<EdgeDetectionNode> &edgeNode, std::shared_ptr<ImageInputNode> &inputNode)
{
    edgeNode->setInput(inputNode->getOutput());
    std::cout << "Give type of Edge Detection Algorithm. (Sobel/Canny)" << std::endl;
    std::string Algotype;
    std::cin >> Algotype;
    if (Algotype == "Sobel" || Algotype == "sobel")
    {
        edgeNode->setEdgeDetectionType(EdgeDetectionNode::SOBEL);
    }
    else if (Algotype == "Canny" || Algotype == "canny")
    {
        edgeNode->setEdgeDetectionType(EdgeDetectionNode::CANNY);
    }
    else
    {
        std::cout << "Invalid choice! Defaulting to Sobel." << std::endl;
        edgeNode->setEdgeDetectionType(EdgeDetectionNode::SOBEL);
    }

    edgeNode->process();

    cv::Mat edgeDetectedImage = edgeNode->getOutput();

    if (!edgeDetectedImage.empty())
    {
        cv::GaussianBlur(edgeDetectedImage, edgeDetectedImage, cv::Size(5, 5), 0);
        // cv::bitwise_not(edgeDetectedImage, edgeDetectedImage);

        cv::imwrite("Reversed_Soft_Edge_Detected_Image.png", edgeDetectedImage);
        std::cout << "Edge detection applied, softened, and reversed successfully. Image saved as Reversed_Soft_Edge_Detected_Image.png." << std::endl;

        cv::imshow("Reversed Soft Edge Detected Image", edgeDetectedImage);
        cv::waitKey(0);
    }
    else
    {
        std::cout << "Edge detection failed. The output image is empty." << std::endl;
    }
}

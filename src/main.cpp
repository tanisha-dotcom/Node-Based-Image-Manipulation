#include "graph/NodeGraph.hpp"
#include "nodes/EdgeDetectionNode.hpp"
#include "nodes/ImageInputNode.hpp"
#include "nodes/ColorChannelSplitterNode.hpp"
#include "nodes/OutputNode.hpp"
#include "nodes/BrightnessContrastNode.hpp"
#include "nodes/BlurNode.hpp"
#include "nodes/ThresholdNode.hpp"
#include "nodes/EdgeDetectionNode.hpp"
#include "nodes/BlendNode.hpp"
#include "nodes/NoiseGenerationNode.hpp"
#include "nodes/ConvolutionFilterNode.hpp"
#include <iostream>
#include <imgui.h>
#include <memory>

// Function to display the menu of available actions to the user
void showMenu()
{
    // Display the menu title
    std::cout << "\nChoose an action:" << std::endl;

    // List the available options to the user
    std::cout << "1. Load Image" << std::endl;                     // Option to load an image
    std::cout << "2. Split Color Channels" << std::endl;           // Option to split the image into RGB channels
    std::cout << "3. Convert to Grayscale" << std::endl;           // Option to convert the image to grayscale
    std::cout << "4. Adjust Brightness and Contrast" << std::endl; // Option to adjust brightness and contrast
    std::cout << "5. Save Output Image" << std::endl;              // Option to save the processed image
    std::cout << "6. Merge Channels" << std::endl;                 // Option to merge the split channels back
    std::cout << "7. Apply Blur to Image" << std::endl;            // Option to apply blur (e.g., Gaussian blur)
    std::cout << "8. Apply Thresholding" << std::endl;             // Option to apply thresholding (binary, adaptive, Otsu)
    std::cout << "9. Edge Detection" << std::endl;                 // Option to detect edges in the image
    std::cout << "10. Blend Images" << std::endl;                  // Option to blend two images
    std::cout << "11. Generate Noise" << std::endl;                // Option to add noise to the image
    std::cout << "12. Apply Convolution Filter" << std::endl;      // Option to apply a convolution filter (e.g., sharpen, blur)
    std::cout << "13. See Current Image." << std::endl;            // Option to display the current image
    std::cout << "14. Undo Image (only 1 step)" << std::endl;      // Option to undo the last action
    std::cout << "15. Exit" << std::endl;                          // Option to exit the program
}

// Function to load an image file through the ImageInputNode
void loadImage(std::shared_ptr<ImageInputNode> &inputNode, bool &imageLoaded, cv::Mat &currentImage)
{
    // Check if the image is already loaded
    if (!imageLoaded)
    {
        std::string imagename;        // To store the user-inputted image file name
        std::string s = "../assets/"; // Path to assets directory

        // Loop until a valid image is loaded
        while (true)
        {
            std::cout << "Enter the image file name: "; // Ask user for image file name
            std::cin >> imagename;
            std::string imagePath = s + imagename; // Construct the full image path

            // Try to read the image
            cv::Mat img = cv::imread(imagePath, cv::IMREAD_COLOR);

            // Check if the image was loaded successfully
            if (img.empty())
            {
                // If not, print error and prompt user to try again
                std::cout << "Error: Image not found or could not be loaded. Please try again.\n";
            }
            else
            {
                // Create the ImageInputNode with the image path and process it
                inputNode = std::make_shared<ImageInputNode>("MyImage", imagePath);
                inputNode->process(); // Process the image through the input node
                std::cout << "Image loaded successfully!" << std::endl;
                imageLoaded = true; // Set the flag to indicate the image is loaded
                break;              // Exit the loop after successful load
            }
        }
        // Get the processed image from the input node and assign it to currentImage
        currentImage = inputNode->getOutput();
    }
    else
    {
        // If the image is already loaded, notify the user
        std::cout << "Image is already loaded." << std::endl;
    }
}

// Function to split the color channels of the image.
// Also saves the previous image to allow comparison and restoration if needed.
void splitChannels(std::shared_ptr<ColorChannelSplitterNode> &splitterNode, cv::Mat &currentImage, cv::Mat &previousImage, bool &channelsSplit, bool imageLoaded)
{
    // Save the current image to previousImage to allow undo functionality
    previousImage = currentImage;

    // Check if the image is loaded and the channels have not been split yet
    if (imageLoaded && !channelsSplit)
    {
        // Set the input image for the splitter node and process it
        splitterNode->setInput(currentImage);
        splitterNode->process();

        // Notify the user that the channels were split successfully
        std::cout << "Color channels split successfully!" << std::endl;

        // Set the flag to true indicating that channels are now split
        channelsSplit = true;
    }
    else if (!imageLoaded)
    {
        // If no image is loaded, notify the user to load an image first
        std::cout << "Please load an image first." << std::endl;
    }
    else
    {
        // If the channels are already split, notify the user
        std::cout << "Channels are already split." << std::endl;
    }
}

// Function to convert the current image to grayscale and store the previous image.
// This allows users to revert back to the original image if needed.
void convertToGrayscale(cv::Mat &currentImage, cv::Mat &previousImage)
{
    // Save the current image to previousImage to allow undo functionality
    previousImage = currentImage;

    // Create a new matrix to hold the grayscale image
    cv::Mat output;

    // Convert the current image from BGR (colored) to grayscale
    cv::cvtColor(currentImage, output, cv::COLOR_BGR2GRAY);

    // Update the currentImage with the grayscale output
    currentImage = output;
}

// Function to adjust the brightness and contrast of the image.
// Takes the current image and applies the changes, storing the previous state for potential undo.
void adjustBrightnessContrast(std::shared_ptr<BrightnessContrastNode> &bcNode, bool imageLoaded, cv::Mat &currentImage, cv::Mat &previousImage)
{
    // Save the current image to previousImage to allow undo functionality
    previousImage = currentImage;

    // Check if an image has been loaded
    if (imageLoaded)
    {
        // Set the input image to the BrightnessContrastNode
        bcNode->setInput(currentImage);

        // Declare variables for contrast (alpha) and brightness (beta) values
        float alpha, beta;

        // Prompt user to input the contrast (alpha) value
        std::cout << "Enter contrast (alpha) value (1.0 is normal): ";
        std::cin >> alpha;

        // Prompt user to input the brightness (beta) value
        std::cout << "Enter brightness (beta) value (0 is normal): ";
        std::cin >> beta;

        // Set the parameters for brightness and contrast
        bcNode->setParams(alpha, beta);

        // Process the image to adjust brightness and contrast
        bcNode->process();
        std::cout << "Brightness and contrast adjusted." << std::endl;

        // Get the output image after adjustment
        cv::Mat out = bcNode->getOutput();

        // If the output is not empty, save the adjusted image to disk
        if (!out.empty())
        {
            cv::imwrite("Bright.png", out); // Save the image as "Bright.png"
            std::cout << "Bright image saved." << std::endl;
        }
    }
    else
    {
        // Inform the user that an image must be loaded first
        std::cout << "Please load an image first." << std::endl;
    }
}

// Function to save the output image (after applying the desired processing).
void saveOutputImage(std::shared_ptr<OutputNode> &outputNode, cv::Mat &currentImage)
{
    // Check if the current image is not empty
    if (!currentImage.empty())
    {
        // Ask the user to specify the image file type (jpg, png, jpeg)
        std::cout << "Please Specify Image type: (jpg/png/jpeg): ";
        std::string type;
        std::cin >> type; // Get the image type from the user

        // Set the output image type in the OutputNode
        outputNode->settype(type);

        // Set the input image for the output node
        outputNode->setInput(currentImage);

        // Process the image to save it in the specified format
        outputNode->process();

        // Inform the user that the image has been saved
        std::cout << "Output image saved." << std::endl;
    }
    else
    {
        // Inform the user that no image has been loaded
        std::cout << "No Image Loaded." << std::endl;
    }
}

// Function to merge the color channels back into a single image after they have been split.
void mergeChannels(std::shared_ptr<ColorChannelSplitterNode> &splitterNode)
{
    // Call the mergeChannels function from the splitterNode to merge the color channels
    cv::Mat mergedImage = splitterNode->mergeChannels();

    // Check if the merged image is not empty
    if (!mergedImage.empty())
    {
        // Save the merged image to a file
        cv::imwrite("Merged_Image.png", mergedImage);
        std::cout << "Merged image saved." << std::endl;
    }
    else
    {
        // If merging the channels fails, inform the user
        std::cout << "Failed to merge channels." << std::endl;
    }
}

// Function to apply a blur (either Gaussian or directional) to the image.
void applyBlur(std::shared_ptr<BlurNode> &blurNode, cv::Mat &currentImage, cv::Mat &previousImage)
{
    // Save the current image to previousImage for potential undo functionality
    previousImage = currentImage;

    // Set the input image for the blur node
    blurNode->setInput(currentImage);

    // Ask the user for the type of blur they want to apply (Directional or Gaussian)
    std::cout << "What type of blur do you require? (D/G)" << std::endl;
    std::string blurtype;
    std::cin >> blurtype;

    // Apply Directional Blur (if "D" or "d" is chosen)
    if (blurtype == "D" || blurtype == "d")
    {
        int radius;
        float angle;
        // Ask user for the radius and angle for directional blur
        std::cout << "Specify Radius and Angle." << std::endl;
        std::cout << "Radius: ";
        std::cin >> radius;
        std::cout << "Angle: ";
        std::cin >> angle;

        // Set blur properties for directional blur
        blurNode->setDirectional(true);
        blurNode->setRadius(radius);
        blurNode->setAngle(angle);

        // Apply the blur
        blurNode->process();
    }

    // Apply Gaussian Blur (if "G" or "g" is chosen)
    if (blurtype == "G" || blurtype == "g")
    {
        int radius;
        // Ask user for the radius for Gaussian blur
        std::cout << "Specify Radius: ";
        std::cin >> radius;

        // Set blur properties for Gaussian blur
        blurNode->setRadius(radius);
        blurNode->setDirectional(false);

        // Apply the blur
        blurNode->process();
    }

    // Get the output of the blurred image
    cv::Mat blurredImage = blurNode->getOutput();

    // If the image is valid, save it to a file
    if (!blurredImage.empty())
    {
        cv::imwrite("Blurred_Image.png", blurredImage);
        std::cout << "Blurred image saved." << std::endl;
    }
    else
    {
        // If blur application failed, inform the user
        std::cout << "Failed to apply blur." << std::endl;
    }
}

// Function to apply thresholding to the image, transforming it into a binary image based on the threshold values.
void applyThreshold(std::shared_ptr<ThresholdNode> &thresholdNode, cv::Mat &currentImage, cv::Mat &previousImage)
{
    // Save the current image to previousImage for potential undo functionality
    previousImage = currentImage;

    // Set the input image for the threshold node
    thresholdNode->setInput(currentImage);

    // Ask the user for the type of threshold they want to apply
    std::cout << "Which type of Threshold do you want? (B/A/O)" << std::endl;
    std::string thresholdtype;
    std::cin >> thresholdtype;

    // Set the threshold type based on user input
    if (thresholdtype == "B")
    {
        thresholdNode->setThresholdType(ThresholdNode::BINARY);
    }
    else if (thresholdtype == "A")
    {
        thresholdNode->setThresholdType(ThresholdNode::ADAPTIVE);
    }
    else if (thresholdtype == "O")
    {
        thresholdNode->setThresholdType(ThresholdNode::OTSU);
    }

    // Apply the thresholding operation
    thresholdNode->process();

    // Store the original image before thresholding
    cv::Mat originalImage = currentImage;

    // Update the current image to the thresholded output
    currentImage = thresholdNode->getOutput();

    // Convert the original image to grayscale if it's not already
    if (originalImage.channels() != 1)
    {
        cv::cvtColor(originalImage, originalImage, cv::COLOR_BGR2GRAY);
    }

    // Display the original and thresholded images
    cv::imshow("Original Image", originalImage);
    cv::imshow("Thresholded Image", currentImage);
    cv::waitKey(0);

    // Compute the difference image between the original and thresholded images
    cv::Mat diffImage;
    cv::absdiff(originalImage, currentImage, diffImage);

    // Display the difference image
    cv::imshow("Difference Image", diffImage);
    cv::waitKey(0);

    // Save the difference image to disk
    cv::imwrite("Difference_Image.png", diffImage);

    std::cout << "Difference image saved." << std::endl;
}

// Function to apply edge detection (Sobel or Canny) to the image and display the detected edges.
void applyEdgeDetection(std::shared_ptr<EdgeDetectionNode> &edgeNode, cv::Mat &currentImage, cv::Mat &previousImage)
{
    // Save the current image to previousImage for potential undo functionality
    previousImage = currentImage;

    // Set the input image for the edge detection node
    edgeNode->setInput(currentImage);

    // Ask the user for the type of edge detection algorithm to use
    std::cout << "Give type of Edge Detection Algorithm. (Sobel/Canny)" << std::endl;
    std::string Algotype;
    std::cin >> Algotype;

    // Set up the edge detection algorithm based on user input
    if (Algotype == "Sobel" || Algotype == "sobel")
    {
        // Use Sobel edge detection
        edgeNode->setEdgeDetectionType(EdgeDetectionNode::SOBEL);
        std::cout << "Give Sobel Kernel Size: ";
        int kernelsize;
        std::cin >> kernelsize;
        edgeNode->setSobelKernelSize(kernelsize);
    }
    else if (Algotype == "Canny" || Algotype == "canny")
    {
        // Use Canny edge detection
        edgeNode->setEdgeDetectionType(EdgeDetectionNode::CANNY);
        std::cout << "Give Canny Threshold values." << std::endl;
        int threshold1, threshold2;
        std::cout << "Threshold1 value: ";
        std::cin >> threshold1;
        std::cout << "Threshold2 value: ";
        std::cin >> threshold2;
        edgeNode->setCannyThresholds(threshold1, threshold2);
    }
    else
    {
        // Default to Sobel if the input is invalid
        std::cout << "Invalid choice! Defaulting to Sobel." << std::endl;
        edgeNode->setEdgeDetectionType(EdgeDetectionNode::SOBEL);
        std::cout << "Give Sobel Kernel Size: ";
        int kernelsize;
        std::cin >> kernelsize;
        edgeNode->setSobelKernelSize(kernelsize);
    }

    // Ask the user if they want to overlay the edges on the original image
    std::cout << "Do you want to overlay edges on the original image? (yes/no): ";
    std::string overlayInput;
    std::cin >> overlayInput;

    // Set whether to overlay the edges or not based on the user's input
    bool overlay = (overlayInput == "yes" || overlayInput == "Yes" || overlayInput == "y");
    edgeNode->setOverlayEdges(overlay);

    // Apply the edge detection algorithm
    edgeNode->process();

    // Get the output image from the edge detection process
    currentImage = edgeNode->getOutput();

    // If the edge-detected image is not empty, apply Gaussian blur and save the output
    if (!currentImage.empty())
    {
        // Apply Gaussian blur to soften the edges
        cv::GaussianBlur(currentImage, currentImage, cv::Size(5, 5), 0);

        // Save the processed image as "Soft_Edge_Detected_Image.png"
        cv::imwrite("Soft_Edge_Detected_Image.png", currentImage);
        std::cout << "Edge detection applied, softened";
        if (overlay)
            std::cout << ", overlaid";
        std::cout << ", and saved as Soft_Edge_Detected_Image.png." << std::endl;

        // Display the final edge-detected image
        cv::imshow("Final Edge Detected Image", currentImage);
        cv::waitKey(0);
    }
    else
    {
        // If the edge detection failed (empty output), notify the user
        std::cout << "Edge detection failed. The output image is empty." << std::endl;
    }
}

// Function to blend the current image with a previous one or another image, allowing for effects like fade or transition.
void applyBlend(std::shared_ptr<BlendNode> &blendNode, cv::Mat &currentImage, cv::Mat &previousImage)
{
    // Save the current image as the previous image
    previousImage = currentImage;

    // Ask the user for the path of the second image to blend with the current image
    std::string secondImageName;
    std::cout << "Enter path of second image: ";
    std::cin >> secondImageName;

    // Create an input node for the second image and process it
    std::shared_ptr<ImageInputNode> secondInput = std::make_shared<ImageInputNode>("SecondImage", secondImageName);
    secondInput->process();

    // Set the input images for the blend node
    blendNode->setInputA(currentImage);
    blendNode->setInputB(secondInput->getOutput());

    // Ask the user to select a blend mode
    std::string blendMode;
    std::cout << "Enter blend mode (normal/multiply/screen/overlay/difference): ";
    std::cin >> blendMode;
    if (blendMode == "normal")
    {
        blendNode->setBlendMode(BlendNode::NORMAL);
    }
    else if (blendMode == "multiply")
    {
        blendNode->setBlendMode(BlendNode::MULTIPLY);
    }
    else if (blendMode == "screen")
    {
        blendNode->setBlendMode(BlendNode::SCREEN);
    }
    else if (blendMode == "overlay")
    {
        blendNode->setBlendMode(BlendNode::OVERLAY);
    }
    else if (blendMode == "difference")
    {
        blendNode->setBlendMode(BlendNode::DIFFERENCE);
    }
    else
    {
        std::cout << "Invalid blend mode. Defaulting to 'normal'." << std::endl;
        blendNode->setBlendMode(BlendNode::NORMAL);
    }

    // Ask the user for the opacity (mix factor) between the two images
    float opacity;
    std::cout << "Enter mix/opacity (0.0 - 1.0): ";
    std::cin >> opacity;
    blendNode->setOpacity(opacity);

    // Process the blend node
    blendNode->process();

    // Get the output blended image
    currentImage = blendNode->getOutput();

    // If the blended image is valid, save and display it
    if (!currentImage.empty())
    {
        // Save the blended image to a file
        cv::imwrite("Blended_Image.png", currentImage);
        std::cout << "Blended image saved as Blended_Image.png" << std::endl;

        // Display the blended image
        cv::imshow("Blended Image", currentImage);
        cv::waitKey(0);
    }
    else
    {
        // Handle the case where blending failed
        std::cout << "Failed to blend images" << std::endl;
    }
}

// Function to apply procedural noise (e.g., Perlin, Simplex) to the image, generating a noise pattern.
void applyNoise(std::shared_ptr<NoiseGeneratorNode> &noiseNode, std::shared_ptr<ImageInputNode> &inputNode)
{
    std::string type;
    int octaves;
    float persistence, scale;

    // Set the input image for the noise generator node
    noiseNode->setInput(inputNode->getOutput());

    // Ask the user for the noise type (Perlin, Simplex, or Worley)
    while (true)
    {
        std::cout << "Enter Noise Type (perlin/simplex/worley): ";
        std::cin >> type;
        if (type == "perlin")
        {
            noiseNode->setNoiseType(NoiseGeneratorNode::NoiseType::Perlin);
            break;
        }
        else if (type == "simplex")
        {
            noiseNode->setNoiseType(NoiseGeneratorNode::NoiseType::Simplex);
            break;
        }
        else if (type == "worley")
        {
            noiseNode->setNoiseType(NoiseGeneratorNode::NoiseType::Worley);
            break;
        }
        else
        {
            std::cout << "Invalid noise type. Please try again." << std::endl;
        }
    }

    // Get the scale value for the noise generation
    while (true)
    {
        std::cout << "Enter Scale (float): ";
        std::cin >> scale;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input for scale. Please enter a valid floating-point number." << std::endl;
        }
        else
        {
            noiseNode->setScale(scale);
            break;
        }
    }

    // Get the number of octaves for the noise generation
    while (true)
    {
        std::cout << "Enter Octaves (int): ";
        std::cin >> octaves;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input for octaves. Please enter a valid integer." << std::endl;
        }
        else
        {
            noiseNode->setOctaves(octaves);
            break;
        }
    }

    // Get the persistence value for the noise generation
    while (true)
    {
        std::cout << "Enter Persistence (float): ";
        std::cin >> persistence;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input for persistence. Please enter a valid floating-point number." << std::endl;
        }
        else
        {
            noiseNode->setPersistence(persistence);
            break;
        }
    }

    // Ask the user whether to use noise as color or displacement
    std::string usage;
    while (true)
    {
        std::cout << "Use noise as color or displacement? (c/d): ";
        std::cin >> usage;
        if (usage == "c" || usage == "C")
        {
            noiseNode->setUseAsDisplacement(false);
            break;
        }
        else if (usage == "d" || usage == "D")
        {
            noiseNode->setUseAsDisplacement(true);
            break;
        }
        else
        {
            std::cout << "Invalid option. Please enter 'c' for color or 'd' for displacement." << std::endl;
        }
    }

    // Process the noise generation
    noiseNode->process();

    // Get the output of the noise generation
    cv::Mat result = noiseNode->getOutput();
    if (!result.empty())
    {
        // Convert the result to 8-bit format for saving/display
        cv::Mat displayResult;
        result.convertTo(displayResult, CV_8U, 255.0);

        // Save the noise image based on the usage (color or displacement)
        std::string filename = usage == "d" ? "Displacement_Noise.png" : "Color_Noise.png";
        cv::imwrite(filename, displayResult);
        std::cout << "Noise image saved as " << filename << std::endl;
    }
    else
    {
        std::cout << "Failed to generate noise." << std::endl;
    }
}

// Function to apply a convolution filter to the image. This allows for edge detection, sharpening, etc.
void convolutionfilter(std::shared_ptr<ConvolutionFilterNode> &convoNode, cv::Mat &currentImage, cv::Mat &previousImage)
{
    previousImage = currentImage;      // Store the current image as previous image
    convoNode->setInput(currentImage); // Set the current image as input for the convolution filter

    // Display filter options for the user
    std::cout << "Choose a preset filter:\n";
    std::cout << "1. Sharpen\n2. Emboss\n3. Edge Enhance\n4. Custom\n";
    int choice;
    std::cin >> choice; // Get the filter choice from the user

    // Apply the corresponding filter based on user input
    switch (choice)
    {
    case 1:
        convoNode->setPreset(ConvolutionFilterNode::PresetType::Sharpen); // Apply sharpen filter
        break;
    case 2:
        convoNode->setPreset(ConvolutionFilterNode::PresetType::Emboss); // Apply emboss filter
        break;
    case 3:
        convoNode->setPreset(ConvolutionFilterNode::PresetType::EdgeEnhance); // Apply edge enhance filter
        break;
    case 4:
    {
        int kernelSize;
        std::cout << "Enter kernel size (3 or 5): "; // Ask for custom kernel size
        std::cin >> kernelSize;                      // Get the kernel size from user
        if (kernelSize == 3 || kernelSize == 5)      // Check if the kernel size is valid (3x3 or 5x5)
        {
            std::vector<float> customKernel(kernelSize * kernelSize); // Create a vector to store custom kernel values
            std::cout << "Enter the values for the " << kernelSize << "x" << kernelSize << " kernel:\n";

            // Loop to get the kernel values from the user
            for (int i = 0; i < kernelSize * kernelSize; ++i)
            {
                std::cin >> customKernel[i]; // Get the individual kernel values from the user
            }
            convoNode->setCustomKernel(customKernel); // Set the custom kernel for the filter
        }
        else
        {
            std::cout << "Invalid kernel size. Defaulting to sharpen filter.\n"; // Default to sharpen filter if invalid size
            convoNode->setPreset(ConvolutionFilterNode::PresetType::Sharpen);    // Apply sharpen filter
        }
    }
    break;
    default:
        std::cout << "Invalid choice! Defaulting to sharpen filter.\n";   // Handle invalid input
        convoNode->setPreset(ConvolutionFilterNode::PresetType::Sharpen); // Apply sharpen filter by default
        break;
    }

    // Process the image with the selected filter
    convoNode->process();
    currentImage = convoNode->getOutput(); // Get the filtered output

    // Display the result
    cv::imshow("Convolution Filter Effect", currentImage);
    cv::waitKey(0); // Wait for a key press to close the display window

    // Save the filtered image to disk
    std::cout << "Filter applied! Saving the output image...\n";
    cv::imwrite("Filtered_Output.png", currentImage); // Save the image as "Filtered_Output.png"
}

// Function to display the current image, useful for showing results after processing.
void showcurrentimage(cv::Mat &currentImage)
{
    // Check if the image is loaded and not empty
    if (!currentImage.empty())
    {
        // Display the image in a window named "Current Image"
        cv::imshow("Current Image", currentImage);

        // Wait for a key press to close the window
        cv::waitKey(0);
    }
    else
    {
        // If no image is loaded, print a message to the console
        std::cout << "No Image Loaded." << std::endl;
    }
}



int main()
{
    // Create a NodeGraph instance to manage the nodes
    NodeGraph graph;

    // Create nodes for different image processing tasks
    auto inputNode = std::make_shared<ImageInputNode>("MyImage", "../assets/a.jpg");      // Load an image
    auto splitterNode = std::make_shared<ColorChannelSplitterNode>("Splitter", false);    // Split color channels
    auto outputNode = std::make_shared<OutputNode>("Result", "output_image", "jpg", 85);  // Save the processed image
    auto bcNode = std::make_shared<BrightnessContrastNode>("BrightnessContrast", 1.0, 0); // Adjust brightness and contrast
    auto blurNode = std::make_shared<BlurNode>("GaussianBlur");                           // Apply blur to the image
    auto thresholdNode = std::make_shared<ThresholdNode>("ThresholdNode");                // Apply thresholding
    auto edgedetectionNode = std::make_shared<EdgeDetectionNode>("EdgeDetection");        // Detect edges in the image
    auto blendNode = std::make_shared<BlendNode>("Blend");                                // Blend images
    auto noiseNode = std::make_shared<NoiseGeneratorNode>("NoiseID", "NoiseGenerator");   // Generate noise
    auto convoNode = std::make_shared<ConvolutionFilterNode>("conv1", "ConvFilter");      // Apply a convolution filter

    // Add all the created nodes to the graph
    graph.addNode(inputNode);
    graph.addNode(splitterNode);
    graph.addNode(outputNode);
    graph.addNode(bcNode);
    graph.addNode(blurNode);
    graph.addNode(thresholdNode);
    graph.addNode(edgedetectionNode);
    graph.addNode(blendNode);
    graph.addNode(noiseNode);
    graph.addNode(convoNode);

    // Menu choice and image state initialization
    int choice = 0;
    bool imageLoaded = false;            // Flag to check if image is loaded
    bool channelsSplit = false;          // Flag to check if color channels are split
    cv::Mat currentImage, previousImage; // Matrices to hold current and previous image states

    // Main loop for the image processing application
    while (true)
    {
        // Show menu and prompt for user choice
        showMenu();
        std::cout << "Enter your choice (1-15): ";
        std::cin >> choice;

        // Process based on user choice
        switch (choice)
        {
        case 1:
            loadImage(inputNode, imageLoaded, currentImage); // Load an image
            break;
        case 2:
            splitChannels(splitterNode, currentImage, previousImage, channelsSplit, imageLoaded); // Split color channels
            break;
        case 3:
            convertToGrayscale(currentImage, previousImage); // Convert the image to grayscale
            break;
        case 4:
            adjustBrightnessContrast(bcNode, imageLoaded, currentImage, previousImage); // Adjust brightness and contrast
            break;
        case 5:
            saveOutputImage(outputNode, currentImage); // Save the processed image
            break;
        case 6:
            mergeChannels(splitterNode); // Merge the color channels back together
            break;
        case 7:
            applyBlur(blurNode, currentImage, previousImage); // Apply blur (Gaussian)
            break;
        case 8:
            applyThreshold(thresholdNode, currentImage, previousImage); // Apply thresholding
            break;
        case 9:
            applyEdgeDetection(edgedetectionNode, currentImage, previousImage); // Apply edge detection
            break;
        case 10:
            applyBlend(blendNode, currentImage, previousImage); // Apply image blending
            break;
        case 11:
            applyNoise(noiseNode, inputNode); // Add noise to the image
            break;
        case 12:
            convolutionfilter(convoNode, currentImage, previousImage); // Apply a convolution filter
            break;
        case 13:
            showcurrentimage(currentImage); // Display the current image
            break;
        case 14:
            std::cout << "Undo Done" << std::endl;
            currentImage = previousImage; // Undo the previous change
            break;
        case 15:
            std::cout << "Exiting program." << std::endl;
            return 0; // Exit the program
        default:
            std::cout << "Invalid choice. Please select a valid option." << std::endl; // Handle invalid input
        }
    }

    return 0;
}

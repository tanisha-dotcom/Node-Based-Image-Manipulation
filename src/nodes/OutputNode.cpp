#include "OutputNode.hpp"
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <imgui.h>

OutputNode::OutputNode(const std::string& name, const std::string& path, const std::string& type, int quality)
    : savePath(path), type(type), quality(quality) {
    this->name = name;
    this->id = "output_" + name;
}

void OutputNode::setInput(const cv::Mat& input) {
    inputImage = input;
}

void OutputNode::process() {
    if (inputImage.empty()) {
        std::cerr << "No input image for OutputNode: " << name << std::endl;
        return;
    }

    cv::imshow("Preview - " + name, inputImage);
    cv::waitKey(0); 

    std::vector<int> compression_params;
    if (type == "jpg" || type == "jpeg") {
        compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
        compression_params.push_back(quality);
    } else if (type == "png") {
        compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
        compression_params.push_back(quality); 
    }
    bool success = cv::imwrite(savePath + "." + type, inputImage, compression_params);
    if (success) {
        std::cout << "Output saved to: " << savePath << "." << type << std::endl;
    } else {
        std::cerr << "Failed to save output to: " << savePath << "." << type << std::endl;
    }

    cv::destroyAllWindows();
}

void OutputNode::renderUI() {
    ImGui::Text("[OutputNode: %s] Saving to: %s", name.c_str(), savePath.c_str());
    
    ImGui::SliderInt("Quality", &quality, 1, 100);

    char newPath[256];
    strcpy(newPath, savePath.c_str());
    if (ImGui::InputText("Save Path", newPath, IM_ARRAYSIZE(newPath))) {
        savePath = std::string(newPath); 
    }

    if (ImGui::Button("Save Image")) {
        process();  
    }

    if (!inputImage.empty()) {
        cv::Mat displayImage;
        cv::cvtColor(inputImage, displayImage, cv::COLOR_BGR2RGBA);
        displayImage = displayImage.clone();  

        unsigned char* data = displayImage.data;
        ImTextureID imageTexture = reinterpret_cast<ImTextureID>(data);
        ImGui::Image(imageTexture, ImVec2(displayImage.cols, displayImage.rows));  
    }
}

cv::Mat OutputNode::getOutput() const {
    return inputImage;
}

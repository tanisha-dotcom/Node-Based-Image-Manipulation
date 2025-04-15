#include "OutputNode.hpp"
#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
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

    // Optional preview in separate window
    cv::imshow("Preview - " + name, inputImage);
    cv::waitKey(1);  // non-blocking

    std::vector<int> compressionParams;
    if (type == "jpg" || type == "jpeg") {
        compressionParams.push_back(cv::IMWRITE_JPEG_QUALITY);
        compressionParams.push_back(quality);
    } else if (type == "png") {
        compressionParams.push_back(cv::IMWRITE_PNG_COMPRESSION);
        compressionParams.push_back(quality / 10);  // PNG uses 0-9 compression
    }

    std::string fullPath = savePath + "." + type;
    bool success = cv::imwrite(fullPath, inputImage, compressionParams);
    if (success) {
        std::cout << "[✅] Output saved to: " << fullPath << std::endl;
    } else {
        std::cerr << "[❌] Failed to save output to: " << fullPath << std::endl;
    }

    cv::destroyAllWindows();
}

void OutputNode::renderUI() {
    ImGui::Text("🖼️ Output Node: %s", name.c_str());
    
    ImGui::SliderInt("Quality", &quality, 1, 100);

    static char pathBuffer[256];
    strcpy(pathBuffer, savePath.c_str());
    if (ImGui::InputText("Save Path", pathBuffer, IM_ARRAYSIZE(pathBuffer))) {
        savePath = std::string(pathBuffer);
    }

    const char* formats[] = { "jpg", "png" };
    static int formatIdx = (type == "png") ? 1 : 0;
    if (ImGui::Combo("Format", &formatIdx, formats, IM_ARRAYSIZE(formats))) {
        type = formats[formatIdx];
    }

    if (ImGui::Button("💾 Save Image")) {
        process();
    }

    // Embedded preview (if needed — optional)
    if (!inputImage.empty()) {
        cv::Mat resized;
        float maxWidth = 200.0f;
        float scale = maxWidth / inputImage.cols;
        cv::resize(inputImage, resized, cv::Size(), scale, scale);

        cv::Mat rgba;
        cv::cvtColor(resized, rgba, cv::COLOR_BGR2RGBA);

        // Create OpenGL texture for ImGui if using a rendering backend (not covered here)
        // For now just a placeholder to show idea
        ImGui::Text("(Image preview would be shown here)");
    }
}

cv::Mat OutputNode::getOutput() const {
    return inputImage;
}

void OutputNode::settype(const std::string &stype) {
    this->type = std::move(stype);
}

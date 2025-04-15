#pragma once
#include <string>
#include <opencv2/opencv.hpp>
#include <vector>
#include <memory>

class Node {
public:
    std::string id;     
    std::string name;  

    virtual void process() = 0;  
    virtual void renderUI() = 0;  
    virtual cv::Mat getOutput() const = 0;  
    virtual void setInput(const cv::Mat& input) { this->input = input; }  

    cv::Mat getInput() const { return input; }

    virtual ~Node() = default; 

protected:
    cv::Mat input;  

    enum class NodeType { Input, Processing, Output };
    NodeType nodeType; 
};

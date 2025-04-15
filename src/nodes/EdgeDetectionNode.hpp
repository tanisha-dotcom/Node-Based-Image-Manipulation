#pragma once
#include "../graph/Node.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>

class EdgeDetectionNode : public Node
{
private:
    cv::Mat inputImage;
    cv::Mat outputImage;
    bool overlayEdges = false;

    int sobelKernelSize = 3;
    int cannyThreshold1 = 50;
    int cannyThreshold2 = 150;

public:
    enum EdgeDetectionType
    {
        SOBEL,
        CANNY
    };
    EdgeDetectionType edgeDetectionType = SOBEL;
    EdgeDetectionNode(const std::string &name);

    void setInput(const cv::Mat &input) override;

    void process() override;

    void renderUI() override;

    cv::Mat getOutput() const override;

    void setEdgeDetectionType(EdgeDetectionType type);

    void setSobelKernelSize(int size);

    void setCannyThresholds(int threshold1, int threshold2);

    void setOverlayEdges(bool overlay);
};

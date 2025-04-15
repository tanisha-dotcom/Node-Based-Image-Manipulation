#pragma once

#include "../libs/FastNoiseLite.h"
#include "../graph/Node.hpp"
#include <opencv2/opencv.hpp>
#include <string>

class NoiseGeneratorNode : public Node {
public:
    enum class NoiseType { Perlin, Simplex, Worley };

    NoiseGeneratorNode(const std::string& id, const std::string& name);

    // Config setters
    void setNoiseType(NoiseType type);
    void setScale(float scale);               // Frequency of the noise
    void setOctaves(int octaves);            // Number of fractal layers
    void setPersistence(float persistence);  // Amplitude scaling across octaves
    void setUseAsDisplacement(bool use);     // Enable displacement mode

    void setInput(const cv::Mat& input) override;
    cv::Mat getOutput() const override;

    void process() override;
    void renderUI() override;

private:
    void generateNoise();  // Generates procedural noise into `output`

    // Parameters
    NoiseType noiseType = NoiseType::Perlin;
    float scale = 0.05f;
    int octaves = 3;
    float persistence = 0.5f;
    bool useAsDisplacement = false;

    // Internal state
    cv::Mat inputImage;
    cv::Mat output;

    // Noise engine
    FastNoiseLite fastNoiseLite;
};

#pragma once
#include "../graph/Node.hpp"
#include <vector>
#include <memory>

class NodeGUIManager {
public:
    void renderAllNodesUI(const std::vector<std::shared_ptr<Node>>& nodes);
};

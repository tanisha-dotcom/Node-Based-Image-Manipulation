#pragma once
#include <vector>
#include <memory>
#include "Node.hpp"

class NodeGraph {
public:
    void addNode(const std::shared_ptr<Node>& node);
    void run();

    void connectNodes(const std::shared_ptr<Node>& fromNode, const std::shared_ptr<Node>& toNode);

    void clear();

    const std::vector<std::shared_ptr<Node>>& getNodes() const;

private:
    std::vector<std::shared_ptr<Node>> nodes; 
    std::vector<std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>>> connections;  
};

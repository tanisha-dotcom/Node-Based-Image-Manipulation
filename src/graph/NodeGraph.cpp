#include "NodeGraph.hpp"
#include <iostream>
#include <algorithm>

void NodeGraph::addNode(const std::shared_ptr<Node>& node) {
    nodes.push_back(node);
}

void NodeGraph::connectNodes(const std::shared_ptr<Node>& fromNode, const std::shared_ptr<Node>& toNode) {
    if (std::find(nodes.begin(), nodes.end(), fromNode) != nodes.end() &&
        std::find(nodes.begin(), nodes.end(), toNode) != nodes.end()) {
        connections.push_back(std::make_pair(fromNode, toNode));
    } else {
        std::cerr << "Invalid node connection!" << std::endl;
    }
}

void NodeGraph::run() {
    std::cout << "Node graph running with " << nodes.size() << " nodes...\n";

    for (auto& node : nodes) {
        node->process();  
    }

    for (const auto& connection : connections) {
        const auto& fromNode = connection.first;
        const auto& toNode = connection.second;
        
        toNode->setInput(fromNode->getOutput());
    }

    for (auto& node : nodes) {
        node->renderUI();  
    }
}

void NodeGraph::clear() {
    nodes.clear();
    connections.clear();
}

const std::vector<std::shared_ptr<Node>>& NodeGraph::getNodes() const {
    return nodes;
}

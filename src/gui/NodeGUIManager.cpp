#include "NodeGUIManager.hpp"
#include <imgui.h>

void NodeGUIManager::renderAllNodesUI(const std::vector<std::shared_ptr<Node>>& nodes) {
    for (const auto& node : nodes) {
        ImGui::Begin(node->name.c_str());
        node->renderUI();  // 👈 Each node defines its own ImGui layout
        ImGui::End();
    }
}

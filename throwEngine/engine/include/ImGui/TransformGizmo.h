#pragma once
//
// Created by pointerlost on 8/4/25.
//
#include <iostream>
#include <glm/glm.hpp>
#include <memory>
#include <imgui.h>
#include <ImGuizmo/ImGuizmo.h>

namespace SCENE {
    class SceneObject;
}

namespace ENGINE::UI {
    class TransformGizmo {
    public:
        void setTarget(const std::shared_ptr<SCENE::SceneObject>& obj) { m_sceneObject = obj; };

        void setOperation(ImGuizmo::OPERATION op) { m_operation = op; };

        void clear() { m_sceneObject.reset(); };

        [[nodiscard]] bool hasTarget() const { return !m_sceneObject.expired(); };

        [[nodiscard]] std::shared_ptr<SCENE::SceneObject> getTarget() const { return m_sceneObject.lock(); };

        void update(const glm::mat4& view, const glm::mat4& projection, const ImVec2& viewportSize) const;

    private:
        std::weak_ptr<SCENE::SceneObject> m_sceneObject;
        ImGuizmo::OPERATION m_operation = ImGuizmo::TRANSLATE;
    };
}


#pragma once

#include "Vector2.h"
#include "Quad.h"
#include "Renderer.h"

namespace CoolEngineName {
    struct ENGINE_API Camera {
    private:
        glm::mat4 m_ProjMat;
        glm::mat4 m_ViewMat;

        glm::vec3 m_Scale = glm::vec3(1.0f);
        glm::vec3 m_Pos;
        float m_Rotation = 0.0f;

        void m_UpdateMatrix();

    public:
        void SetPos(const glm::vec3& pos);
        void SetPos(const Vector2<float>& pos);

        void SetCamera(const Vector2<float>& pos, const Vector2<float>& offset, float scale = 1.0f, float rotation = 0.0f);
        void SetCamera(const Vector2<float>& pos, const Vector2<float>& offset, const Vector2<float>& scale, float rotation = 0.0f);

        void SetScale(float scale);

        void SetRotation(float rotation);

        void SetProj(float left, float right, float bottom, float top);

        const glm::vec3& GetPos() const;
        const glm::vec3& GetScale() const;

        float GetRotation() const;

        const glm::mat4& GetProjMat() const;

        const glm::mat4& GetViewMat() const;

        Camera();
        Camera(float left, float right, float bottom, float top);
    };
}

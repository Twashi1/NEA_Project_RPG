#include "Renderable.h"

void Renderable::m_Register(int nz)
{
    auto it = Renderer::m_Renderables.find(nz); // Get iterator to "z" key in map

    // Key didn't exist
    if (it == Renderer::m_Renderables.end()) {
        // Create a list which only contains a pointer to this instance
        Utils::List<Renderable*> new_list; new_list.Push(this);
        // Add that list at key "z"
        Renderer::m_Renderables.insert({ nz, new_list });
    }
    // Key did exist
    else {
        // At our key value, push back a pointer to this instance to the list of Renderables
        Renderer::m_Renderables.at(nz).Push(this);
    }
}

Renderable::Renderable()
    : quad(nullptr), shader(nullptr), z(0)
{}

Renderable::Renderable(std::shared_ptr<Quad> quad, Shader* shader, int z)
    : quad(quad), shader(shader), z(z)
{
    m_Register(z);
}

Renderable::Renderable(std::shared_ptr<Quad> quad, Shader* shader, Texture* texture, int z)
    : quad(quad), shader(shader), z(z), texture(texture)
{
    m_Register(z);
}

Renderable::Renderable(std::shared_ptr<Quad> quad, Shader* shader, int z, bool isGameWorld)
    : quad(quad), shader(shader), z(z), isGameWorld(isGameWorld)
{
    m_Register(z);
}

Renderable::Renderable(std::shared_ptr<Quad> quad, Shader* shader, Texture* texture, int z, bool isGameWorld)
    : quad(quad), shader(shader), z(z), texture(texture), isGameWorld(isGameWorld)
{
    m_Register(z);
}

Renderable::Renderable(std::shared_ptr<Quad> quad, Shader* shader, int z, bool isGameWorld, bool isVisible)
    : quad(quad), shader(shader), z(z), isGameWorld(isGameWorld), isVisible(isVisible)
{
    m_Register(z);
}

Renderable::Renderable(std::shared_ptr<Quad> quad, Shader* shader, Texture* texture, int z, bool isGameWorld, bool isVisible)
    : quad(quad), shader(shader), z(z), texture(texture), isGameWorld(isGameWorld), isVisible(isVisible)
{
    m_Register(z);
}

void Renderable::SetZ(int nz)
{
    // Remove ourselves from renderer temporarily
    Renderer::m_Renderables.at(z).Remove(this);
    // Add ourselves at new location
    m_Register(nz);
}

Renderable::Renderable(const Renderable& other)
    : quad(other.quad), shader(other.shader), z(other.z), texture(other.texture), isGameWorld(other.isGameWorld), isVisible(other.isVisible)
{
    m_Register(z);
}

Renderable::~Renderable()
{
    // If object has been constructed properly
    if (quad != nullptr && shader != nullptr) {
        // Remove ourselves from the list at the relevant index
        Renderer::m_Renderables.at(z).Remove(this);
    }
}

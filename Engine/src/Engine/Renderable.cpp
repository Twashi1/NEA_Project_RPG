#include "Renderable.h"

Renderable::Renderable(std::shared_ptr<Quad> quad, Shader* shader, int z)
    : quad(quad), shader(shader), z(z)
{
    auto it = Renderer::m_Renderables.find(z); // Get iterator to "z" key in map

    // Key didn't exist
    if (it == Renderer::m_Renderables.end()) {
        // Create a list which only contains a pointer to this instance
        std::vector<Renderable*> new_list = { this };
        // Add that list at key "z"
        Renderer::m_Renderables.insert({z, new_list});
    }
    // Key did exist
    else {
        // At our key value, push back a pointer to this instance to the list of Renderables
        Renderer::m_Renderables.at(z).push_back(this);
    }
}

Renderable::~Renderable()
{
    // Get list of renderables from m_Renderables
    std::vector<Renderable*>& renderables = Renderer::m_Renderables.at(z);
    // Get index of ourselves in renderables list
    auto it = std::find(renderables.begin(), renderables.end(), this);
    // If element was found
    if (it != renderables.end()) {
        // Remove from list
        renderables.erase(it);
    }
    else {
        Log("Renderable wasn't found in Renderer::m_Renderables", Utilities::ERROR::WARNING);
    }
}

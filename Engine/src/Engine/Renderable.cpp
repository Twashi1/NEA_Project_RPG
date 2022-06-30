#include "Renderable.h"

Renderable::Renderable(std::shared_ptr<Quad> quad, Shader* shader, int z)
    : quad(quad), shader(shader), z(z)
{
    auto it = Renderer::m_Renderables.find(z); // Get iterator to "z" key in map

    // Key didn't exist
    if (it == Renderer::m_Renderables.end()) {
        // Create a list which only contains a pointer to this instance
        Utils::List<Renderable*> new_list; new_list.Push(this);
        // Add that list at key "z"
        Renderer::m_Renderables.insert({z, new_list});
    }
    // Key did exist
    else {
        // At our key value, push back a pointer to this instance to the list of Renderables
        Renderer::m_Renderables.at(z).Push(this);
    }
}

Renderable::~Renderable()
{
    // Remove ourselves from the list at the relevant index
    Renderer::m_Renderables.at(z).Remove(this);
}

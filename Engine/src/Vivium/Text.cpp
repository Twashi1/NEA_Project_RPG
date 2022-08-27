#include "Text.h"

namespace Vivium {
    Ref(Shader) Text::m_DefaultShader = nullptr;
    Ref(Font) Text::m_DefaultFont = nullptr;

    void Text::m_Construct(const std::string& input_str)
    {
        strings = Utils::SplitString(input_str, "\n");

        m_FontTexture = MakeRef(Vivium::Texture, m_DefaultFont.get());
        m_FontTextureAtlas = MakeRef(Vivium::TextureAtlas, m_FontTexture, Vector2<int>{64, 64});
    }

    void Text::m_Init()
    {
        m_DefaultShader = MakeRef(Shader, "text_vertex", "text_frag");
        m_DefaultFont = MakeRef(Font, "consola.ttf");
    }

    float Text::m_GetWidth(const std::string& str) const
    {
        float width = 0.0f;

        for (char c : str) {
            Font::Character font_char = font->character_map.at(c);
            width += float(font_char.advance >> 6) * scale;
        }

        return width;
    }

    Ref(Font) Text::GetDefaultFont()
    {
        if (m_DefaultFont == nullptr) {
            LogWarn("Default font not initialised yet");
        }

        return m_DefaultFont;
    }

    void Text::SetPos(const Vector2<float>& pos)
    {
        *this->pos = pos;
    }

    Vector2<float> Text::GetPos() const
    {
        return *pos;
    }

    void Text::FixToWidth(float max_width)
    {
        std::vector<std::string> new_strings;

        int current_new_string_index = 0;

        float current_width = 0.0f;

        for (std::size_t str_idx = 0; str_idx < strings.size(); str_idx++) {
            std::string& current_string = strings[str_idx];

            for (std::size_t char_idx = 0; char_idx < current_string.size(); char_idx++) {
                char& current_char = current_string[char_idx];

                Font::Character& font_char = font->character_map.at(current_char);

                float char_width = float(font_char.advance >> 6) * scale;

                current_width += char_width;

                if (current_width > max_width) {
                    new_strings.emplace_back(1, current_char);
                    ++current_new_string_index;
                    current_width = 0.0f;
                }
                else {
                    new_strings[current_new_string_index] += current_char;
                }
            }
        }
    }

    float Text::GetWidth() const
    {
        float max_width = 0.0f;

        for (const std::string& str : strings) {
            max_width = std::max(max_width, m_GetWidth(str));
        }

        return max_width;
    }

    float Text::GetHeight() const
    {
        return strings.size() * font->max_height * scale;
    }

    void Text::Render() const
    {
        float height = GetHeight();
        float width = GetWidth();

        Vector2<float> rendering_pos = *pos;
        rendering_pos.y += (height - (font->max_height * scale)) * 0.5f;

        std::size_t total_char_count = 0;

        for (const std::string& str : strings) {
            total_char_count += str.size();
        }

        float* vertices = new float[16 * total_char_count];     std::size_t vertices_idx = 0;
        uint16_t* indices = new uint16_t[6 * total_char_count]; std::size_t indices_idx = 0;
        std::size_t batch_count = 0;

        for (const std::string& str : strings) {
            if (alignment == Alignment::MIDDLE) {
                float line_width = m_GetWidth(str);

                rendering_pos.x = pos->x - line_width * 0.5f;
            }
            else {
                rendering_pos.x = pos->x - width * 0.5f;
            }

            // TODO: better batch rendering needed!
            for (char c : str) {
                Font::Character& font_char = font->character_map.at(c);

                float x = rendering_pos.x + font_char.bearing.x * scale;
                float y = rendering_pos.y - (font_char.size.y - font_char.bearing.y) * scale;
                float w = font_char.size.x * scale;
                float h = font_char.size.y * scale;

                std::array<float, 8> tex_coords = m_FontTextureAtlas->GetCoordsArray(c);

                float left = tex_coords[0];
                float top = tex_coords[1];
                float right = left + ((float)font_char.size.x / (float)m_FontTextureAtlas->GetAtlas()->GetWidth());
                float bottom = top + ((float)font_char.size.y / (float)m_FontTextureAtlas->GetAtlas()->GetHeight());

                // Bottom left
                vertices[vertices_idx++] = x;
                vertices[vertices_idx++] = y;
                vertices[vertices_idx++] = left;
                vertices[vertices_idx++] = bottom;
                // Bottom right
                vertices[vertices_idx++] = x + w;
                vertices[vertices_idx++] = y;
                vertices[vertices_idx++] = right;
                vertices[vertices_idx++] = bottom;
                // Top right
                vertices[vertices_idx++] = x + w;
                vertices[vertices_idx++] = y + h;
                vertices[vertices_idx++] = right;
                vertices[vertices_idx++] = top;
                // Top left
                vertices[vertices_idx++] = x;
                vertices[vertices_idx++] = y + h;
                vertices[vertices_idx++] = left;
                vertices[vertices_idx++] = top;

                int stride = batch_count * 4; // Multiplying by 4 since there's four vertices per character

                indices[indices_idx++] = 0 + stride;
                indices[indices_idx++] = 1 + stride;
                indices[indices_idx++] = 2 + stride;
                indices[indices_idx++] = 2 + stride;
                indices[indices_idx++] = 3 + stride;
                indices[indices_idx++] = 0 + stride;

                ++batch_count;

                rendering_pos.x += float(font_char.advance >> 6) * scale;
            }

            rendering_pos.y -= font->max_height * scale;
        }

        static const BufferLayout layout{
            GLSLDataType::VEC4
        };

        VertexBuffer vb(vertices, batch_count * 16, layout);
        IndexBuffer ib(indices, batch_count  * 6);

        Renderer::Submit(&vb, &ib, shader.get(), m_FontTexture.get());
    }

    Text::Text()
        : pos(), font(nullptr), shader(nullptr), scale(0.0f)
    {
        LogWarn("Default constructor called! Values will not be properly initialised");
        m_Construct("");
    }

    Text::Text(const Text& other)
        : strings(other.strings), pos(other.pos), font(other.font), shader(other.shader), scale(other.scale), alignment(other.alignment)
    {
        
    }

    Text::Text(const std::string& text, const Vector2<float>& pos, float scale)
        : pos(MakeRef(Vector2<float>, pos)), font(m_DefaultFont), shader(m_DefaultShader), scale(scale)
    {
        m_Construct(text);
    }

    Text::Text(const std::string& text, const Vector2<float>& pos, Ref(Font) font, float scale)
        : pos(MakeRef(Vector2<float>, pos)), font(font), shader(m_DefaultShader), scale(scale)
    {
        m_Construct(text);
    }

    Text::Text(const std::string& text, const Vector2<float>& pos, Ref(Shader) shader, float scale)
        : pos(MakeRef(Vector2<float>, pos)), font(m_DefaultFont), shader(shader), scale(scale)
    {
        m_Construct(text);
    }

    Text::Text(const std::string& text, const Vector2<float>& pos, Ref(Font) font, Ref(Shader) shader, float scale)
        : pos(MakeRef(Vector2<float>, pos)), font(font), shader(shader), scale(scale)
    {
        m_Construct(text);
    }

    Text::~Text()
    {
    }
}
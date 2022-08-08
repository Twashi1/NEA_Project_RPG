# Table of Contents

- [ Startup guide ](#StartupGuide)
    - [ Hello World ](#HelloWorld)
- [ Rendering guide ](#RenderingGuide)
    - [ Default shaders ](#DefaultShadersList)
    - [ OO Rendering ](#OOBasedRendering)
        - [ Shaded quad ](#ShadedQuadRendering)
		- [ Textured quad ](#TexturedQuadRendering)
		- [ Texture atlas ](#TextureAtlasRendering)
- [ GUI guide ](#GUIGuide)
- [ Physics guide ](#PhysicsGuide)
- [ Serialising guide ](#SerialisingGuide)
- [ Input guide ](#InputGuide)
- [ Noise guide ](#NoiseGuide)

# Guides
<a name="StartupGuide"></a>
## Startup guide
<a name="HelloWorld"></a>
### Hello world

```c++
#include <Vivium.h>

// Set constants
const int WIDTH = 1080;
const int HEIGHT = 720;
const int FPS = 60;

using namespace Vivium;

int main(void) {
	// Initialise window, classes, etc.
	Application::Init(WIDTH, HEIGHT, FPS);

	// Create a shader
	Shader color_shader = Shader("vertex_shader.glsl", "fragment_shader.glsl");
	// Set the color of the shader
	color_shader.SetUniform3f("u_Color", 1.0f, 0.0f, 0.0f);

	// Create a quad at (300, 300), with dimensions (100, 200)
	Quad my_quad = Quad(300, 300, 100, 200);

	// Main game loop
	while (Application::IsRunning()) {
		// Begin frame
		Application::BeginFrame();

		// Render our quad using the given shader to screen
		Renderer::Submit(&my_quad, &color_shader);

		// End frame
		Application::EndFrame();
	}

	// End program
	Application::Terminate();
}
```

<a name="RenderingGuide"></a>
## Rendering guide
<a name="DefaultShadersList"></a>
### Default shaders list
A list of the default GLSL shaders
#### Vertex Shaders
- `static_texture_vertex` Drawing **textures** onto quads that **should not** be affected by the camera
    - `OUT` Texture coordinates
- `static_vertex` Drawing onto quads that **should not** be affected by the camera
- `texture_vertex` Drawing **textures** onto quads that **should** be affected by the camera
    - `OUT` Texture coordinates
- `text_vertex` For drawing text
- `world_vertex` Drawing onto quads that **should** be affected by the camera

#### Fragment Shaders
- `color_frag` Drawing a flat color
    - Provides `vec3 u_Color` for changing the color
- `grayscale_frag` Draws a texture in grayscale
    - `IN` Texture coordinates
    - Provides `sampler2D u_Texture` for manually setting the texture slot to be drawn from
	- Provides `float u_Scale` for changing the scale of the texture
- `texture_frag` Draws a texture
    - `IN` Texture coordinates
    - Provides `sampler2D u_Texture` for manually setting the texture slot to be drawn from
	- Provides `float u_Scale` for changing the scale of the texture
- `transparency_frag` For drawing a RGBA color
    - Provides `vec4 u_Color` for changing the color and transparency
- `uv_frag` Draws UV coordinates
    - `IN` Texture coordinates

<a name="OOBasedRendering"></a>
### OO based rendering
<a name="ShadedQuadRendering"></a>
#### Basic shaded quad
Create a quad for location/dimensions of what you're rendering
>```c++
>// Create a quad at [0, 0] (screen center), with dimensions 100x100px
>Quad my_quad(0.0f, 0.0f, 100.0f, 100.0f);
>```

Create a shader
>```c++
>// static_vertex: Quad will not be transformed by camera
>// color_frag: Solid color shader
>Shader my_color_shader("static_vertex", "color_frag");
> // Setting the color
>my_color_shader.SetUniform3f("u_Color", RGBColor::RED);
>```

You can then render the quad with a shader
>```c++
>Renderer::Submit(&my_quad, &my_color_shader);
>```

<a name="TexturedQuadRendering"></a>
#### Textured quad/Sprite

Creating a texture shader
>```c++
>// world_texture_vertex: Quad will be transformed by camera, and texture coords are made available to texture_frag
>// texture_frag: Will render all pixels of texture as-is
>Shader my_texture_shader("world_texture_vertex", "texture_frag");
>```
Creating a texture
>```c++
>// Create a RGBA texture from a png
>Texture my_sprite("image.png");
>```

You can then render this texture onto a quad
>```c++
>Renderer::Submit(&my_quad, &my_texture_shader, &my_sprite);
>```

<a name="TextureAtlasRendering"></a>
#### Texture atlas rendering
You can also use a texture atlas for rendering a texture
>```c++
>// Create a texture atlas from "Resources/sprites/atlas.png", with 32x32px for each sprite
>TextureAtlas my_atlas("atlas.png", {32, 32});
>
>my_atlas.Set(&my_quad, 3); // Set quad to use the 4th texture in the atlas
>Renderer::Submit(&my_quad, &my_texture_shader, my_atlas->GetAtlas().get());
>```

<a name="GUIGuide"></a>
## GUI guide
<a name="PhysicsGuide"></a>
## Physics guide
<a name="SerialisingGuide"></a>
## Serialising guide
<a name="InputGuide"></a>
## Input guide
<a name="NoiseGuide"></a>
## Noise guide

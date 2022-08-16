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
    - [ Setup ](#SerialisingSetup)
		- [ Basic types (POD) ](#PODSerialising)
		- [ Arrays ](#ArraySerialising)
		- [ Vectors ](#VectorSerialising)
		- [ User-defined types ](#UserDefined)
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

<a name="SerialisingSetup"></a>
### Setup
Create a serialiser
>```c++
>Serialiser s(Stream::Flags::BINARY | Stream::Flags::TRUNC);
>```

Open a file in read/write mode
>```c++
>s.BeginWrite("path_to_file.txt");
>```

Close a file after read/write
>```c++
>s.EndWrite();
>```

<a name="PODSerialising"></a>
#### Basic types (POD)
*Supports std::string*  

Writing is as simple as
>```c++
>int x = 5;
>serialiser.Write(x);
>```

And for reading
>```c++
>int x;
>serialiser.Read(&x);
>```

<a name="ArraySerialising"></a>
#### Arrays

For std::array

*Supports std::string*

>```c++
>std::array<int, 5> my_array = { 1, 2, 3, 4, 5 };
>serialiser.Write(my_array);
>```

>```c++
>std::array<int, 5> my_array;
>serialiser.Read(&my_array);
>```

For c-style arrays

*Doesn't support std::string*

>```c++
>int my_array[5] = { 1, 2, 3, 4, 5 };
>serialiser.Write(my_array, 5);
>```

>```c++
>int* my_array = new int[5];
>serialiser.Read(&my_array, 5);
>```

*alternatively*
>```c++
>int my_array[5];
>serialiser.Read(my_array, 5);
>```

<a name="VectorSerialising"></a>
#### Vectors
*Small memory overhead when writing vectors of 4 bytes - or sizeof(std::size_t)*  
*Supports std::string*

>```c++
>std::vector<int> my_vector = {1, 2, 3, 4, 5};
>serialiser.Write(my_vector);
>```

>```c++
>std::vector<int> my_vector;
>serialiser.Read(&my_vector);
>```

<a name="UserDefined"></a>
#### User-defined types

Defining a class that can be serialised
>```c++
>class MyStruct : IStreamable
>{
>	int x, y;
>
>	// Default constructor is useful for IStreamable::Read
>	MyStruct() {}
>	MyStruct(int x, int y) : x(x), y(y) {}
>	
>	void Read(Serialiser& s) override
>	{
>		s.Read(&x);
>		s.Read(&y);
>	}
>	void Write(Serialiser& s) const override
>	{
>		s.Write(x);
>		s.Write(y);
>	}
>}
>```

Serialising the class
>```c++
>MyStruct object(5, 3);
>serialiser.Write(object);
>```

>```c++
>MyStruct read_object{};
>serialiser.Read(&read_object);
>```

<a name="InputGuide"></a>
## Input guide
<a name="NoiseGuide"></a>
## Noise guide

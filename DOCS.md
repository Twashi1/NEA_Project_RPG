# Contents
## Examples
- [ Hello world ](#HelloWorld)

## Classes
- [ Engine class ](#EngineClass)
- [ Input class ](#InputClass)

<a name="Examples"></a>
# Examples
<a name="HelloWorld"></a>
## Hello world
<details>
<summary>Click to expand</summary>

```c++
#include <Engine.h>

// Set constants
const int WIDTH = 1080;
const int HEIGHT = 720;
const int FPS = 60;

int main(void) {
	// Initialise window, classes, etc.
	Engine::Init(WIDTH, HEIGHT, FPS);

	// Create a shader
	Shader color_shader = Shader("static_vertex", "color_frag");
	// Set the color of the shader
	color_shader.SetUniform3f("u_Color", 1.0f, 0.0f, 0.0f);

	// Create a quad at (300, 300), with dimensions (100, 200)
	Quad my_quad = Quad(300, 300, 100, 200);

	// Main game loop
	while (Engine::IsRunning()) {
		// Begin frame
		Engine::BeginFrame();

		// Render our quad using the given shader to screen
		Renderer::Schedule(&my_quad, &color_shader);

		// End frame
		Engine::EndFrame();
	}

	// End program
	Engine::Terminate();
}
```
</details>

<a name="Engine"></a>
# Engine
<a name="EngineClass"></a>
## Engine class
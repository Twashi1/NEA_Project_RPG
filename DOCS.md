# Table of Contents

- [ Startup guide ](#StartupGuide)
- [ Rendering guide ](#RenderingGuide)
- [ GUI guide ](#GUIGuide)
- [ Physics guide ](#PhysicsGuide)
- [ Serialising guide ](#SerialisingGuide)
- [ Input guide ](#InputGuide)
- [ Noise guide ](#NoiseGuide)

# Guides
<a name="StartupGuide"></a>
## Startup guide

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

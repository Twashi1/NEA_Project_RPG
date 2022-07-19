
# Examples
- [ Hello world ](#HelloWorldExample)

# Contents
##  Misc.
- [ Engine ](#EngineClass)
- [ Input ](#InputClass)
- [ Noise ](#NoiseClass)
- [ Serialiser ](#SerialiserClass)
## Maths
- [ Utils ](#UtilsClass)
- [ Rect ](#RectClass)
- [ Vector2 ](#Vector2Class)
- [ Vector3 ](#Vector3Class)
## Physics
- [ Physics ](#PhysicsClass)
- [ Body ](#BodyClass)
## GUI
- [ Button ](#ButtonClass)
- [ Slider ](#SliderClass)
- [ TextInput ](#TextInputClass)
## Rendering
- [ Renderer ](#RendererClass)
- [ Animation ](#AnimationClass)
- [ Camera ](#CameraClass)
- [ Quad ](#QuadClass)
- [ SceneObject ](#SceneObjectClass)
- [ Shader ](#ShaderClass)
- [ Text ](#TextClass)
- [ Texture ](#TextureClass)

<a name="Examples"></a>
# Examples

<a name="HelloWorldExample"></a>
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

<a name="Docs"></a>
# Docs

## Misc.

<a name="EngineClass"></a>
### Engine

<p>Wraps window, camera, physics, start-of-frame operations and end-of-frame operations</p>

#### Functions

Initialises all classes and creates a window. `fps` determines the maximum amount of frames to be displayed per second. `stats` will create text objects that show performance information that can later be displayed with `UpdateStats`
>```c++
>static void Init(int width, int height, int fps, bool stats)
>```

Terminates GLFW and all engine classes
>```c++
>static void Terminate()
>```

Changes background color
>```c++
>static void SetBGColor(const Vector3<float>&)
>```

Updates text objects for all stats information, using `player_body` to update various values such as the player's position, velocity, etc.
>```c++
>static void UpdateStats(const Body* player_body)
>```

Clears screen, and updates various manager classes with the time since the last update
>```c++
>static void BeginFrame()
>```

Checks if window has been resized, swaps window buffers, and polls performance
>```c++
>static void EndFrame()
>```

Returns if the window is still open, used for main game loop
>```c++
>static bool IsRunning()
>```

<a name="InputClass"></a>
### Input

<a name="NoiseClass"></a>
### Noise

<a name="SerialiserClass"></a>
### Serialiser

## Maths

<a name="UtilsClass"></a>
### Utils

<a name="RectClass"></a>
### Rect

<a name="Vector2Class"></a>
### Vector2

<a name="Vector3Class"></a>
### Vector3

## Physics

<a name="PhysicsClass"></a>
### Physics

<a name="BodyClass"></a>
### Body

## GUI

<a name="ButtonClass"></a>
### Button

<a name="SliderClass"></a>
### Slider

<a name="TextInputClass"></a>
### Text Input

## Rendering

<a name="RendererClass"></a>
### Renderer

<a name="AnimationClass"></a>
### Animation

<a name="CameraClass"></a>
### Camera

<a name="QuadClass"></a>
### Quad

<a name="SceneObjectClass"></a>
### Scene Object

<a name="ShaderClass"></a>
### Shader

<a name="TextClass"></a>
### Text

<a name="TextureClass"></a>
### Texture

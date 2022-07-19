
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

<details>
<summary>Click to expand</summary>

<p>Wraps window, camera, physics, start-of-frame operations and end-of-frame operations</p>

</details>

<a name="InputClass"></a>
### Input

<details>
<summary>Click to expand</summary>

</details>

<a name="NoiseClass"></a>
### Noise

<details>
<summary>Click to expand</summary>

</details>

<a name="SerialiserClass"></a>
### Serialiser

<details>
<summary>Click to expand</summary>

</details>

## Maths

<a name="UtilsClass"></a>
### Utils

<details>
<summary>Click to expand</summary>

</details>

<a name="RectClass"></a>
### Rect

<details>
<summary>Click to expand</summary>

</details>

<a name="Vector2Class"></a>
### Vector2

<details>
<summary>Click to expand</summary>

</details>

<a name="Vector3Class"></a>
### Vector3

<details>
<summary>Click to expand</summary>

</details>

## Physics

<a name="PhysicsClass"></a>
### Physics

<details>
<summary>Click to expand</summary>

</details>

<a name="BodyClass"></a>
### Body

<details>
<summary>Click to expand</summary>

</details>

## GUI

<a name="ButtonClass"></a>
### Button

<details>
<summary>Click to expand</summary>

</details>

<a name="SliderClass"></a>
### Slider

<details>
<summary>Click to expand</summary>

</details>

<a name="TextInputClass"></a>
### Text Input

<details>
<summary>Click to expand</summary>

</details>

## Rendering

<a name="RendererClass"></a>
### Renderer

<details>
<summary>Click to expand</summary>

</details>

<a name="AnimationClass"></a>
### Animation

<details>
<summary>Click to expand</summary>

</details>

<a name="CameraClass"></a>
### Camera

<details>
<summary>Click to expand</summary>

</details>

<a name="QuadClass"></a>
### Quad

<details>
<summary>Click to expand</summary>

</details>

<a name="SceneObjectClass"></a>
### Scene Object

<details>
<summary>Click to expand</summary>

</details>

<a name="ShaderClass"></a>
### Shader

<details>
<summary>Click to expand</summary>

</details>

<a name="TextClass"></a>
### Text

<details>
<summary>Click to expand</summary>

</details>

<a name="TextureClass"></a>
### Texture

<details>
<summary>Click to expand</summary>

</details>
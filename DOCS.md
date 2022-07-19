
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

#### Variables
Window width
> ```c++
>static int width
>```

Window height
>```c++
>static int height
>```

Shared ptr to camera currently being used
>```c++
>ENG_Ptr(Camera) camera
>```

Shared ptr to physics system
>```c++
>ENG_Ptr(Physics) physics
>```

<a name="InputClass"></a>
### Input
Singleton that handles all mouse/keyboard inputs

Returns a list of character codes of all printable characters entered in the last update
>```c++
>static const std::vector<unsigned int>& GetLastChars()
>```

Gets state of a key, takes a GLFW key code
>```c++
>static Input::State GetKeyState(int key)
>```

Gets state of a mouse button, takes a GLFW key code
>```c++
>static Input::State GetMouseState(int button)
>```

Gets cursor position in range `(0, 0)` to the `(window width, window height)`
>```c++
>static Vector2<float> GetCursorPos()
>```

Gets cursor position in range `(0, 0)` to `(1, 1)`
>```c++
>static Vector2<float> GetCursorUVPos()
>```

#### State
<p>Enum to store current state of a key or mouse button</p>

- `NONE` Key is currently not pressed
- `PRESS` Key is has just been pressed down
- `RELEASE` Key has just been let go
- `HOLD` Key is being held down

#### Mod
<p>Enum to store the modifiers that were held down last update</p>

- `NONE` No modifiers were pressed
- `SHIFT` Shift key was pressed down
- `CTRL` Control key was pressed down
- `ALT` Alt key was pressed down
- `SUPER` Windows button (between ctrl and alt) key was pressed down
- `CAPS_LOCK` Caps lock was pressed down
- `NUM_LOCK` Num lock was pressed down

#### KeypressLog
<p>Struct to record one instance of a key being pressed and the modifiers that were held when that key was pressed</p>

- `int key` Stores the keycode for the key pressed
- `int mods` Stores a bitmask for all modifiers pressed

##### Constructors
>```c++
>KeypressLog(int key, int mods);
>```

#### Listener
<p>Struct to record various data about a key or button</p>

- `int last_action` Stores the GLFW code for the last action (press/release)  
- `Input::State state` Stores the current state of the key  
- `float time_held` Stores the time the key has been held down for (if currently being held down)

<a name="NoiseClass"></a>
### Noise
Namespace that contains functions/classes to generate 1D and 2D noise

- `unsigned int m_Seed` seed for the noise, using the same seed will generate the same noise pattern every time  
- `float amplitude` fixed multiplier for every noise value generated  
- `int wavelength` determines how often a new noise value is generated (e.g., a wavelength of `3` will mean `Get(0)`, `Get(1)`, `Get(2)` will all return the same value

#### White
<p>Class to generate simple white noise based on a 1D or 2D coordinate</p>

##### Constructors
>```c++
>White(unsigned int seed, float amplitude, int wavelength)
>```

##### Functions
Get white noise at a 1D coordinate
> Get noise value as float from `0` to `1.0`
>>```c++
>>float Get(int x)
>>```
> Get noise value as an unsigned byte from `0` to `255`
>>```c++
>>uint8_t GetByte(int x)
>>```

Get white noise for 2D coordinate
> Get noise value as float from `0` to `1.0`
>>```c++
>>float Get(int x, int y)
>>```
> Get noise value as an unsigned byte from `0` to `255`
>>```c++
>>uint8_t GetByte(int x, int y)
>>```

Get noise values from `x` to `x + length`; returns an array of floats, of length `length`
> Get noise value as float from `0` to `1.0`
>>```c++
>>float* GetList(int x, unsigned int length)
>>```
> Get noise values as an unsigned bytes from `0` to `255`
>>```c++
>>uint8_t* GetByteList(int x, unsigned int length)
>>```

Get noise values for 2D area, with `(x, y)` as starting coordinates; returns a 2D array, with dimensions `(width, height)`
> Get noise value as float from `0` to `1.0`
>>```c++
>>float** GetList(int x, int y, unsigned int width, unsigned int height)
>>```
> Get noise values as an unsigned bytes from `0` to `255`
>>```c++
>>uint8_t** GetByteList(int x, int y, unsigned int width, unsigned int height)
>>```

#### Interpolated
<p>For generating "smooth" noise</p>

##### Constructors
>```c++
>Interpolated(unsigned int seed, float amplitude, int wavelength)
>```

##### Functions

Generate linearly interpolated noise for 1D coordinate
> Get linearly interpolated noise value as float from `0` to `1.0`
>>```c++
>>float GetLinear(int x)
>>```
> Get linearly interpolated noise value as unsigned byte from `0` to `255`
>>```c++
>>uint8_t GetByteLinear(int x)
>>```

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

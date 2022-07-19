
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

Wraps window, camera, physics, start-of-frame operations and end-of-frame operations

#### Properties
- `static int width` Window width
- `static int height` Window height
- `ENG_Ptr(Camera)` Shared ptr to camera being used
- `ENG_Ptr(Physics)` Shared ptr to physics system being used

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
Enum to store current state of a key or mouse button

- `NONE` Key is currently not pressed
- `PRESS` Key is has just been pressed down
- `RELEASE` Key has just been let go
- `HOLD` Key is being held down

#### Mod
Enum to store the modifiers that were held down last update

- `NONE` No modifiers were pressed
- `SHIFT` Shift key was pressed down
- `CTRL` Control key was pressed down
- `ALT` Alt key was pressed down
- `SUPER` Windows button (between ctrl and alt) key was pressed down
- `CAPS_LOCK` Caps lock was pressed down
- `NUM_LOCK` Num lock was pressed down

#### KeypressLog
Struct to record one instance of a key being pressed and the modifiers that were held when that key was pressed

- `int key` Stores the keycode for the key pressed
- `int mods` Stores a bitmask for all modifiers pressed

##### Constructors
>```c++
>KeypressLog(int key, int mods);
>```

#### Listener
Struct to record various data about a key or button

##### Properties
- `int last_action` Stores the GLFW code for the last action (press/release)  
- `Input::State state` Stores the current state of the key  
- `float time_held` Stores the time the key has been held down for (if currently being held down)

<a name="NoiseClass"></a>
### Noise
Namespace that contains functions/classes to generate 1D and 2D noise

#### Properties
- `unsigned int m_Seed` seed for the noise, using the same seed will generate the same noise pattern every time  
- `float amplitude` fixed multiplier for every noise value generated  
- `int wavelength` determines how often a new noise value is generated (e.g., a wavelength of `3` will mean `Get(0)`, `Get(1)`, `Get(2)` will all return the same value

#### White
Class to generate simple white noise based on a 1D or 2D coordinate

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

##### Constructors
>```c++
>White(unsigned int seed, float amplitude, int wavelength)
>```

#### Interpolated
For generating "smooth" noise

##### Functions

Get linearly interpolated noise for 1D coordinate
> Get linearly interpolated noise value as float from `0` to `1.0`
>>```c++
>>float GetLinear(int x)
>>```
> Get linearly interpolated noise value as unsigned byte from `0` to `255`
>>```c++
>>uint8_t GetByteLinear(int x)
>>```

Get linearly interpolated noise for 1D coordinate using smooth step interpolation
> Get linearly interpolated noise value as float from `0` to `1.0`
>>```c++
>>float GetSmooth(int x)
>>```
> Get linearly interpolated noise value as unsigned byte from `0` to `255`
>>```c++
>>uint8_t GetByteSmooth(int x)
>>```

Get linearly interpolated noise for 2D coordinate as float
>```c++
>float Get(int x, int y)
>```

Get fractal noise for 2D coordinate as float

- `octaves` determines how many layers of noise to generate and overlap

>```c++
>float GetFractal(int x, int y, int octaves)
>```

##### Constructors
>```c++
>Interpolated(unsigned int seed, float amplitude, int wavelength)
>```

<a name="SerialiserClass"></a>
### Serialiser
Provides classes and functions for serialising/unserialising data under namespace `Serialiser`

#### Functions
Serialises one instance of a type by writing raw data - thus only works for very simple type. This has a specialisation allowing `std::string`
>```c++
>template <typename T>
>void Serialise(Stream& s, const T& data)
>```

Unserialises one instance of a type, however type must have default constructor. This has a specialisation for `std::string`
>```c++
>template <typename T>
>T Unserialise(Stream& s)
>```

Unserialises one instance of a type, storing it in the pointer given. This has a specialisation for `std::string`
>```c++
>void Unserialise(Stream& s, T* memory)
>```

#### Stream
Provides file opening/closing functions, and used in Serialise and Unserialise functions

##### Properties
- `std::ofstream* out` Output filestream ptr
- `std::ifstream* in` Input filestream ptr

##### Functions

Opens file for unserialising
>```c++
>void BeginRead(const char* path)
>```

Opens file for serialising
>```c++
>void BeginWrite(const char* path)
>```

Closes file for unserialising
>```c++
>void EndRead()
>```

Closes file for serialising
>```c++
>void EndWrite()
>```

##### Constructors
>```c++
>Stream()
>```

#### Streamable
Base for all classes that should be able to be serialised/unserialised

##### Functions

Writes (serialises) data to stream
>```c++
>void Write(Stream& s) const
>```

Reads (unserialises) data from stream
>```c++
>void Read(Stream& s)
>```

## Maths

<a name="UtilsClass"></a>
### Utils
Provides colors, a timer, miscellaneous helper and maths functions

#### Functions
Removes every instance of `object` from `data`, then resizes `data`
>```c++
>template <typename T>
>void Remove(std::vector<T>& data, const T& object)
>```

Returns true if the given path is a file
>```c++
>bool CheckFileExists(const std::string& path)
>```

Returns true if the given path is a directory
>```c++
>bool CheckDirectoryExists(const std::string& path)
>```

Creates directory at given path
>```c++
>void CreateDirectory(const std::string& path)
>```

Rotates a `point` about a `pivot`, counterclockwise by `angle` **radians**, returns rotated point
>```c++
>Vector2<float> RotatePoint(Vector2<float> point, Vector2<float> pivot, float angle)
>```

Rotates a `point` about a `pivot`, using cosine and sine of the angle, returns rotated point
>```c++
>Vector2<float> RotatePointPrecomp(Vector2<float> point, Vector2<float> pivot, float cos_angle, float sin_angle)
>```

Calculates factorial
>```c++
>uint32_t Factorial(uint32_t n)
>```

Calculates area of triangle `ABC`
>```c++
>float TriangleArea(Vector2<float> a, Vector2<float> b, Vector2<float> c)
>```

Limits value of `val` to `max`
>```c++
>Vector2<float> ClampMax(Vector2<float> val, float max)
>```

Limits value of `val` to `min`
>```c++
>Vector2<float> ClampMin(Vector2<float> val, float min)
>```

Limits value of `val` to `[max, min]`
>```c++
>Vector2<float> Clamp(Vector2<float> val, float min, float max)
>```

Removes a substring from a string, editing `str` to reflect new value
> Takes substring as a `std::string`
>>```c++
>>void EraseSubstring(std::string& str, const std::string& substr)
>>```
> Takes substring as a `const char*`
>>```c++
>>void EraseSubstring(std::string& str, const char* substr)
>>```

Rounds value to given amount of decimal places
> Rounds a `double`
>>```c++
>>double Round(double val, int decimal_places)
>>```
> Rounds a `Vector2<float>`
>>```c++
>>Vector2<float> Round(Vector2<float> val, int decimal_places)
>>```
> Rounds a `Vector2<double>`
>>```c++
>>Vector2<double> Round(Vector2<double> val, int decimal_places)
>>```

Reads all file data at path and stores in string on heap - must delete string after to prevent memory leak
>```c++
>std::string* ReadFile(const std::string& path)
>```

Splits a string by delimiter, returning vector of each split part of string
>```c++
>std::vector<std::string> SplitString(const std::string& s, const std::string& delim)
>```

#### Colors
Singleton class that contains multiple `Vector3<float>` storing RGB data for colours. Not under `Utils` namespace

Colours provided are:

- `COLORS::BLACK` Black
- `COLORS::WHITE` White
- `COLORS::RED` Red
- `COLORS::GREEN` Green
- `COLORS::BLUE` Blue
- `COLORS::YELLOW` Yellow
- `COLORS::PURPLE` Purple
- `COLORS::ORANGE` Orange
- `COLORS::GRAY` Grey
- `COLORS::DARKGRAY` Dark grey

#### Timer
Simple timer class to get elapsed time, current time, etc.

##### Properties
- `static const std::chrono::system_clock::time_point compile_time` Stores the unix timestamp of when program was compiled

##### Functions
Get time in seconds since compilation
>```c++
>static double GetTime()
>```

Get current time as a string, format `hh:mm:ss`
>```c++
>static std::string GetTimeString()
>```

Get elapsed time since last `GetElapsed` call (requires you to construct a `Timer` object)
>```c++
>double GetElapsed()
>```

##### Constructors
Constructs setting `m_Time` to the current time - thus the next `GetElapsed` call will return elapsed time from constructing object
>```c++
>Timer()
>```

Constructs setting `m_Time` to the given value - thus the next `GetElapsed` call will return elapsed time from `start_time`
>```c++
>Timer(double start_time)
>```

<a name="RectClass"></a>
### Rect

#### Properties
- Center of rect is stored as a union of:
    - `Vector2<float> center`
    - `float x, y`

- Dimensions of rect is stored as a union of:
    - `Vector2<float> dim`
    - `float width, height`

- `float angle` Stores angle in radians

#### Functions

Gets list of vertices of `Rect`  
Returns vertices in order:

- `0` Bottom left
- `1` Bottom right
- `2` Top right
- `3` Top left

>```c++
>std::vector<Vector2<float>> GetVertices() const
>```

Gets coordinates of bottom left vertex
>```c++
>Vector2<float> BottomLeft() const
>```

Gets coordinates of bottom right vertex
>```c++
>Vector2<float> BottomRight() const
>```

Gets coordinates of top right vertex
>```c++
>Vector2<float> TopRight() const
>```

Gets coordinates of top left vertex
>```c++
>Vector2<float> TopLeft() const
>```

Checks if any of the listed vertices are within the bounds of the rect
>```c++
>bool ContainsAnyOf(const std::vector<Vector2<float>>& vertices) const
>```

Checks if any of the vertices of the rect are within the bounds of the rect
>```c++
>bool ContainsAnyOf(const Rect& rect) const
>```

Checks if point is within the bounds of the rect
>```c++
>bool Contains(const Vector2<float>& point) const
>```

Checks if two rects are intersecting
>```c++
>bool IsIntersecting(const Rect& rect) const
>```

Streamable override for writing (serialising) to stream
>```c++
>void Write(Stream& s) const
>```

Streamable override for reading (unserialising) from stream
>```c++
>void Read(Stream& s)
>```

#### Constructors
Creates `0` area rect at `(0, 0)`
>```c++
>Rect()
>```

Takes center and dimensions as `float`
>```c++
>Rect(float x, float y, float width, float height, float angle = 0.0f)
>```

Takes center and dimensions as `Vector2<float>`
>```c++
>Rect(const Vector2<float>& center, const Vector2<float>& dim, float angle = 0.0f)
>```

<a name="Vector2Class"></a>
### Vector2
Provides functions for performing mathematical operations on 2D vectors, can take any arithmetic type

#### Properties
- `T x` x value
- `T y` y value

#### Functions
Returns distance from this point to another
>```c++
>T distance(const Vector2& other) const
>```

Returns dot product of `this` and `other`
>```c++
>T dot(const Vector2& other) const
>```

Returns cross product of `this` and `other`
>```c++
>T cross(const Vector2& other) const
>```

Returns magnitude of `this`
>```c++
>T magnitude() const
>```

Normalises `this`
>```c++
>void normalise()
>```

Returns floor of `this`
>```c++
>Vector2<T> floor() const
>```

Streamable override for writing (serialising) to stream
>```c++
>void Write(Stream& s) const
>```

Streamable override for reading (unserialising) from stream
>```c++
>void Read(Stream& s)
>```

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

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

To make your own class serialisable, inherit from Vivium::IStreamable, and implement the following methods:
>```c++
>void Write(Vivium::Serialiser&) const
>```

>```c++
>void Read(Vivium::Serialiser&) const
>```

A standard implementation could look like:
```c++
#include <Vivium.h>

struct MyVector2 : Vivium::IStreamable {
	int x, y;

	void Write(Vivium::Serialiser& s) const override {
		// Defining names is not necessary, but if you are using text serialisation, it makes the file more readable
		s.Write(x, "x");
		s.Write(y, "y");
	}

	void Read(Vivium::Serialiser& s) override {
		// Defining names when using text serialisation will allow you to read the variables in a different order than they are written,
		// however this would not work for binary serialisation
		s.Read(&x, "x");
		s.Read(&y, "y");
	}
}
```

1. To serialise objects first create an instance of `Vivium::Serialiser`, also giving the mode you want to serialse data in  
*Using BINARY in this example, but TEXT is also available, so you can write data in a human-readable format*
>```c++
>Vivium::Serialiser my_serialiser(Vivium::Stream::Mode::BINARY | Vivium::Stream::Mode::TRUNC);
>```

2. Open the file you will be writing to/reading from
>```c++
>my_serialiser.BeginWrite("my_file.txt");
>```

*OR*

>```c++
>my_serialiser.BeginRead("my_file.txt");
>```

3. Call `Serialiser::Write` or `Serialiser::Read` to serialise your data

>```c++
>my_serialiser.Write(8);
>my_serialiser.Write("some text");
>my_serialiser.Write(Vivium::Vector2<int>(5, 4));
>```

*OR*

>```c++
>int my_int;
>my_serialiser.Read(&my_int);
>std::string my_text;
>my_serialiser.Read(&my_text);
>Vivium::Vector2<int> my_vec2;
>my_serialiser.Read(&my_vec2);
>```

4. Close the file
>```c++
>my_serialiser.EndWrite();
>my_serialiser.EndRead();
>```

<a name="InputGuide"></a>
## Input guide
<a name="NoiseGuide"></a>
## Noise guide

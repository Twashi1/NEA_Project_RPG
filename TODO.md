# Vivium
## Application
- Change `PollPerformance`
- Initialisation functions for various classes should be private, and instead should make Application their friend

## Renderer
- [x] Framebuffers
    - Allow user to render to frame buffer
    - Allow user to render from frame buffer
- [ ] Compute shader API
- [ ] Scene API
- [x] Camera
- [ ] Color type
    - Should support RGB, HEX, HSV
    - Shader SetUniform should support it
    - Have statics for some common colours

## GUI
- [x] Buttons
- [x] Sliders
    - Add feature to make slider snap to intervals
- [x] Input boxes
    - Typing bar still weird
- [ ] Toggle switches
    - Implementation
- [ ] Panels
    - Add other GUI objects to Anchor overload
    - Add resizing
    - Add better rendering? (and bar on top?) (maybe use Panel as base class for RenderablePanel)

## General
- [ ] Pathfinding
- [ ] Ambient particles
- [ ] Sound API
- [ ] Physics system
    - Impulse resolution
- [x] Input system
    - Some function names are pretty bad
- [x] Serialiser
    - Implement IStreamable overrides for some types (Rect, Quad, etc.)
    - Add text serialisation (JSON? Custom format?)
    - Add serialise helper functions for vectors, arrays, maybe even generalised for containers

# Game
## World
- [ ] Terrain generation
- [ ] Save files
- [ ] Animating tiles

## Gameplay
- [ ] Crafting
- [ ] Complex movement
- [ ] Inventory

## Enemies
- [ ] Enemy AI
- [ ] Enemy sprites/animation

## GUI
- [ ] Control config
- [ ] Options menu

## Player
- [ ] Player sprites/animation

# Other
- ECS!!!!!!!
- Some files include more than they need to (like including `Utils.h` when all you need is `Core.h`)
- Still major CPU bottleneck
- Sleep in engine `Application::EndFrame`
- Better link to Resources folder/re-do file paths and file extension statics
- Input box typing bar size is using magic numbers
- Default statics are ugly
- Make functions private/only accessible in engine
- Better api for shader/texture/texture-atlas
- Cursor no longer switches when hovering text input box
- docs docs docs docs docs docs docs docs
- Inconsistent data layout between GUI objects
- inl file for Vector2/3

# Bugs
- First frame of animations shows entire texture atlas
- (Unconfirmed) Interpolated 2D noise seems to break on region boundaries
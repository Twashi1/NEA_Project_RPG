# Vivium
## Application
- [x] Initialisation functions for various classes should be private, and instead should make Application their friend

## Renderer
- [x] Framebuffers
    - [x] More helper functions for rendering from frame buffers
    - [x] Use framebuffers for drawing at specific z levels
- [ ] Batch rendering API (kinda done)
- [ ] Compute shader API
- [ ] Scene API
    - [ ] Load scene/Unload scene
    - [ ] Scenes are stored as text-serialised files which contain quads, shaders, textures, etc.
    - [ ] Each of these components is linked to an entity, also defined in the scene file
- [x] Camera
- [x] Color type
- [ ] Rendering API for entities
    - [ ] Add a tag to an entity, and given the entity has the requires components for the tag, it will render that entity on the next Renderer::Update() call

## GUI
- [x] Buttons
- [x] Sliders
    - [ ] Add feature to make slider snap to intervals
- [x] Input boxes
    - [ ] Typing bar still using magic numbers
- [ ] Panels
    - [ ] Add other GUI objects to Anchor overload
    - [ ] Add resizing
    - [ ] Fix bugs with Text and TextInput panels
    - [ ] Add better rendering (bar on top with text; maybe use Panel as base class for RenderablePanel)

## General
- [ ] Pathfinding
- [ ] Ambient particles/Particle system
- [ ] Sound API
- [ ] Physics system
    - [ ] Impulse resolution
- [x] Input system
    - [ ] Fix bad function names
- [x] Serialiser
    - [x] Binary serialisation
    - [ ] Add text serialisation (JSON? Custom format?)
    - [ ] Add serialise helper functions generalised for containers
    - [ ] Add serialise helper function for map and unordered_map

# Game
## World
- [ ] Terrain generation
    - [ ] Fix messy world generation code
    - [ ] Better structure generation
    - [ ] More versatile texture coords for tile atlas
- [ ] Save files
    - [ ] Implement saving to/loading from file
- [ ] Animating tiles / Drawing animated tiles
- [x] Batch render tiles
- [x] Drawing selection hover
- [ ] Items/Floor drops
- [ ] Tiles should cleanly connect to each other
    - [ ] Draw possible transitions
- [x] Tile breaking
    - [x] Implement selecting a tile to break and holding lmb to start timer
    - [ ] Only decor/top tiles can be broken
    - [x] Tile will grow and shrink rapidly when being broken, until being removed and being converted into an item

## Gameplay
- [ ] Crafting
- [ ] Complex movement
- [ ] Inventory
    - [ ] Hotbar
    - [ ] Keyboard shortcuts

## Enemies
- [ ] Enemy AI
- [ ] Enemy sprites/animation

## GUI
- [ ] Title screen
    - [ ] Navigate to options, create new world, load world
- [ ] Control config
- [ ] Options menu

## Player
- [ ] Player sprites/animations

# Other
- ECS API
- Sleep in engine `Application::EndFrame`
- Better link to Resources folder/re-do file paths and file extension statics
- Input box typing bar size is using magic numbers
- Default statics are ugly
- Better api for shader/texture/texture-atlas
- Inconsistent data layout between GUI objects
- GL id should be private for GL abstractions
- Allow void* user data for callback functions (mostly done?)
- Batch rendering API (mostly done)
- Change performance stats in Application to use a running average
- docs docs docs docs docs docs docs docs
- INL file for Vector2
- Physics system is still has massive performance issues, also not accurate anymore

# Bugs
- Cursor no longer switches when hovering text input box
- Renderer::BeginScene and EndScene interfere with transparency (Application clear color is mixed in?)
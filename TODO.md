# Vivium
## Application
- Initialisation functions for various classes should be private, and instead should make Application their friend

## Renderer
- [x] Framebuffers
    - More helper functions for rendering from frame buffers
    - ~~Use framebuffers for z levels~~
- [ ] Batch rendering API (kinda done)
- [ ] Compute shader API
- [ ] Scene API
    - Load scene/Unload scene
    - Scenes are stored as text-serialised files which contain quads, shaders, textures, etc.
    - Each of these components is linked to an entity, also defined in the scene file
- [x] Camera
- [x] Color type
- [ ] Rendering API for entities
    - Add a tag to an entity, and given the entity has the requires components for the tag, it will render that entity on the next Renderer::Update() call
    - Figure out how to implement z levels for entities being drawn (individual framebuffers for each entity with a different z level seems pretty inefficient, but also the only viable solution other than a long sort)

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
    - Fix bugs with Text and TextInput panels
    - Add better rendering? (and bar on top?) (maybe use Panel as base class for RenderablePanel)

## General
- [ ] Pathfinding
- [ ] Ambient particles/Particle system
- [ ] Sound API
- [ ] Physics system
    - Impulse resolution
- [x] Input system
    - Some function names are pretty bad
- [x] Serialiser
    - Add text serialisation (JSON? Custom format?)
    - Add serialise helper functions for vectors, arrays, maybe even generalised for containers

# Game
## World
- [ ] Terrain generation
- [ ] Save files
    - Implement saving to/loading from file
- [ ] Animating tiles
- [x] Batch render tiles
- [ ] Drawing selection hover
- [ ] Items/Floor drops
- [ ] Tiles should cleanly connect to each other
- [ ] Tile breaking
    - Implement selecting a tile to break and holding lmb to start timer
    - Only decor/top tiles can be broken
    - Tile will grow and shrink rapidly when being broken, until being removed and being converted into an item

## Gameplay
- [ ] Crafting
- [ ] Complex movement
- [ ] Inventory
    - Hotbar
    - Keyboard shortcuts

## Enemies
- [ ] Enemy AI
- [ ] Enemy sprites/animation

## GUI
- [ ] Title screen
    - Navigate to options, create new world, load world
- [ ] Control config
- [ ] Options menu

## Player
- [ ] Player sprites/animations

# Other
- ECS
- Sleep in engine `Application::EndFrame`
- Better link to Resources folder/re-do file paths and file extension statics
- Input box typing bar size is using magic numbers
- Default statics are ugly
- Make static initialise functions private/only accessible in engine
- Better api for shader/texture/texture-atlas
- Cursor no longer switches when hovering text input box
- Inconsistent data layout between GUI objects
- GL id should be private for GL abstractions
- Allow void* user data for callback functions (mostly done?)
- Batch rendering API (mostly done)
- docs docs docs docs docs docs docs docs

# Bugs
# Vivium
## Application
- [x] Initialisation functions for various classes should be private, and instead should make Application their friend

## Renderer
- [ ] `IRenderable`, implements Render
- [x] `IBatchable` implements `Submit(Batch*)`
- [ ] "TextureLikeType", union of Texture, TextureAtlas, Framebuffer, implements texture-like api, its union types should be able to implicitly last to and from the type
- [x] Framebuffers
    - [x] More helper functions for rendering from frame buffers
    - [x] Use framebuffers for drawing at specific z levels
- [x] Batch rendering API
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
- [x] Text
    - [x] Batch render characters for text
    - [x] Align text to left or center
- [x] Sliders
    - [ ] Add feature to make slider snap to intervals
- [x] Input boxes
    - [ ] Typing bar still using magic numbers
- [x] Panels
    - [x] Add other GUI objects to Anchor overload
    - [ ] Add resizing
    - [x] Fix bugs with Text and TextInput panels
- [ ] GUI objects should not be responsible for the rendering methods, only giving the correct position data for the object

## General
- [ ] Pathfinding
- [ ] Ambient particles/Particle system
- [ ] Sound API
    - [x] Sound library
- [ ] Physics system
    - [x] Implement broad phase
    - [ ] Impulse resolution
- [x] Input system
    - [ ] Probably needs an API rework
    - [ ] Fix bad function names
- [x] Serialiser
    - [x] Binary serialisation
    - [ ] Add text serialisation (JSON? Custom format?)
    - [ ] Add serialise helper functions generalised for containers
    - [x] Add serialise helper function for map and unordered_map

# Game
## World
- [x] Small world map in corner
    - ~~Fix performance issues~~
    - ~~Fix ugly black border for unloaded chunks~~
- [x] World loads around player
    - [x] Multithreading to stop lagspike when loading new regions (mostly done, serialising regions should also be multithreaded)
- [x] Terrain generation
    - [ ] Better height map, weights for tiles, etc.
    - [x] Fix messy world generation code
    - [x] Better structure generation
    - [x] More versatile texture coords for tile atlas
- [x] Save files
    - [x] Implement saving to/loading from file
- [ ] Animating tiles / Drawing animated tiles
- [x] Batch render tiles
- [x] Physics for tiles
    - Better system for adding physics objects
- [x] Drawing selection hover
- [x] Items/Floor drops
     - [x] Items with counts of 2 will display 2 copies of the item texture, and counts of 3 or above will display 3 copies of the item texture
     - [x] Items should move slightly immediately after being broken
- [ ] Tiles should cleanly connect to each other
    - [ ] Draw possible transitions
    - [ ] Create logic to select appropriate transition
- [x] Tile breaking
    - [x] Implement selecting a tile to break and holding lmb to start timer
    - [ ] Only decor/top tiles can be broken
    - [x] Tile will grow and shrink rapidly when being broken, until being removed and being converted into an item

## Gameplay
- [ ] Crafting
    - [x] Recipes system
        - [ ] Speed up by precomputing some values in Inventory (item counts)
    - [ ] Crafting UI
- [ ] Complex movement
     - [ ] Double tap to dash
- [ ] Inventory
    - [x] Hotbar
    - [ ] Some sort of "pending item changes"
    - [ ] Keyboard shortcuts

## Enemies
- [ ] Enemy AI
- [ ] Enemy sprites/animation

## GUI
- [x] Title screen
    - [x] Navigate to options, create new world, load world
- [ ] Control config
- [ ] Options menu

## Player
- [ ] Player sprites/animations

# Other
- ECS API
- ~~Tiny little map in the corner is taking up majority of performance~~
- Better link to Resources folder/re-do file paths and file extension statics
- Input box typing bar size is using magic numbers
- Default statics are ugly
- Inconsistent data layout between GUI objects
- docs docs docs docs docs docs docs docs
- INL file for Vector2

# Bugs
- ~~Going fullscreen crashes game~~
- Can't move items between hotbar slots unless inventory is opened
- ~~Cursor no longer switches when hovering text input box~~
- Renderer::BeginScene and EndScene interfere with transparency (Application clear color is mixed in?)
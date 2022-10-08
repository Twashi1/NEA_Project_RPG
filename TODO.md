# Engine (Vivium)
## General
- [x] Event handler https://stackoverflow.com/questions/9711414/what-is-the-proper-way-of-doing-event-handling-in-c
    - [ ] Prefer event handler to callback functions
    - [ ] Use integer ID to differentiate between events
- [ ] Update and Render methods could have a virtual behind them, also splitting them is inconvenient for a lot of classes 
- [ ] Vivium Printable? would make some things bigger though
- Consistent style when overriding function

## Application
- [x] Initialisation functions for various classes should be private, and instead should make Application their friend
- [ ] Make stats information available through struct

## Renderer
- [ ] Look into vertex arrays
- [ ] Rendering API for only vertices, no indices
- [ ] `IRenderable`, implements Render
- [x] `IBatchable` implements `Submit(Batch*)`
- [ ] "TextureLikeType", union of Texture, TextureAtlas, Framebuffer, implements texture-like api, its union types should be able to implicitly last to and from the type
- [x] Framebuffers
    - [x] More helper functions for rendering from frame buffers
    - [x] Use framebuffers for drawing at specific z levels
- [x] Batch rendering API
    - [ ] API for batching raw vertices without indices
- [ ] Compute shader API
- [x] Camera
- [x] Color type
- [ ] Default shaders accessible through an enum or something similar

## GUI
- [x] Buttons
- [x] Text
    - [x] Batch render characters for text
    - [x] Align text to left or center
- [x] Sliders
    - [ ] Add feature to make slider snap to intervals
- [x] Input boxes
    - [ ] Typing bar using magic numbers
- [x] Panels
    - [x] Add other GUI objects to Anchor overload
    - [ ] Add resizing
    - [x] Fix bugs with Text and TextInput panels
- [ ] GUI objects should not be responsible for the rendering methods, only giving the correct position data for the object

## General
- [x] Pathfinding
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
## General
- ~~Switch to 16x16~~
- [ ] Tileset https://www.gamedev.net/tutorials/_/technical/game-programming/tilemap-based-game-techniques-handling-terrai-r934/
- [ ] Some way to escape game and go back to main menu/access settings/etc

## World
- ~~Remove bot/mid/top for tiles, just foreground/background~~
- [x] Small world map in corner
    - ~~Fix performance issues~~
    - ~~Fix ugly black border for unloaded chunks~~
- [x] World loads around player
    - [x] Multithreading to stop lagspike when loading new regions (mostly done, serialising regions should also be multithreaded)
- [x] Terrain generation
    - Fix tall cacti not spawning
    - [ ] Better height map, weights for tiles, etc.
    - [ ] Fix messy world generation code
    - [x] Structure generation system
        - [ ] Regions should store the structures inside them/enable structure generation on region borders
    - [x] More versatile texture coords for tile atlas
- [x] Save files
    - [x] Implement saving to/loading from file
- [ ] Animating tiles / Drawing animated tiles
- [x] Batch render tiles
- [ ] Some tiles/parts of tiles should render in front of the player (maybe with some slight opacity or some pixel mask)
- [x] Physics for tiles
    - ~~Better system for adding physics objects~~
- [x] Drawing selection hover
- [x] Items/Floor drops
     - [x] Items with counts of 2 will display 2 copies of the item texture, and counts of 3 or above will display 3 copies of the item texture
     - [x] Items should move slightly immediately after being broken
- [x] Tile breaking
    - [x] Implement selecting a tile to break and holding lmb to start timer
    - [x] Tile will grow and shrink rapidly when being broken, until being removed and being converted into an item

## Gameplay
- [ ] Play block breaking sound in sync with block breaking "animation"
- [x] Crafting
    - [x] Recipes system
        - [x] Speed up by precomputing some values in Inventory (item counts)
    - [x] Crafting UI
- [ ] Double tap to dash
- [ ] Inventory
    - [x] Hotbar
    - [ ] Some sort of "pending item changes"
    - [ ] Keyboard shortcuts
    - [x] Selected item slot
    - [ ] Sync up better with crafting inventory?
- [ ] Weapons
    - [x] Weapon rendering
    - [x] Weapons fire projectiles
    - [ ] Weapons damage enemies
- [ ] Spell system? Buff system? maybe this is useless but i really wanna use those magic circle sprites

## NPCs
- [ ] Random spawning even after world load 

## Enemies
- [ ] Enemy AI
- [ ] Enemy sprites/animation

## GUI
- [x] Title screen
    - [x] Navigate to options, create new world, load world
    - [ ] Show environment in background, with some blur and movement
- [ ] Keybinds config
- [x] Options menu

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
- Finish scoping everything
- ~~Match colour of the amethyst wand, ball, and sword to amethyst crystal/sword, or other way round~~
- 

# Bugs
- Loading a world doesn't load NPCs/kills NPCs?
- Tall cacti not spawning in desert biome
- ~~Topaz wand not working~~
- ~~Rare bug where a Vector2 which is an index for a loaded region is unallocated and checked for equivelance~~
- ~~Going fullscreen crashes game~~
- ~~Can't move items between hotbar slots unless inventory is opened~~
- ~~Cursor no longer switches when hovering text input box~~
- Renderer::BeginScene and EndScene interfere with transparency (Application clear color is mixed in?)
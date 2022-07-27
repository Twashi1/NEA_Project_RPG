# Engine
## Renderer
- [ ] Post-processing
- [ ] Framebuffers
- [ ] Compute shader API
- [ ] Scene API
- [x] Camera

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
- [x] Input system
- [x] Serialiser
    -  API still needs work

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
- Cleanup
- Still major CPU bottleneck
- Sleep in engine EndFrame
- Better link to Resources folder/re-do file paths and file extension statics
- Better name for class Text
- Input box typing bar size is using magic numbers
- Default statics are ugly
- Colors should be put under utils namespace
- Make functions private/only accessible in engine
- First frame of animations shows entire texture atlas
- Better api for shader/texture/texture-atlas (clump them into one struct?)
- Cursor no longer switches when hovering text input box
- Update docs
- Inconsistent data layout between GUI objects
- Rename engine
- Create global namespace
- Interpolated 2D noise seems to break on region boundaries?
- inl file for Vector2/3
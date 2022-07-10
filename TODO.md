# Engine
## Renderer
- [ ] Post-processing
- [ ] Framebuffers

## GUI
- [ ] Slides
- [ ] Input boxes
- [ ] Panels

## General
- [ ] Pathfinding
- [ ] Ambient particles
- [ ] Sound API
- [ ] Physics system

# Game
## World
- [ ] Terrain generation
- [ ] Save files

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
- Texture atlas API sometimes leaves stay pixels on side of image
- Objects aren't being deconstructed properly, so program can't properly exit
- Fix sandbox
- Move player to Game/Sandbox
- World render around still not working (actually basically entirety of world isn't working)
- Make quad ib a static
- Better link to Resources folder
- While updating a body in the physics system will update its rect, the VertexBuffer of the quad that contains that rect is not updated, thus the object will not appear as having moved
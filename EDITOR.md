# Bound Engine - Editor Controls

## Editor Mode

**Press TAB** to toggle between PLAY and EDITOR modes.

### Editor Controls

Once in EDITOR mode, you can manipulate selected objects:

#### Movement (IJKL)
- **I** - Move forward (Z+)
- **K** - Move backward (Z-)
- **J** - Move left (X-)
- **L** - Move right (X+)
- **U** - Move up (Y+)
- **O** - Move down (Y-)

#### Rotation (Arrow Keys)
- **LEFT/RIGHT** - Rotate around Y axis (yaw)
- **UP/DOWN** - Rotate around X axis (pitch)

#### Scale (+/-)
- **PLUS (+)** - Increase scale
- **MINUS (-)** - Decrease scale

#### Object Management
- **X** - Delete selected object

### Keyboard Shortcuts (TODO)
- **C** - Spawn Cube
- **P** - Spawn Pyramid
- **F** - Spawn Floor
- **S + CTRL** - Save Level (Assets/Levels/Level1.data)
- **L + CTRL** - Load Level

### Debug Output
- Object spawns, selection, and deletion are logged to Debug Output window
- Selected object info shows: ID, Position, Rotation, Scale

## Play Mode

Normal game controls apply:
- **W/S** - Move forward/backward
- **Q/E** - Strafe left/right
- **Space** - Move up
- **Shift** - Move down
- **A/D** - Rotate left/right
- **Right Mouse Button** - Free look

---

**Note:** Currently transforming editor objects in real-time. Next step: Add mesh spawning commands and level persistence.

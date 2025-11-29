todo: some effect documentation is missing or outdated

The Sequence structure (Sequence.h) contains:
- Vector of keyframes 
- A name (const char*)
- Transition (OverlaySequence*) (default nullptr)
- Minimum wait duration (default 0)
- Maximum wait duration (default 0)

Keyframe (Keyframe.h) structure:
- Effects (vector <Effect*>)  (default empty)
    Effects to apply when displaying the current keyframe (explained below)
- Bitmap draws (vector <BitmapDraw>) (default empty)
    BitmapDraws specify what bitmaps to draw on which displays by bitmask.
    These are copied to a temporary buffer. Effects and overlays without
    BITMAPBUFFER in their display bitmask get overlayed on top, otherwise
    they will be applied directly to the bitmap.
- Keyframe duration (default set in config)
- Next keyframe index (default -1)
    What keyframe to jump to next.
    Value of -1 goes to the next one.
    Value of -2 repeats the current one indefinitely.
    When the keyframe index goes out of range the animation stops.
- Repeat count (default 0)
    How many times should the current keyframe be repeated.

Effects explained:
- Effect code is in Drawing/Effects directory. Most effects require a display bitmask
  argument that specifies which display or group of displays to apply the effect to.
  Display bitmasks are assigned when registering displays in main.cpp.
  Check available bitmasks and groups in config.h.
  Combine bitmasks by using | (OR) bitwise operator: EYE_L | MOUTH_L.
- Mirror
    Mirrors the bitmap.
- VerticalMirror
    Mirrors the bitmap vertically.
- Invert
    Inverts the bits of a bitmap.
- Glitch
    Shifts rows by a random value. 
    Parameters:
    Intensity (0 - 100) 
    Maximum shift amount (0 - 8)
- TVStatic
    Toggle random bits, makes the bitmap "noisy". 
    Parameters:
    Intensity (0 - 100) 
- EmitParticles
    Emits particles at given position.
    Parameters:
    count, x, y, minXSpeed, maxXSpeed, minYSpeed, maxYspeed
    Note: Speed values are divided by 100.
- ParticleColumn
    Creates a column of particles according to bits of an integer (value)
    Parameters:
    count, column, value, minXSpeed, maxXSpeed, minYSpeed, maxYspeed 
- ClearParticles
    Clears particles.
- Rotate90
    Rotates the bitmap by 90 degrees.
    Parameters:
    direction (true to rotate in the other direction)
- Rotate180
    Rotates the bitmap by 180 degrees.
- ExpandY
    Expands a column by n pixels
    Parameters:
    column, amount
- Clear
    Clears the bitmap.
- SetPixel
    Sets a pixel to a value at a selected x, y coordinate
    Parameters:
    x, y, state
- SetRow
    Sets an entire row to an 8-bit integer value
    Parameters:
    row, value
- SetColumn
    Sets an entire column to an 8-bit integer value
    Parameters:
    column, value
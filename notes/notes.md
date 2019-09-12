# Notes about wideangle view implementation efforts

## 2019-08-31 -> 2019-09-01

Open the in game console with `shift + esc`.s

Fisheye should be implemented as enablable and configurable from the graphics options menu.

Interesting files:
- similar/main/render.cpp
- similar/main/gamerend.cpp
- similar/arch/ogl/ogl.cpp
- common/2d/canvas.cpp
- similar/main/game.cpp


Draw to canvas or frame buffer? What is a canvas really?

Line 131 of similar/main/game.cpp says
`grs_canvas	Screen_3d_window;							// The rectangle for rendering the mine to`
i.e. it declares the canvas to draw the game to.
`grs_canvas` and `grs_bitmap` are defined in common/include/gr.h
`grs_canvas` has a `grs_bitmap` as a property


Line 763 of similar/main/gamerend.cpp defines the function `game_render_frame_mono` with the comment "render a frame for the game". The function calls `gr_set_current_canvas(Screen_3d_window);` seemingly setting the canvas on which to draw the 3d world, and in the rest of function it seems to also draw the HUD and possibly other things on the same canvas.

Line 94 of common/2d/canvas.cpp defines the function `gr_set_current_canvas` it seems. The function sets the pointer `grd_curcanv` to the canvas passed to the function. The pointer is declared on line 34 of the same file.

`game_render_frame_mono` calls `render_frame`, a function defined in similar/main/render.cpp which obviously renders the frame. However, before it does that it calls `update_rendered_data` which seems to set the camera data, and probably a lot of other things, for the renderer. It sets the "viewer" property of the window object.

IDEA: At the same places as allocating and modifying `Screen_3d_window`, allocate and modify other canvases for the wideangle cube face canvases.

`object_base` defined in common/main/object.h has the properties `pos` and `orient` for the position and orientation of any object.

The missile view and guide bot view are drawn after the HUD as rectangular "extra views". If the player has CM_FULL_COCKPIT selected the HUD is then drawn again so that the non-rectangular HUD windows crop the extra views.

common/3d/globvars.cpp contains shared variables for the 3D rendering. ^^

I must make sure that the 90x90 degree renderings are not cropped.
Is it possible to render to offscreen buffers, and then from them to the screen?

The extra views rendering might give clues to how to draw the wideangle cube faces, and possibly how to draw them to other canvases and then "copy" them onto the main canvas.

The function `gr_bitmap` in common/2d/bitblt.cpp seems to copy information from a bitmap into a canvas (into the bitmap of that canvas, I think) at the specified coordinates.

**Alright**, I managed to render all six faces of the visual cube by creating subcanvases from the main canvas and calling the original render function once for each face with different orientations and subcanvases.

## 2019-09-06

`gr_for_each_bitmap_byte` in common/2d/bitblt.cpp might be promising.

Hmm, maybe I should just put all of it inside "IFDEF OGL" (you know what I mean) and use an OGL fragment shader to draw to the canvas from a cubemap of the visual cube faces. That is quite possibly the best way.

[Here](https://learnopengl.com/Advanced-OpenGL/Cubemaps) is a tutorial for cubemaps in opengl.

Lines 1924-1940 in similar/arch/ogl/ogl.cpp seem to show how to draw a texture to a rectangle with OpenGL.

## 2019-09-11
[This commit](https://github.com/dxx-rebirth/dxx-rebirth/commit/db267af6f2696825e0acbdbedb321c82cfcfa639) introduces OpenGL extension loading. Perhaps I could load glCreateShader and the rest of the functions I need in the same way as 
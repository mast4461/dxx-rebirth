R""(
attribute vec2 a_position;
uniform vec2 u_resolution;
varying vec2 v_texCoord;

void main() {
  float s = 2.0;

  float w = u_resolution.x;
  float h = u_resolution.y;
  vec2 m = vec2(1, 1);
  if (w < h)
  {
    m = vec2(1, w/h);
  }
  else if (w > h)
  {
    m = vec2(h/w, 1);
  }

  gl_Position = vec4(a_position * vec2(1, -1), 0, 1);

  // pass the texCoord to the fragment shader
  // The GPU will interpolate this value between points.
  v_texCoord = a_position / m / vec2(s, s);
}
)""
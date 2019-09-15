R""(
#define PI 3.1415926535897932384626433832795
#define ZOOM 0.5

// our textures
uniform samplerCube u_cubemap;

// the texCoords passed in from the vertex shader.
varying vec2 v_texCoord;

void main() {
  // Extract relative screen coordinates from texCoord vector
  float u = v_texCoord.x;
  float v = v_texCoord.y;

  // Calculate spherical coordinates
  float phi = atan(v, u);
  float theta = PI * sqrt(u*u + v*v);

  // Convert spherical coordinates to cartesian coordinates
  float minusSinTheta = -sin(theta);
  float xt = minusSinTheta*cos(phi);
  float yt = minusSinTheta*sin(phi);
  float zt = cos(theta);

  // Sample cubemap
  gl_FragColor = textureCube(u_cubemap, vec3(xt, yt, zt + ZOOM));
}
)""
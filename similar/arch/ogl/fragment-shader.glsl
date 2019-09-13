R""(
#define TAU 6.283185307179586 // Two pi

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
  float theta = TAU * sqrt(u*u + v*v);

  // Convert spherical coordinates to cartesian coordinates
  float minusSinTheta = -sin(theta);
  float xt = minusSinTheta*cos(phi);
  float yt = minusSinTheta*sin(phi);
  float zt = cos(theta);

  // Sample cubemap
  gl_FragColor = vec4(textureCube(u_cubemap, vec3(xt, yt, zt)).xyz, 0.5);
  //gl_FragColor = vec4(0.5 + xt *0.5, 0.5 + yt * 0.5, 0.5 + zt * 0.5, 0.5);
}
)""
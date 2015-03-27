//////////////////////////////////////////////////////////////////////////////////////////
//
// default frament shader for textures
//

// constant parameters
uniform sampler2D top;
uniform sampler2D topmask;

// inputs
varying vec2 uv_;

void main() {
  vec4 background = texture2D(topmask, uv_);
  vec4 foreground = texture2D(top, uv_);
  gl_FragColor = vec4(foreground.rgb, background.r);
  if (background.r < 0.5) discard;
}


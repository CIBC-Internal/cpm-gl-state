
#ifndef IAUNS_GL_STATE_H
#define IAUNS_GL_STATE_H

/// \todo Handle state relative to some default state?

#include <cstddef>
#include <gl-platform/GLPlatform.hpp>

namespace CPM_GL_STATE_NS {

#ifndef CPM_GL_STATE_MAX_TEXTURE_UNITS
#define CPM_GL_STATE_MAX_TEXTURE_UNITS 12
#endif

/// Front or back face culling.
enum STATE_CULL 
{
  CULL_FRONT,
  CULL_BACK
};

enum CULL_ORDER
{
  ORDER_CCW,
  ORDER_CW
};

enum BLEND_FUNC 
{
  BF_ZERO,
  BF_ONE,
  BF_SRC_COLOR,
  BF_ONE_MINUS_SRC_COLOR,
  BF_DST_COLOR,
  BF_ONE_MINUS_DST_COLOR,
  BF_SRC_ALPHA,
  BF_ONE_MINUS_SRC_ALPHA,
  BF_DST_ALPHA,
  BF_ONE_MINUS_DST_ALPHA,
  BF_SRC_ALPHA_SATURATE
};

enum BLEND_EQ {
  BE_FUNC_ADD,
  BE_FUNC_SUBTRACT,
  BE_FUNC_REVERSE_SUBTRACT,
  BE_MIN,
  BE_MAX
};

/// Settings for the depth function.
enum DEPTH_FUNC {
  DF_NEVER,
  DF_LESS,
  DF_EQUAL,
  DF_LEQUAL,
  DF_GREATER,
  DF_NOTEQUAL,
  DF_GEQUAL,
  DF_ALWAYS
};

/// Represents a one of a multitude of OpenGL states.
/// In order for changes to be reflected on the GPU, state must be modified
/// through the StateManager. This class only represents a snapshot of some GPU 
/// state. It knows nothing about the current state of the GPU.
class GLState
{
public:

  GLState() :
      mDepthTestEnable(true),
      mDepthFunc(DF_LESS),
      mCullFaceEnable(false),
      mCullState(CULL_BACK),
      mCullOrder(ORDER_CCW),
      mBlendEnable(true),
      mBlendEquation(BE_FUNC_ADD),
      mBlendFuncSrc(BF_SRC_ALPHA),
      mBlendFuncDst(BF_ONE_MINUS_SRC_ALPHA),
      mDepthMask(true),
      mColorMask(true),
      mLineWidth(2.0f),
      mLineSmoothing(false),
      mTexActiveUnit(0)
  {}

  // Use default copy constructor, we are safe directly copying array.

  ~GPUState() {}

  /// Applies OpenGL state immediately. It does not check the current OpenGL
  /// state, or any other GLState. It applies the entire GLState blindly.
  /// Use this when you want a clean OpenGL state.
  void apply() const;

  /// Applies this state relative to another GLState (state), which represents
  /// the current OpenGL state.
  void applyRelative(const GLState& state) const;

  /// Attempts to detect errors in the OpenGL state (invalid state settings).
  /// Returns true if the state was verified, otherwise false is returned.
  /// and \p errorString , if given, is populated with a specific error.
  bool verifyState(std::string& errorString) const;

  /// Reads OpenGL state from OpenGL and updates all appropriate class
  /// variables. This modifies the entire state.
  void readStateFromOpenGL();

  /// Functions for getting/setting specific OpenGL states.
  /// The apply... family of functions all have the same parameters:
  /// (force, currentState).
  /// When calling apply, curState will be modified to the value of the
  /// current state.
  /// force     - If true, then the current state is applied regardless
  ///             of the relative GLState.
  /// curState  - Current OpenGL state. This will be modified if it differs
  ///             from the set GL state.
  /// @{

  /// Enable depth test.
  /// OpenGL: glEnable(GL_DEPTH_TEST) or glDisable(GL_DEPTH_TEST)
  void    setDepthTestEnable(bool value)  {mDepthTestEnable = value;}
  bool    getDepthTestEnable() const      {return mDepthTestEnable;}
  void    applyDepthTestEnable(bool force, GLState* curState = nullptr);

  /// Enable depth function.
  /// OpenGL: glDepthFunc(value)
  /// Example values: GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL, GL_GREATER,
  ///                 GL_NOTEQUAL, GL_GEQUAL, GL_ALWAYS.
  void    setDepthFunc(GLenum value)      {mDepthFunc = value;}
  GLenum  getDepthFunc() const            {return mDepthFunc;}
  void    applyDepthFunc(bool force, GLState* curState = nullptr);

  /// Set culling state.
  /// OpenGL: glCullFace(value)
  /// Example values: GL_FRONT, GL_BACK.
  void    setCullFace(GLenum value) {mCullFace = value;}
  GLenum  getCullFace()             {return mCullFace;}
  void    applyCullFace(bool force, GLState* curState = nullptr);

  /// Enable face culling.
  /// OpenGL: glEnable(GL_CULL_FACE) or glDisable(GL_CULL_FACE)
  void    setCullFaceEnable(bool value) {mCullFaceEnable = value;}
  bool    getCullFaceEnable()           {return mCullFaceEnable;}
  void    applyCullFaceEnable(bool force, GLState* curState = nullptr);

  /// Set culling front face order.
  /// OpenGL: glFrontFace(value)
  /// Example values: GL_CCW, GL_CW.
  void    setFrontFace(GLenum value)  {mCullFrontFace = value}
  GLenum  getFrontFace()              {return mCullFrontFace;}
  void    applyFrontFace(bool force, GLState* curState = nullptr);
  
  /// Enable / disable blending.
  /// OpenGL: glEnable(GL_BLEND) or glDisable(GL_BLEND)
  void    setBlendEnable(bool value)   {mBlendEnable = value;}
  bool    getBlendEnable()             {return mBlendEnable;}
  void    applyBlendEnable(bool force, GLState* curState = nullptr);

  void setBlendEquation(BLEND_EQ value, bool apply = false, bool force = false);
  void setBlendFunction(BLEND_FUNC src, BLEND_FUNC dest, bool apply = false, bool force = false);
  void setDepthMask(bool value,         bool apply = false, bool force = false);
  void setColorMask(bool mask,          bool apply = false, bool force = false);
  void setLineWidth(float width,        bool apply = false, bool force = false);
  void setLineSmoothingEnable(bool value, bool apply = false, bool force = false);

  /// @}

  /// This reads state from OpenGL, only call when a context is active.
  size_t getMaxTextureUnits() const;

  bool        mDepthTestEnable;
  GLenum      mDepthFunc;

  GLenum      mCullFace;
  bool        mCullFaceEnable;
  GLenum      mCullFrontFace;

  bool        mBlendEnable;
  BLEND_EQ    mBlendEquation;
  BLEND_FUNC  mBlendFuncSrc;
  BLEND_FUNC  mBlendFuncDst;

  bool        mDepthMask;
  bool        mColorMask;

  float       mLineWidth;       ///< glLineWidth(...)
  bool        mLineSmoothing;   ///< GL_LINE_SMOOTH - Anti-aliasing for lines

  size_t      mTexActiveUnit;

  struct TextureState
  {
    TextureState()
    {
      tex1D = false;
      tex2D = false;
      tex3D = false;
      cubeMap = false;
    }
    bool tex1D;   // glEnable(GL_TEXTURE_1D)
    bool tex2D;   // glEnable(GL_TEXTURE_2D)
    bool tex3D;   // glEnable(GL_TEXTURE_3D)
    bool cubeMap; // glEnable(GL_TEXTURE_CUBE_MAP)
  };

  TextureState   mTextureState[CPM_GL_STATE_MAX_TEXTURE_UNITS];
};

} // namespace CPM_GL_STATE_NS 

#endif 


#ifndef IAUNS_GL_STATE_H
#define IAUNS_GL_STATE_H

/// \todo Handle state relative to some default state?

#include <cstddef>
#include <gl-platform/GLPlatform.hpp>

namespace CPM_GL_STATE_NS {

#ifndef CPM_GL_STATE_MAX_TEXTURE_UNITS
#define CPM_GL_STATE_MAX_TEXTURE_UNITS 12
#endif

class GLState

public:

  GLState() :
      mDepthTestEnable(true),
      mDepthFunc(DF_LESS),
      mCullFaceEnable(false),
      mCullFace(GL_BACK),
      mCullFrontFace(GL_CCW),
      mBlendEnable(true),
      mBlendEquation(GL_FUNC_ADD),
      mBlendFuncSrc(GL_SRC_ALPHA),
      mBlendFuncDst(GL_ONE_MINUS_SRC_ALPHA),
      mDepthMask(GL_TRUE),
      mColorMaskRed(GL_TRUE),
      mColorMaskGreen(GL_TRUE),
      mColorMaskBlue(GL_TRUE),
      mColorMaskAlpha(GL_TRUE),
      mLineWidth(2.0f),
      mLineSmoothing(false),
      mTexActiveUnit(GL_TEXTURE0)
  {}

  // Use default copy constructor, we are safe directly copying array.

  ~GLState() {}

  /// String representation of entire GLState. String includes '\n' chars.
  std::string getStateDescription();

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
  /// This call will internally change the active texture unit with
  /// glActiveTexture, but will switch back to the last active state
  /// before returning.
  void readStateFromOpenGL();

  /// This reads state from OpenGL, only call when a context is active.
  size_t getMaxTextureUnits() const;

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

  /// Set the blending equation
  /// OpenGL: glBlendEquation(value)
  /// Example values: GL_FUNC_ADD, GL_FUNC_SUBTRACT, GL_FUNC_REVERSE_SUBTRACT,
  ///                 GL_MIN (no ES 2.0), GL_MAX (no ES 2.0).
  void    setBlendEquation(GLenum value)  {mBlendEquation = value;}
  GLenum  getBlendEquation()              {return mBlendEquation;}
  void    applyBlendEquation(bool force, GLState* curState = nullptr);

  /// Set blending function
  /// OpenGL: glBlendFunc(src, dst)
  /// Example values: GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR,
  ///                 GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR, GL_SRC_ALPHA,
  ///                 GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA,
  ///                 GL_SRC_ALPHA_SATURATE.
  void    setBlendFunction(GLenum src, GLenum dest) {mBlendFuncSrc = src; mBlendFuncDst = dest;}
  std::pair<GLenum, GLenum> getBlendFunction()      {return std::make_pair(mBlendFuncSrc, mBlendFuncDst);}
  void    applyBlendFunction(bool force, GLState* curState = nullptr);

  /// Set depth mask
  /// OpenGL: glDepthMask(value)
  void    setDepthMask(GLboolean value)   {mDepthMask = value;}
  GLboolean getDepthMask()                {return mDepthMask;}
  void    applyDepthMask(bool force, GLState* curState = nullptr);

  /// Set color mask
  /// OpenGL: glColorMask(red, green, blue, alpha)
  void    setColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
  std::tuple<GLboolean, GLboolean, GLboolean, GLboolean> getColorMask();
  void    applyColorMask(bool force, GLState* curState = nullptr);

  /// Set line width.
  /// OpenGL: glLineWidth()
  void    setLineWidth(float width) {mLineWidth = width;}
  float   getLineWidth()            {return mLineWidth;}
  void    applyLineWidth(bool force, GLState* curState = nullptr);

  /// Set line smoothing.
  /// OpenGL: glEnable(GL_LINE_SMOOTH)
  /// NOTE: Not supported in OpenGL ES.
  void    setLineSmoothingEnable(bool value)  {mLineSmoothing = value;}
  bool    getLineSmoothingEnable()            {return mLineSmoothing;}
  void    applyLineWidth(bool force, GLState* curState = nullptr);

  /// Enables 1D textures
  /// OpenGL: glEnable(GL_TEXTURE_1D)
  /// NOTE: Not supported in OpenGL ES.
  void      setTexture1D(size_t index, GLboolean value);
  GLboolean getTexture1D(size_t index);
  void      applyTexture1D(size_t index, bool force, GLState* curState = nullptr);

  /// Set active texture unit.
  /// OpenGL: glActiveTexture(value)
  void    setActiveTexture(GLenum value)      {mTexActiveUnit = value;}
  GLenum  getActiveTexture()                  {return mTexActiveUnit;}
  void    applyActiveTexture(bool force, GLState* curState = nullptr);
  /// @}


  bool        mDepthTestEnable;
  GLenum      mDepthFunc;

  GLenum      mCullFace;
  bool        mCullFaceEnable;
  GLenum      mCullFrontFace;

  bool        mBlendEnable;
  GLenum      mBlendEquation;
  GLenum      mBlendFuncSrc;
  GLenum      mBlendFuncDst;

  GLboolean   mDepthMask;

  GLboolean   mColorMaskRed;
  GLboolean   mColorMaskGreen;
  GLboolean   mColorMaskBlue;
  GLboolean   mColorMaskAlpha;

  float       mLineWidth;       ///< glLineWidth(...)
  bool        mLineSmoothing;   ///< GL_LINE_SMOOTH - Anti-aliasing for lines

  GLenum      mTexActiveUnit;   ///< Active texture unit.

  struct TextureState
  {
    TextureState()
    {
      tex1D   = GL_FALSE;
      tex2D   = GL_FALSE;
      tex3D   = GL_FALSE;
      cubeMap = GL_FALSE;
    }
    GLboolean tex1D;   // glEnable(GL_TEXTURE_1D)
    GLboolean tex2D;   // glEnable(GL_TEXTURE_2D)
    GLboolean tex3D;   // glEnable(GL_TEXTURE_3D)
    GLboolean cubeMap; // glEnable(GL_TEXTURE_CUBE_MAP)
  };

  TextureState   mTextureState[CPM_GL_STATE_MAX_TEXTURE_UNITS];
};

} // namespace CPM_GL_STATE_NS 

#endif 


#include "GLState.hpp"

namespace CPM_GL_STATE_NS {

// All of these functions can be made into a 2D array (mimicking a 1-1 function).

//------------------------------------------------------------------------------
GLState::GLState()
{
}

//------------------------------------------------------------------------------
GLState::~GLState()
{
}

//------------------------------------------------------------------------------
size_t GLState::getMaxTextureUnits() const
{
  GLint tmp;
#ifdef CPM_GL_STATE_ES_2
  glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &tmp);
#else
  glGetIntegerv(GL_MAX_TEXTURE_UNITS, &tmp);
#endif
  return static_cast<size_t>(tmp);
}


//------------------------------------------------------------------------------
void GLState::apply() const
{
  // Directly apply entire OpenGL state.
  apply(this);
}

//------------------------------------------------------------------------------
void GLState::applyRelative(const GLState& state) const
{
  
}

//------------------------------------------------------------------------------
void GLState::apply(const GPUState& state, bool force)
{
  GL_CHECK();

  setDepthTestEnable(state.mDepthTestEnable, force);
  setDepthFunc(state.mDepthFunc, force);
  setCullFaceEnable(state.mCullFaceEnable, force);
  setCullState(state.mCullState, force);
  setBlendEnable(state.mBlendEnable, force);
  setCullFrontFaceOrder(state.mCullOrder, force);
  setLineWidth(state.mLineWidth, force);
  setLineSmoothingEnable(state.mLineSmoothing, force);

  /*
  // Do this by hand to avoid the redundant glActiveTexture calls
  /// \todo Grab the maximum number of texture units and use that instead...
  for (size_t i = 0; i < getMaxTextureUnits();  i++) 
  {
    if (force || state.mTexEnable[i] != mInternalState.mTexEnable[i]) 
    {
      glActiveTexture(GLenum(GL_TEXTURE0+i));
      mInternalState.mTexEnable[i] = state.mTexEnable[i];
      switch (mInternalState.mTexEnable[i])
      {
#ifdef CPM_GL_STATE_ES_2
        case TEX_1D:      Log::error() << "1D textures not supported in ES 2.0" << std::endl;
                          glDisable(GL_TEXTURE_2D);
                          glDisable(GL_TEXTURE_CUBE_MAP);
                          break;
        case TEX_2D:      glDisable(GL_TEXTURE_CUBE_MAP);
                          glEnable(GL_TEXTURE_2D);
                          break;
        case TEX_3D:      Log::error() << "3D textures not supported in ES 2.0" << std::endl;
                          glDisable(GL_TEXTURE_CUBE_MAP);
                          glDisable(GL_TEXTURE_2D);
                          break;
        case TEX_NONE:    glDisable(GL_TEXTURE_2D);
                          glDisable(GL_TEXTURE_CUBE_MAP);
                          break;
#else
        case TEX_1D:      glDisable(GL_TEXTURE_2D);
                          glDisable(GL_TEXTURE_3D);
                          glDisable(GL_TEXTURE_CUBE_MAP);
                          glEnable(GL_TEXTURE_1D);
                          break;
        case TEX_2D:      glDisable(GL_TEXTURE_3D);
                          glDisable(GL_TEXTURE_CUBE_MAP);
                          glEnable(GL_TEXTURE_2D);
                          break;
        case TEX_3D:      glDisable(GL_TEXTURE_CUBE_MAP);
                          glDisable(GL_TEXTURE_1D);
                          glDisable(GL_TEXTURE_2D);
                          glEnable(GL_TEXTURE_3D);
                          break;
        case TEX_NONE:    glDisable(GL_TEXTURE_1D);
                          glDisable(GL_TEXTURE_2D);
                          glDisable(GL_TEXTURE_3D);
                          glDisable(GL_TEXTURE_CUBE_MAP);
                          break;
#endif
      }
    }
  }
  mInternalState.mTexActiveUnit = state.mTexActiveUnit;
  glActiveTexture(GLenum(GL_TEXTURE0 + mInternalState.mTexActiveUnit));
  */

  setDepthMask(state.mDepthMask, force);
  setColorMask(state.mColorMask, force);
  setBlendEquation(state.mBlendEquation, force);
  setBlendFunction(state.mBlendFuncSrc, state.mBlendFuncDst, force);

  GL_CHECK();
}

//------------------------------------------------------------------------------
GPUState GLState::getStateFromOpenGL() const
{
  GL_CHECK();

  GPUState state;

  GLint e;
  glGetIntegerv(GL_DEPTH_FUNC, &e);
  state.mDepthTestEnable              = glIsEnabled(GL_DEPTH_TEST) != 0;
  state.mDepthFunc                    = GLToDEPTH_FUNC(static_cast<GLenum>(e));

  state.mCullFaceEnable               = glIsEnabled(GL_CULL_FACE) != 0;

  glGetIntegerv(GL_CULL_FACE_MODE, &e);
  state.mCullState                    = (e == GL_FRONT) ? CULL_FRONT : CULL_BACK;

  state.mBlendEnable                  = glIsEnabled(GL_BLEND) != 0;

  glGetIntegerv(GL_FRONT_FACE, &e);
  state.mCullOrder                    = GLToCULL_ORDER(static_cast<GLenum>(e));

  for(size_t i=0; i < getMaxTextureUnits(); ++i)
  {
    glActiveTexture(GL_TEXTURE0 + GLenum(i));
#ifdef CPM_GL_STATE_ES_2
    if (glIsEnabled(GL_TEXTURE_2D))
    {
      state.mTexEnable[i] = TEX_2D;
    }
    else
    {
      state.mTexEnable[i] = TEX_NONE;
    }
#else
    if (glIsEnabled(GL_TEXTURE_3D))
    {
      state.mTexEnable[i] = TEX_3D;
    }
    else if (glIsEnabled(GL_TEXTURE_2D))
    {
      state.mTexEnable[i] = TEX_2D;
    }
    else if (glIsEnabled(GL_TEXTURE_1D))
    {
      state.mTexEnable[i] = TEX_1D;
    }
    else 
    {
      state.mTexEnable[i] = TEX_NONE;
    }
#endif
  }
  GLboolean	 b;
  glGetBooleanv(GL_DEPTH_WRITEMASK, &b);
  state.mDepthMask = b != 0;

  GLboolean	 col[4];
  glGetBooleanv(GL_COLOR_WRITEMASK, col);
  state.mColorMask = col[0] != 0;  

  GLint src, dest;
#ifdef CPM_GL_STATE_ES_2
  //GL_BLEND_DST_RGB and GL_BLEND_DST_ALPHA
  glGetIntegerv(GL_BLEND_SRC_RGB, &src);
  glGetIntegerv(GL_BLEND_DST_RGB, &dest);
#else
  glGetIntegerv(GL_BLEND_SRC, &src);
  glGetIntegerv(GL_BLEND_DST, &dest);
#endif
  state.mBlendFuncSrc = GLToBLEND_FUNC(static_cast<GLenum>(src));
  state.mBlendFuncDst = GLToBLEND_FUNC(static_cast<GLenum>(dest));

  GLint equation;
  GL(glGetIntegerv(GL_BLEND_EQUATION_RGB, &equation));
  state.mBlendEquation = GLToBLEND_EQ(static_cast<GLenum>(equation));

  return state;
}


//------------------------------------------------------------------------------
void GLState::applyDepthTestEnable(bool force, GLState* cur)
{
  if (force || (cur && cur->mDepthTestEnable != mDepthTestEnable))
  {
    if (cur) cur->mDepthTestEnable = mDepthTestEnable;
    if (mDepthTestEnable)
      GL(glEnable(GL_DEPTH_TEST));
    else
      GL(glDisable(GL_DEPTH_TEST));
  }
}

//------------------------------------------------------------------------------
void GLState::applyDepthFunc(bool force, GLState* cur)
{
  if (force || (cur && cur->mDepthFunc != mDepthFunc))
  {
    if (cur) cur->mDepthFunc = mDepthFunc;
    GL(glDepthFunc(mDepthFunc));
  }
}

//------------------------------------------------------------------------------
void GLState::applyCullFace(bool force, GLState* cur)
{
  if (force || (cur && cur->mCullState != mCullState))
  {
    if (cur) cur->mCullState = mCullState;
    GL(glCullFace(mCullState));
  }
}

//------------------------------------------------------------------------------
void GLState::applyCullFaceEnable(bool force, GLState* cur)
{
  if (force || (cur && cur->mCullFaceEnable != mCullFaceEnable))
  {
    if (cur) cur->mCullFaceEnable = mCullFaceEnable;
    if (mCullFaceEnable)
      GL(glEnable(GL_CULL_FACE));
    else
      GL(glDisable(GL_CULL_FACE));
  }
}

//------------------------------------------------------------------------------
void GLState::applyFrontFace(bool force, GLState* cur)
{
  if (force || (cur && cur->mCullFrontFace != mCullFrontFace))
  {
    if (cur) cur->mCullFrontFace = mCullFrontFace;
    GL(glFrontFace(mCullFrontFace));
  }
}

//------------------------------------------------------------------------------
void GLState::applyBlendEnable(bool force, GLState* cur)
{
  if (force || (cur && cur->mBlendEnable != mBlendEnable))
  {
    if (cur) cur->mBlendEnable = mBlendEnable;
    if (mBlendEnable)
      GL(glEnable(GL_BLEND));
    else
      GL(glDisable(GL_BLEND));
  }
}

//------------------------------------------------------------------------------
void GLState::applyBlendEquation(bool force, GLState* cur)
{
  if (force || (cur && cur->mBlendEquation != mBlendEquation))
  {
    if (cur) cur->mBlendEquation = mBlendEquation;
    GL(glBlendEquation(mBlendEquation));
  }
}

//------------------------------------------------------------------------------
void GLState::applyBlendFunction(bool force, GLState* cur)
{
  if (   force
      || (cur && cur->mBlendFuncSrc != mBlendFuncSrc)
      || (cur && cur->mBlendFuncDst != mBlendFuncDst))
  {
    if (cur)
    {
      cur->mBlendFuncSrc = mBlendFuncSrc;
      cur->mBlendFuncDst = mBlendFuncDst;
    }
    GL(glBlendFunc(mBlendFuncSrc, mBlendFuncDst));
  }
}

//------------------------------------------------------------------------------
void GLState::applyDepthMask(bool force, GLState* cur)
{
  if (force || (cur && cur->mDepthMask != mDepthMask))
  {
    if (cur) cur->mDepthMask = mDepthMask;
    GL(glDepthMask(mInternalState.mDepthMask ? 1 : 0));
  }
}

//------------------------------------------------------------------------------
void GPUStateManager::applyLineWidth(bool force, GLState* curState)
{
  if (force || (cur && cur->mLineWidth != mLineWidth))
  {
    if (cur) cur->mLineWidth = mLineWidth;
    GL(glLineWidth(mLineWidth));
  }
}


//------------------------------------------------------------------------------
void GLState::applyColorMask(bool force, GLState* cur)
{
  if (   force 
      || (cur && cur->mColorMaskRed != mColorMaskRed)
      || (cur && cur->mColorMaskGreen != mColorMaskGreen)
      || (cur && cur->mColorMaskBlue != mColorMaskBlue)
      || (cur && cur->mColorMaskAlpha != mColorMaskAlpha))
  {
    if (cur)
    {
      cur->mColorMaskRed   = mColorMaskRed;
      cur->mColorMaskGreen = mColorMaskGreen;
      cur->mColorMaskBlue  = mColorMaskBlue;
      cur->mColorMaskAlpha = mColorMaskAlpha;
    }
    GL(glColorMask(mColorMaskRed, mColorMaskGreen, mColorMaskBlue, mColorMaskAlpha));
  }
}

//------------------------------------------------------------------------------
void GLState::setColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
  mColorMaskRed   = red;
  mColorMaskGreen = green;
  mColorMaskBlue  = blue;
  mColorMaskAlpha = alpha;
}

//------------------------------------------------------------------------------
std::tuple<GLboolean, GLboolean, GLboolean, GLboolean> GLState::getColorMask()
{
  return std::make_tuple(mColorMaskRed, mColorMaskGreen, mColorMaskBlue, mColorMaskAlpha);
}

//------------------------------------------------------------------------------
void GLState::applyLineWidth(bool force, GLState* curState)
{
  if (force || (cur && cur->mLineSmoothing != mLineSmoothing))
  {
    if (cur) cur->mLineSmoothing = mLineSmoothing;

    // Line smoothing not supported in OpenGL ES.
#ifndef CPM_GL_STATE_ES_2
    if (mLineSmoothing)
    {
      GL(glEnable(GL_LINE_SMOOTH));
    }
    else
    {
      GL(glDisable(GL_LINE_SMOOTH));
    }
#endif
  }
}

} // namespace CPM_GL_STATE_NS 


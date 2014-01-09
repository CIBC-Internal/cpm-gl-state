/// \author James Hughes
/// \date   November 2013

#include <batch-testing/GlobalGTestEnv.hpp>
#include <batch-testing/SpireTestFixture.hpp>

#include <gl-state/GLState.hpp>

using namespace CPM_BATCH_TESTING_NS;

TEST_F(TestStateSet, TestQuadRotation)
{
  // Current state.
  GLState defaultState;
  defaultState.readStateFromOpenGL();

  auto testStateAgainstOpenGL = [](GLState& state)
  {
    GLState curState;
    curState.readStateFromOpenGL();
    if (curState == state)
      return true;
    else
      return false;
  };

  // Returns true if the state in OpenGL is the same as the state stored
  // in the GLState structure.
  auto applyAndTestStateAgainstOpenGL = [&](GLState& state)
  {
    // Reset to default state.
    defaultState.apply();
    GLState currentState = defaultState;

    // Attempt relative application.
    state.applyRelative(currentState);
    testStateAgainstOpenGL(state);

    // Reset to default state.
    defaultState.apply();
    currentState = defaultState;

    // Attempt direct application
    state.apply();
    testStateAgainstOpenGL(state);
  };

  // Test state changes by modifying the default state, the reading the
  // OpenGL state to see if the necessary change was made.

  GLState testState;

  // Depth test enable
  testState = defaultState;
  testState.setDepthTestEnable(true);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setDepthTestEnable(false);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(TestState));

  // Depth func
  testState = defaultState;
  testState.setDepthFunc(GL_NEVER);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setDepthTestEnable(GL_LESS);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(TestState));
  testState.setDepthTestEnable(GL_EQUAL);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(TestState));
  testState.setDepthTestEnable(GL_LEQUAL);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(TestState));
  testState.setDepthTestEnable(GL_GREATER);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(TestState));
  testState.setDepthTestEnable(GL_NOTEQUAL);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(TestState));
  testState.setDepthTestEnable(GL_GEQUAL);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(TestState));
  testState.setDepthTestEnable(GL_ALWAYS);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(TestState));

  // Cull face
  testState = defaultState;
  testState.setCullFace(GL_FRONT);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setCullFace(GL_BACK);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));

  // Cull face enable
  testState = defaultState;
  testState.setCullFaceEnable(true);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setCullFaceEnable(false);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));

  // Set front face
  testState = defaultState;
  testState.setFrontFace(GL_CW);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setFrontFace(GL_CCW);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));

  // Set blend enable
  testState = defaultState;
  testState.setBlendEnable(true);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setBlendEnable(false);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));

  // Set blend equation
  testState = defaultState;
  testState.setBlendEquation(GL_FUNC_ADD);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setBlendEquation(GL_FUNC_SUBTRACT);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setBlendEquation(GL_MIN);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setBlendEquation(GL_MAX);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));

  // Set blending function (only testing subset)
  testState = defaultState;
  testState.setBlendEquation(GL_ZERO, GL_ZERO);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setBlendEquation(GL_ONE, GL_ZERO);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setBlendEquation(GL_ZERO, GL_ONE);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setBlendEquation(GL_ONE, GL_ONE);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setBlendEquation(GL_SRC_COLOR, GL_SRC_ALPHA);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setBlendEquation(GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));

  // Set depth mask
  testState = defaultState;
  testState.setDepthMask(GL_TRUE);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setDepthMask(GL_FALSE);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));

  // Set color mask (only testing subset instead of all 2^4)
  testState = defaultState;
  testState.setColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_FALSE);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setColorMask(GL_TRUE, GL_TRUE, GL_FALSE, GL_FALSE);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_FALSE);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setColorMask(GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setColorMask(GL_FALSE, GL_FALSE, GL_TRUE, GL_TRUE);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));

  // Set line width
  testState = defaultState;
  testState.setLineWidth(2.5f);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setLineWidth(1.5f);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setLineWidth(1.0f);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));

  // Set line smoothing
  testState = defaultState;
  testState.setLineSmoothingEnable(true);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setLineSmoothingEnable(false);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));

  // Set active texture unit (only tests till 3)
  testState = defaultState;
  testState.setActiveTexture(GL_TEXTURE0);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setActiveTexture(GL_TEXTURE1);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setActiveTexture(GL_TEXTURE2);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));
  testState.setActiveTexture(GL_TEXTURE3);
  EXPECT_EQ(true, applyAndTestStateAgainstOpenGL(testState));

  // We should also be able to test that *only* a particular state was
  // changed when we perform applyRelative .
}


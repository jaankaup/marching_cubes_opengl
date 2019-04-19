#include "camera.h"

// http://in2gpu.com/2016/03/14/opengl-fps-camera-quaternion/

Camera::Camera()
{
  pEyePosition = glm::vec3(0.0f,3.0f,0.0f);
//  pEyePosition = glm::vec3(0.0f,1.5f,1.5f);
//    pEyePosition = glm::vec3(00.0f,50.0f,0.0f);
//    glm::vec3 pUp = glm::vec3(0.0f,1.0f, 0.0f);
//    glm::vec3 pLookat = glm::vec3(0.0f,0.0f,0.0f);
    pPitch = 0.0f;
    pYaw = 0.0f;
//    pPitch = -40.0f;
//    pYaw = -90.0f;
    pRoll = 0.0f;
    pCamera_quat = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
    pMousePosition =  glm::vec2(0.0f,0.0f); 
    pPrevTick = SDL_GetTicks();
    float pMouseX_sensitivity = 350.2f;
    float pMouseY_sensitivity = 350.2f;
}

Camera::~Camera()
{

}

//glm::mat4 createViewMatrix() const
//{
//    return glm::lookAt(pEyePositions, pLookat, pUp);
//}

void Camera::update()
{
  //temporary frame quaternion from pitch,yaw,roll
  //here roll is not used
  glm::quat key_quat = glm::quat(glm::vec3(pPitch, pYaw, 0.0f /* pRoll */));

  pPitch = 0.0f;
  pYaw = 0.0f;
  pRoll = 0.0f;

  //order matters,update camera_quat
  pCamera_quat = key_quat * pCamera_quat;
  pCamera_quat.z = 0.0f;   
  pCamera_quat = glm::normalize(pCamera_quat);
  glm::mat4 rotate = glm::mat4_cast(pCamera_quat);

  glm::mat4 translate = glm::translate(glm::mat4(1.0f), -pEyePosition);
//  Log::getDebug().log("pEyePosition = (%,%,%)",std::to_string(pEyePosition.x),std::to_string(pEyePosition.y),std::to_string(pEyePosition.z));

  glm::mat4 viewMatrix = rotate * translate;
//  pLookat = glm::vec3(viewMatrix[0][2],viewMatrix[1][2],viewMatrix[2][2]);
//  pEyePosition = glm::vec3(viewMatrix[3][0],viewMatrix[3][1],viewMatrix[3][2]);
//  pEyePosition = glm::vec3(viewMatrix[0][3],viewMatrix[1][3],viewMatrix[2][3]);
  //pLookat = glm::vec3(viewMatrix[2][0],viewMatrix[2][1],viewMatrix[2][2]);
//  pEyePosition = glm::vec3(viewMatrix[3][0],viewMatrix[3][1],viewMatrix[3][2]);
//  Log::getDebug().log("pLookat = (%,%,%)",std::to_string(pLookat.x),std::to_string(pLookat.y),std::to_string(pLookat.z));
//  Log::getDebug().log("pEyePosition = (%,%,%)",std::to_string(pEyePosition.x),std::to_string(pEyePosition.y),std::to_string(pEyePosition.z));
//  Log::getDebug().log("viewMatrix = (%,%,%,%)",std::to_string(viewMatrix[0][0]),std::to_string(viewMatrix[0][1]),std::to_string(viewMatrix[0][2]),std::to_string(viewMatrix[0][3]));
//  Log::getDebug().log("viewMatrix = (%,%,%,%)",std::to_string(viewMatrix[1][0]),std::to_string(viewMatrix[1][1]),std::to_string(viewMatrix[1][2]),std::to_string(viewMatrix[1][3]));
//  Log::getDebug().log("viewMatrix = (%,%,%,%)",std::to_string(viewMatrix[2][0]),std::to_string(viewMatrix[2][1]),std::to_string(viewMatrix[2][2]),std::to_string(viewMatrix[2][3]));
//  Log::getDebug().log("viewMatrix = (%,%,%,%)",std::to_string(viewMatrix[3][0]),std::to_string(viewMatrix[3][1]),std::to_string(viewMatrix[3][2]),std::to_string(viewMatrix[3][3]));
//  Log::getDebug().log("pEyePosition = (%,%,%,%)",std::to_string(viewMatrix[3][0]),std::to_string(viewMatrix[3][1]),std::to_string(viewMatrix[3][2]),std::to_string(viewMatrix[3][3]));
  //auto temp = glm::vec3(viewMatrix[8],viewMatrix[9],viewMatrix[10]);
//  pViewMatrix = viewMatrix;
  pViewMatrix = viewMatrix;
//  return viewMatrix;
}

void Camera::rotateCamera(float x, float y, int width, int height, bool isMousePressed)
{
  if (isMousePressed == false)
      return;
  //always compute delta
  //mousePosition is the last mouse position
  glm::vec2 mouse_delta = glm::vec2(x, y) - pMousePosition;
  pMousePosition = glm::vec2(x,y);

  //notice that we reduce the sensitvity
    
  pYaw   = pMouseX_sensitivity * mouse_delta.x;
  pPitch = pMouseY_sensitivity * mouse_delta.y;

  pMousePosition = glm::vec2(x, y);
  //update();
}


void Camera::handleEvents()
{
  uint32_t newTick = SDL_GetTicks();
//  uint32_t deltaTick = newTick - pPrevTick;
  float deltaTime = pPrevTick == 0 ? 0.0f : (newTick*1.0f) / pPrevTick;
  pPrevTick = newTick;

    
//  pViewMatrix[3][0] = position.x; 
//  pViewMatrix[3][1] = position.y; 
//  pViewMatrix[3][2] = position.z; 

  SDL_Event e;
  while (SDL_PollEvent(&e))
  {
    switch (e.type)
    {
      case SDL_QUIT:
        ProgramState::getInstance().setAppRunning(false);
        break;

        /* Kameralle */
      case SDL_MOUSEMOTION:
      case SDL_MOUSEBUTTONDOWN:
        switch (e.type)
        {
          case (SDL_MOUSEBUTTONDOWN):
            pMousePosition = glm::vec2(e.motion.x,e.motion.y);
//            Log::getDebug().log("SDL_MOUSEBUTTONDOWN: x,y = (%,%)",std::to_string(x),std::to_string(y));
//            break;
          case (SDL_MOUSEMOTION):
            switch (e.motion.state)
            {
              case (SDL_BUTTON_LMASK):
//                Log::getDebug().log("Rotating camera (%,%)",std::to_string(x),std::to_string(y));
                //rotateCamera(e.motion.x * deltaTime ,e.motion.y * deltaTime,0,0,true);
                rotateCamera(e.motion.x,e.motion.y,0,0,true);
                //pLastX = e.motion.x; 
                //pLastY = e.motion.y; 
                break;
            }
        }
        break;
    }
  }
//  float movementInterp = (SDL_GetTicks() * 0.0005f) * deltaTime;

  update();

  glm::vec3 lookAt = getLookAt();
//  glm::vec3 stafe = getStafe();
//  Log::getDebug().log("lookAt = (%,%,%)",std::to_string(lookAt.x),std::to_string(lookAt.y),std::to_string(lookAt.z));
//  Log::getDebug().log("**********************");
  auto keystate = SDL_GetKeyboardState(NULL); 

//  glm::vec3 lookat = glm::vec3(pViewMatrix[0][2],pViewMatrix[1][2],pViewMatrix[2][2]);
      /* Shift hidastaa liikkumisnopeutta */
    float speedMultiplier = pCamspeed;
    bool shift = false;
    if(keystate[SDL_SCANCODE_LSHIFT] || keystate[SDL_SCANCODE_RSHIFT] )
    {
        speedMultiplier *= 0.01f;
        shift = true;
    }

    /* WASD-nappaimet */
    if(keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W])
        pEyePosition -= lookAt * speedMultiplier * deltaTime;

    if(keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S])
        pEyePosition += lookAt * speedMultiplier * deltaTime;

    if(keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A])
        //pEyePosition += stafe * speedMultiplier * deltaTime;
        pEyePosition += glm::normalize(glm::cross(lookAt, glm::vec3(0.0f,1.0f,0.0f))) * speedMultiplier * deltaTime;

    if(keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D])
        //pEyePosition -= stafe * speedMultiplier * deltaTime;
        pEyePosition -= glm::normalize(glm::cross(lookAt, glm::vec3(0.0f,1.0f,0.0f))) * speedMultiplier * deltaTime;

    /* Ylos ja alas liikkuminen */
    if(keystate[SDL_SCANCODE_V])
        pEyePosition += glm::normalize(glm::vec3(0.0f,1.0f,0.0f)) * speedMultiplier * deltaTime;

    if(keystate[SDL_SCANCODE_C])
        pEyePosition -= glm::normalize(glm::vec3(0.0f,1.0f,0.0f)) * speedMultiplier * deltaTime;

    if(keystate[SDL_SCANCODE_Q])
        ProgramState::getInstance().setAppRunning(false);

    if(keystate[SDL_SCANCODE_KP_PLUS])
    {
        float vpb = ProgramState::getInstance().getVoxelsPerBlock() + 0.1f;
        ProgramState::getInstance().setVoxelsPerBlock(vpb);
        Log::getInfo().log("voxels_per_block = (%)",std::to_string(vpb));
    }

    if(keystate[SDL_SCANCODE_KP_MINUS])
    {
        float vpb = ProgramState::getInstance().getVoxelsPerBlock() - 0.1f;
        ProgramState::getInstance().setVoxelsPerBlock(vpb);
        Log::getInfo().log("voxels_per_block = (%)",std::to_string(vpb));
    }
    if(keystate[SDL_SCANCODE_SPACE])
    {
        bool wireframe = ProgramState::getInstance().getWireframe();
        ProgramState::getInstance().setWireframe(!wireframe);
        Log::getInfo().log("Wireframe = (%)",!wireframe ? "Enabled" : "Disabled");
    }
    if(keystate[SDL_SCANCODE_KP_MULTIPLY])
    {
        float cubeMask = ProgramState::getInstance().getCubeMask();
        if (cubeMask == 255.0f) return;
        ProgramState::getInstance().setCubeMask(cubeMask + 0.5f);
        Log::getInfo().log("CubeMask = (%)",std::to_string(ProgramState::getInstance().getCubeMaskCeil()));
    }
    if(keystate[SDL_SCANCODE_KP_DIVIDE])
    {
        float cubeMask = ProgramState::getInstance().getCubeMask();
        if (cubeMask == -1.0f) return;
        ProgramState::getInstance().setCubeMask(cubeMask - 0.5f);
        Log::getInfo().log("CubeMask = (%)",std::to_string(ProgramState::getInstance().getCubeMaskCeil()));
    }
    if(keystate[SDL_SCANCODE_RETURN])
    {
        bool debugMask = ProgramState::getInstance().getDebugCube();
        ProgramState::getInstance().setDebugCube(!debugMask);
        Log::getInfo().log("Debuggin cube = (%)", !debugMask ? "True" : "False" );
    }

    if(keystate[SDL_SCANCODE_I] && shift)
    {
        auto sp = ProgramState::getInstance().getStartPoint();
        sp = sp + glm::vec3(0.1f,0.0f,0.0f);
        ProgramState::getInstance().setStartPoint(sp);
        logGLM("startPoint",sp);
    }

    if(keystate[SDL_SCANCODE_I] && !shift)
    {
        auto sp = ProgramState::getInstance().getStartPoint();
        sp = sp + glm::vec3(-0.1f,0.0f,0.0f);
        ProgramState::getInstance().setStartPoint(sp);
        logGLM("startPoint",sp);
    }

    if(keystate[SDL_SCANCODE_J] && shift)
    {
        auto sp = ProgramState::getInstance().getStartPoint();
        sp = sp + glm::vec3(0.0f,0.1f,0.0f);
        ProgramState::getInstance().setStartPoint(sp);
        logGLM("startPoint",sp);
    }

    if(keystate[SDL_SCANCODE_J] && !shift)
    {
        auto sp = ProgramState::getInstance().getStartPoint();
        sp = sp + glm::vec3(0.0f,-0.1f,0.0f);
        ProgramState::getInstance().setStartPoint(sp);
        logGLM("startPoint",sp);
    }
    if(keystate[SDL_SCANCODE_K] && shift)
    {
        auto sp = ProgramState::getInstance().getStartPoint();
        sp = sp + glm::vec3(0.0f,0.0f,0.1f);
        ProgramState::getInstance().setStartPoint(sp);
        logGLM("startPoint",sp);
    }

    if(keystate[SDL_SCANCODE_K] && !shift)
    {
        auto sp = ProgramState::getInstance().getStartPoint();
        sp = sp + glm::vec3(0.0f,0.0f,-0.1f);
        ProgramState::getInstance().setStartPoint(sp);
        logGLM("startPoint",sp);
    }

    update();
//  auto result = update();
//  
//
//    return result;
}

glm::vec3 Camera::getLookAt() const
{
  return glm::vec3(pViewMatrix[0][2],pViewMatrix[1][2],pViewMatrix[2][2]);
}

glm::vec3 Camera::getStafe() const
{
  return glm::normalize(glm::vec3(pViewMatrix[0][0],pViewMatrix[0][1],pViewMatrix[0][2]));
}

//glm::vec3 Camera::getEyePosition() const
//{
//  return glm::vec3(pViewMatrix[3][0],pViewMatrix[3][1],pViewMatrix[3][2]);
//}

glm::vec3 Camera::getPosition() const
{
  return pEyePosition;
}

glm::mat4 Camera::getViewMatrix() const
{
  return pViewMatrix;
}

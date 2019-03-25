#include "camera.h"

// http://in2gpu.com/2016/03/14/opengl-fps-camera-quaternion/

Camera::Camera()
{
    pEyePosition = glm::vec3(0.0f,0.0f,0.0f);
//    glm::vec3 pUp = glm::vec3(0.0f,1.0f, 0.0f);
//    glm::vec3 pLookat = glm::vec3(0.0f,0.0f,0.0f);
    pPitch = 0.0f;
    pYaw = 0.0f;
//    pPitch = -40.0f;
//    pYaw = -90.0f;
    pRoll = 0.0f;
    pCamera_quat = glm::quat(glm::vec3(pPitch, pYaw, pRoll));
    pMousePosition =  glm::vec2(0.0f,0.0f); 
    pPrevTick = SDL_GetTicks();
    float pMouseX_sensitivity = 0.2f;
    float pMouseY_sensitivity = 0.2f;
}

Camera::~Camera()
{

}

//glm::mat4 createViewMatrix() const
//{
//    return glm::lookAt(pEyePositions, pLookat, pUp);
//}

glm::mat4 Camera::update()
{
  //temporary frame quaternion from pitch,yaw,roll
  //here roll is not used
  glm::quat key_quat = glm::quat(glm::vec3(pPitch, pYaw, pRoll));

  pPitch = 0.0f;
  pYaw = 0.0f;
  pRoll = 0.0f;

  //order matters,update camera_quat
  pCamera_quat = key_quat * pCamera_quat;
  pCamera_quat = glm::normalize(pCamera_quat);
  glm::mat4 rotate = glm::mat4_cast(pCamera_quat);

  glm::mat4 translate = glm::mat4(1.0f);
  translate = glm::translate(translate, -pEyePosition);

  glm::mat4 viewMatrix = rotate * translate;
  pLookat = glm::vec3(viewMatrix[0][2],viewMatrix[1][2],viewMatrix[2][2]);
//  Log::getDebug().log("pLookat = (%,%,%)",std::to_string(pLookat.x),std::to_string(pLookat.y),std::to_string(pLookat.z));
//  Log::getDebug().log("pEyePosition = (%,%,%)",std::to_string(pEyePosition.x),std::to_string(pEyePosition.y),std::to_string(pEyePosition.z));
  //auto temp = glm::vec3(viewMatrix[8],viewMatrix[9],viewMatrix[10]);
  return viewMatrix;
}

void Camera::rotateCamera(float x, float y, int width, int height, bool isMousePressed)
{
  if (isMousePressed == false)
      return;
  //always compute delta
  //mousePosition is the last mouse position
  glm::vec2 mouse_delta = glm::vec2(x, y) - pMousePosition;

  //notice that we reduce the sensitvity
//  const float mouseX_Sensitivity = 0.0020f;
//  const float mouseY_Sensitivity = 0.0020f;

  pYaw   = pMouseX_sensitivity * mouse_delta.x;
  pPitch = pMouseY_sensitivity * mouse_delta.y;

  pMousePosition = glm::vec2(x, y);
  //update();
}


glm::mat4 Camera::handleEvents()
{
  uint32_t newTick = SDL_GetTicks();
  uint32_t deltaTick = newTick - pPrevTick;
  float deltaTime = pPrevTick == 0 ? 0.0f : newTick / pPrevTick;
  pPrevTick = newTick;

  SDL_Event e;
  while (SDL_PollEvent(&e))
  {
    switch (e.type)
    {
      case SDL_QUIT:
        //            running_ = false;
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
                rotateCamera(e.motion.x * deltaTime ,e.motion.y * deltaTime,0,0,true);
                //pLastX = e.motion.x; 
                //pLastY = e.motion.y; 
                break;
            }
        }
        break;
    }
  }
//  float movementInterp = (SDL_GetTicks() * 0.0005f) * deltaTime;
  auto keystate = SDL_GetKeyboardState(NULL); 

      /* Shift hidastaa liikkumisnopeutta */
    float speedMultiplier = pCamspeed;
    if(keystate[SDL_SCANCODE_LSHIFT])
        speedMultiplier *= 0.1f;

    /* WASD-nappaimet */
    if(keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W])
        pEyePosition -= pLookat * speedMultiplier * deltaTime;

    if(keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S])
        pEyePosition += pLookat * speedMultiplier * deltaTime;

    if(keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A])
        pEyePosition += glm::normalize(glm::cross(pLookat, glm::vec3(0.0f,1.0f,0.0f))) * speedMultiplier * deltaTime;

    if(keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D])
        pEyePosition -= glm::normalize(glm::cross(pLookat, glm::vec3(0.0f,1.0f,0.0f))) * speedMultiplier * deltaTime;

    /* Ylos ja alas liikkuminen */
    if(keystate[SDL_SCANCODE_V])
        pEyePosition += glm::normalize(glm::vec3(0.0f,1.0f,0.0f)) * speedMultiplier * deltaTime;

    if(keystate[SDL_SCANCODE_C])
        pEyePosition -= glm::normalize(glm::vec3(0.0f,1.0f,0.0f)) * speedMultiplier * deltaTime;

    return update();
}

glm::vec3 Camera::getPosition() const
{
  return pEyePosition;
}

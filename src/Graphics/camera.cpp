#include "camera.h"

// Konstruktori
Camera::Camera(const glm::vec3& cameraPosition, const glm::vec3& cameraTarget, const glm::vec3& upVector)
{

    defaultPosition_ = cameraPosition;
    position_ = cameraPosition;
    target_ = cameraTarget;
    up_ = upVector;

    rotateCamera(0,0);
}

// Destruktori
Camera::~Camera()
{
}


/**
 * Palauttaa kameran matriisin
 * @return Matrix
 */
glm::mat4 Camera::getMatrix() const
{
    return view;
}

/**
 * Palauttaa kameran sijainnin
 * @return position
 */
glm::vec3 Camera::getPosition() const
{
    return position_;
}


/**
 * Asettaa kameran katsomaan annettua kohdetta
 * @param cameraTarget kameran kohdevektori
 */
void Camera::setView(const glm::vec3& cameraTarget)
{
    view = glm::lookAt(position_, cameraTarget, up_);
}


/**
 * Palauttaa kameran sijainnin alkuarvoihin
 */
void Camera::resetView()
{
    position_ = defaultPosition_;
    pitch     =  -45.0;
    yaw       =  -90.0;
    rotateCamera(0,0);
}


/**
 * Vaihtaa kameratyyppiä
 */
void Camera::toggleMode()
{
//    switch (camMode)
//    {
//        case (CameraMode::Free): camMode = CameraMode::Orbit; break;
//        case (CameraMode::Orbit): camMode = CameraMode::Floating; break;
//        case (CameraMode::Floating): camMode = CameraMode::Free; resetView(); break;
//    }
}


/**
 * Kameran sijainnin muutos
 * @param newPos uusi sijainti
 */
void Camera::translate(const glm::vec3& newPos)
{
    position_ = newPos;
}


/***
 * Kameran suunnan muutos hiirikoordinaattien perusteella.
 * @param xoffset x-muutos
 * @param xoffset y-muutos
 */
void Camera::rotateCamera(const float &xoffset, const float &yoffset)
{
    yaw   += xoffset;
    pitch += yoffset;
    pitch = glm::clamp(pitch, -89.0f, 89.0f);

    front_.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front_.y = sin(glm::radians(pitch));
    front_.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    front_ = glm::normalize(front_);
}


/**
 * Muuttaa kameran liikkumisnopeutta parametrin verran.
 * Minimi 1.0, maksimi 10.0
 * @param adjust muutos
 */
void Camera::adjustSpeed(const float &adjust)
{
    camSpeed += adjust;
    camSpeed = glm::clamp(camSpeed, 1.0f, 10.0f);
    //cout << "Camera speed: " << camSpeed << endl;
    //logInfo.log("Camera speed: %", camSpeed);
}


/**
 * Muuttaa kameran kääntymisnopeutta parametrin verran.
 * Minimi 0.1, maksimi 1.0
 * @param adjust muutos
 */
void Camera::adjustSensitivity(const float &adjust)
{
    camSensitivity += adjust;
    camSensitivity = glm::clamp(camSensitivity, 0.1f, 1.0f);
    //cout << "Camera sensitivity: "  << camSensitivity << endl;
    //logInfo.log("Camera sensitivity: %", camSensitivity);
}


/**
 * Kameran päivitys
 * @param time aikakerroin, (currentTick / lastTick), välitetään program-luokasta
 */
void Camera::update(const float time)
{
    //deltaTime = time;
    //movementInterp = (SDL_GetTicks() * 0.0005f) * deltaTime;

//    if (camMode == CameraMode::Free)
//    {
        // WASD-kamera
        translate(position_);
        setView(position_ + front_);
//    }
//    else if (camMode == CameraMode::Floating)
//    {
//        // Liikuttelee kameraa edestakaisin
//        float coordX = 80.0f + 80.0f * sin(movementInterp); // Vaihteluväli 0-160
//        float coordY = 80.0f + 40.0f * cos(movementInterp); // Vaihteluväli 40-120
//        float coordZ = 80.0f + 80.0f * sin(movementInterp) * cos(movementInterp); // Vaihteluväli 40-120
//        translate(glm::vec3(coordX, coordY, coordZ));
//        setView(target_);
//    }
//    else if (camMode == CameraMode::Orbit)
//    {
//        // Pyörittää kameraa kohteen ympäri
//        float coordX = 0.0f + 160.0f * sin(movementInterp);
//        float coordY = 120.0f; // Korkeus
//        float coordZ = 0.0f + 160.0f * cos(movementInterp);
//        translate(glm::vec3(coordX, coordY, coordZ));
//        setView(target_);
//    }

//    handleKeyInput();
}

/**
 * Näppäimistötapahtumien käsittely
 */
void Camera::handleKeyInput()
{
  uint32_t newTick = SDL_GetTicks();
  //deltaTime = pPrevTick == 0 ? 0.0f : (newTick*1.0f) / pPrevTick;
  auto del = newTick - pPrevTick;
  if (del != 0)
  {
    deltaTime = float(newTick)/float(pPrevTick);
  }
  pPrevTick = newTick;
    /* Pyydetään SDL:n näppäintila */
    const Uint8* keystate = SDL_GetKeyboardState(NULL);

    /* Shift hidastaa liikkumisnopeutta */
    float speedMultiplier = camSpeed;
    bool shift = false;
    if (keystate[SDL_SCANCODE_LSHIFT] || keystate[SDL_SCANCODE_RSHIFT] )
    {
        speedMultiplier *= 0.01f;
        shift = true;
    }

    /* WASD-näppäimet */
    if(keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W])
        position_ += front_ * speedMultiplier * deltaTime;

    if(keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S])
        position_ -= front_ * speedMultiplier * deltaTime;

    if(keystate[SDL_SCANCODE_LEFT] || keystate[SDL_SCANCODE_A])
        position_ -= glm::normalize(glm::cross(front_, up_)) * speedMultiplier * deltaTime;

    if(keystate[SDL_SCANCODE_RIGHT] || keystate[SDL_SCANCODE_D])
        position_ += glm::normalize(glm::cross(front_, up_)) * speedMultiplier * deltaTime;

    /* Ylös ja alas liikkuminen */
    if(keystate[SDL_SCANCODE_V])
        position_ += glm::normalize(up_) * speedMultiplier * deltaTime;

    if(keystate[SDL_SCANCODE_C])
        position_ -= glm::normalize(up_) * speedMultiplier * deltaTime;
    update(deltaTime);
//    update(deltaTime);
//    if(keystate[SDL_SCANCODE_KP_PLUS])
//    {
//        float vpb = ProgramState::getInstance().getVoxelsPerBlock() + 0.1f;
//        ProgramState::getInstance().setVoxelsPerBlock(vpb);
//        Log::getInfo().log("voxels_per_block = (%)",std::to_string(vpb));
//    }
//
//    if(keystate[SDL_SCANCODE_KP_MINUS])
//    {
//        float vpb = ProgramState::getInstance().getVoxelsPerBlock() - 0.1f;
//        ProgramState::getInstance().setVoxelsPerBlock(vpb);
//        Log::getInfo().log("voxels_per_block = (%)",std::to_string(vpb));
//    }
//    if(keystate[SDL_SCANCODE_SPACE])
//    {
//        bool wireframe = ProgramState::getInstance().getWireframe();
//        ProgramState::getInstance().setWireframe(!wireframe);
//        Log::getInfo().log("Wireframe = (%)",!wireframe ? "Enabled" : "Disabled");
//    }
//    if(keystate[SDL_SCANCODE_KP_MULTIPLY])
//    {
//        float cubeMask = ProgramState::getInstance().getCubeMask();
//        if (cubeMask == 255.0f) return;
//        ProgramState::getInstance().setCubeMask(cubeMask + 0.5f);
//        Log::getInfo().log("CubeMask = (%)",std::to_string(ProgramState::getInstance().getCubeMaskCeil()));
//    }
//    if(keystate[SDL_SCANCODE_KP_DIVIDE])
//    {
//        float cubeMask = ProgramState::getInstance().getCubeMask();
//        if (cubeMask == -1.0f) return;
//        ProgramState::getInstance().setCubeMask(cubeMask - 0.5f);
//        Log::getInfo().log("CubeMask = (%)",std::to_string(ProgramState::getInstance().getCubeMaskCeil()));
//    }
//    if(keystate[SDL_SCANCODE_RETURN])
//    {
//        bool debugMask = ProgramState::getInstance().getDebugCube();
//        ProgramState::getInstance().setDebugCube(!debugMask);
//        Log::getInfo().log("Debuggin cube = (%)", !debugMask ? "True" : "False" );
//    }
//
//    if(keystate[SDL_SCANCODE_I] && shift)
//    {
//        auto sp = ProgramState::getInstance().getStartPoint();
//        sp = sp + glm::vec3(0.1f,0.0f,0.0f);
//        ProgramState::getInstance().setStartPoint(sp);
//        logGLM("startPoint",sp);
//    }
//
//    if(keystate[SDL_SCANCODE_I] && !shift)
//    {
//        auto sp = ProgramState::getInstance().getStartPoint();
//        sp = sp + glm::vec3(-0.1f,0.0f,0.0f);
//        ProgramState::getInstance().setStartPoint(sp);
//        logGLM("startPoint",sp);
//    }
//
//    if(keystate[SDL_SCANCODE_J] && shift)
//    {
//        auto sp = ProgramState::getInstance().getStartPoint();
//        sp = sp + glm::vec3(0.0f,0.1f,0.0f);
//        ProgramState::getInstance().setStartPoint(sp);
//        logGLM("startPoint",sp);
//    }
//
//    if(keystate[SDL_SCANCODE_J] && !shift)
//    {
//        auto sp = ProgramState::getInstance().getStartPoint();
//        sp = sp + glm::vec3(0.0f,-0.1f,0.0f);
//        ProgramState::getInstance().setStartPoint(sp);
//        logGLM("startPoint",sp);
//    }
//    if(keystate[SDL_SCANCODE_K] && shift)
//    {
//        auto sp = ProgramState::getInstance().getStartPoint();
//        sp = sp + glm::vec3(0.0f,0.0f,0.1f);
//        ProgramState::getInstance().setStartPoint(sp);
//        logGLM("startPoint",sp);
//    }
//
//    if(keystate[SDL_SCANCODE_K] && !shift)
//    {
//        auto sp = ProgramState::getInstance().getStartPoint();
//        sp = sp + glm::vec3(0.0f,0.0f,-0.1f);
//        ProgramState::getInstance().setStartPoint(sp);
//        logGLM("startPoint",sp);
//    }
//    if(keystate[SDL_SCANCODE_T])
//    {
//        // This should go somewhere else.
//        auto metadata = ProgramState::getInstance().getMetadata();
//        auto name = metadata->texture3Dname;
//        TextureManager::getInstance().deleteTexture(name); 
//        Texture tex3D = TextureManager::getInstance().create3D(name);
//        auto tex3D_data = createPerlin3D(metadata->block_size*2,metadata->block_size*2,metadata->block_size*2);
//        tex3D.create3D(tex3D_data);
//        metadata->texture3Dname = name;
//        Log::getInfo().log("Creating a new 3D texture...");
//        
//        // Recreate scene model.
//        ModelManager::getInstance().createSceneObject();
//    }

//    if (keystate[SDL_SCANCODE_0]) { changeScene(0);}
//    if (keystate[SDL_SCANCODE_1]) { changeScene(1);}
//    if (keystate[SDL_SCANCODE_2]) { changeScene(2);}
//    if (keystate[SDL_SCANCODE_3]) { changeScene(3);}
//    if (keystate[SDL_SCANCODE_4]) { changeScene(4);}
//    if (keystate[SDL_SCANCODE_5]) { changeScene(5);}
//    if (keystate[SDL_SCANCODE_6]) { changeScene(6);}
//    if (keystate[SDL_SCANCODE_7]) { changeScene(7);}
//    if (keystate[SDL_SCANCODE_8]) { changeScene(8);}
//    if (keystate[SDL_SCANCODE_9]) { changeScene(9);}


}

void Camera::changeScene(const char number) const
{
        std::string shaderName = ProgramState::getInstance().getMetadata()->triangulationShader;
        std::string newName = shaderName.substr(0, shaderName.size()-1) + std::to_string(number);
        Log::getInfo().log("ShaderName == %", newName);
        ProgramState::getInstance().getMetadata()->triangulationShader = newName;
        Log::getInfo().log("Changing densityShader to % ...", newName);
        ModelManager::getInstance().createSceneObject();
}

/**
 * Hiiritapahtumien käsittely. Program välittää tapahtumat.
 */
void Camera::handleMouseInput(SDL_Event &inputEvent)
{

    switch (inputEvent.type)
    {
    case (SDL_MOUSEBUTTONDOWN): /* Klikkaus. Sijainti talteen sulavampaa liikettä varten */
        lastMouseX = inputEvent.motion.x;
        lastMouseY = inputEvent.motion.y;
    case (SDL_MOUSEMOTION):
        switch (inputEvent.motion.state)
        {
        case (SDL_BUTTON_LMASK): /* Hiiren vasen nappi pohjassa */
            rotateCamera((inputEvent.motion.x - lastMouseX) * camSensitivity * deltaTime,
                         (lastMouseY - inputEvent.motion.y) * camSensitivity * deltaTime);
            /* Hiiren sijainti talteen muutoksen laskemista varten */
            lastMouseX = inputEvent.motion.x;
            lastMouseY = inputEvent.motion.y;
            break;
        }
    }
}

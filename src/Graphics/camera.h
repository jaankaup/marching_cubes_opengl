#ifndef CAMERA_H
#define CAMERA_H
//#include <iostream>
//#include <vector>
//#include <array>
//#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "programstate.h"
#include "../Utils/log.h"

/** A simple shader class with basic shader operations. */
class Camera
{
	public:
		Camera();
		~Camera();

//    glm::mat4 createViewMatrix() const;

    // Returns a view-matrix.
    void update();
    void rotateCamera(float x, float y, int width, int height, bool isMouserPressed);
    void handleEvents();
    glm::vec3 getPosition() const;
    glm::vec3 getLookAt() const;
    glm::mat4 getViewMatrix() const;
    glm::vec3 getStafe() const;
//    glm::vec3 getEyePosition() const;

	private:
    glm::vec3 pEyePosition;
    glm::mat4 pViewMatrix;
//    glm::vec3 pUp;
    glm::vec3 pLookat;
    glm::quat pCamera_quat;
    float pTime = 0.0f;;
    float pPitch = -40.0f;
    float pYaw = -90.0f;
    float pRoll = 0.0f;
    glm::vec2 pMousePosition;
    float pMouseX_sensitivity = 0.002f;
    float pMouseY_sensitivity = 0.002f;
    float pCamspeed= 0.5f;
    uint32_t pPrevTick;
    uint32_t pLastX = 0;
    uint32_t pLastY = 0;
};

#endif // CAMERA_H

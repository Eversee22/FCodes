#ifndef MNWCAMERA_H  
#define MNWCAMERA_H  
#include <glm/vec3.hpp> 
#include <iostream>  


//default camera value
const float SPEED = 400.0f;
const float SENSITIVITY = 0.3f;
const float ZOOM = 60.0f;
const float ZOOMLEFT = 30.0f;
const float ZOOMRIGHT = 89.0f;
const float YAW = 0.0f;
const float PITCH = 0.0f;

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class MNWCamera
{
public:
	//MNWCamera();
	MNWCamera(const glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3 target = glm::vec3(0.0f, 0.0f, -1.0f), const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));
	void setModelViewMatrix();
	void setShape(float viewAngle, float aspect, float Near, float Far);
	void setLook(glm::vec3 pos,glm::vec3 target,glm::vec3 up=glm::vec3(0.0f,1.0f,0.0f));
	void setLook(float posX, float posY, float posZ, float targetX, float targetY, float targetZ, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f);
	void slide(float du, float dv, float dn);
	void roll(float angle);
	void yaw(float angle);
	void pitch(float angle);
	void updateCamVecs();
	//float getDist();
	void processMouseMovement(float xoffset, float yoffset, bool constrainPitch=true);
	void processMouseScroll(float yoffset);
	void processKeyboard(Camera_Movement direction, float deltaTime=0.02f);
	void restore();
	//void rotateX(float angle);
	void rotateY(float angle,float radius,glm::vec3 position);
	float getZoom();
	glm::vec3 getPosition();
	glm::vec3 getDirection();
	const double * getViewMatrix();
	//void setYawPitch(float yaw, float pitch);
private:
	glm::vec3 m_pos;
	glm::vec3 m_target;
	glm::vec3 m_up;
	glm::vec3 u, v, n; //u:right,v:up,n:direction
	float movementSpeed;
	float mouseSensitivity;
	float Zoom;
	float Yaw;
	float Pitch;
	float CYaw;
	float CPitch;
	glm::vec3 Rv[4];
	glm::vec3 AsNF;
	double m[16];
	void updateYawPitch();
};

#endif // MNWCAMERA_H  
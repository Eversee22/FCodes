#include "mnwCamera.h"
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

//MNWCamera::MNWCamera()
//{
//}

MNWCamera::MNWCamera(const glm::vec3 pos, const glm::vec3 target, const glm::vec3 up) :
	movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	//Yaw(YAW), Pitch(PITCH),CYaw(YAW),CPitch(PITCH)
{
	m_pos = pos;
	m_target = target;
	m_up = glm::normalize(up);
	n = (target - pos);
	u = glm::cross(up, n);
	v = glm::cross(n, u);

	n = glm::normalize(n);
	u = glm::normalize(u);
	v = glm::normalize(v);

	Rv[0] = m_pos;
	Rv[1] = u;
	Rv[2] = v;
	Rv[3] = n;
	updateYawPitch();
	//setModelViewMatrix();
}

void MNWCamera::setModelViewMatrix()
{
	//double m[16];
	m[0] = u.x; m[4] = u.y; m[8] = u.z; m[12] = -glm::dot(m_pos,u);
	m[1] = v.x; m[5] = v.y; m[9] = v.z; m[13] = -glm::dot(m_pos,v);
	m[2] = n.x; m[6] = n.y; m[10] = n.z; m[14] = -glm::dot(m_pos,n);
	m[3] = 0;  m[7] = 0;  m[11] = 0;  m[15] = 1.0;
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(m); //用M矩阵替换原视点矩阵 
}

void MNWCamera::setShape(float viewAngle, float aspect, float n, float f)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();                                   //设置当前矩阵模式为投影矩阵并归一化  
	gluPerspective(viewAngle, aspect, n, f);       //对投影矩阵进行透视变换  
	AsNF = glm::vec3(aspect, n, f);
}

void MNWCamera::setLook(glm::vec3 pos, glm::vec3 target,glm::vec3 up)
{
	m_pos = pos;
	m_target = target;
	m_up = glm::normalize(up);
	n = (target - pos);
	u = glm::cross(up, n);
	v = glm::cross(n, u);

	n = glm::normalize(n);
	u = glm::normalize(u);
	v = glm::normalize(v);

	//store base vector for purpose of restoring
	Rv[0] = m_pos;
	Rv[1] = u;
	Rv[2] = v;
	Rv[3] = n;
	updateYawPitch();
	//setModelViewMatrix();
}

void MNWCamera::setLook(float posX, float posY, float posZ, float targetX, float targetY, float targetZ, float upX, float upY, float upZ)
{
	m_pos.x = posX; m_pos.y = posY; m_pos.z = posZ;
	m_target.x = targetX; m_target.y = targetY; m_target.z = targetZ;
	m_up = glm::normalize(glm::vec3(upX,upY,upZ));
	n = (m_target - m_pos);
	u = glm::cross(m_up, n);
	v = glm::cross(n, u);

	n = glm::normalize(n);
	u = glm::normalize(u);
	v = glm::normalize(v);

	//store base vector for purpose of restoring
	Rv[0] = m_pos;
	Rv[1] = u;
	Rv[2] = v;
	Rv[3] = n;
	updateYawPitch();
	//setModelViewMatrix();
}

void MNWCamera::slide(float du, float dv, float dn)
{
	/*du *= mouseSensitivity;
	dv *= mouseSensitivity;
	dn *= mouseSensitivity;*/
	/*glm::vec3 front;
	front.x = glm::cos(glm::radians(Pitch))*glm::sin(glm::radians(Yaw));
	front.y = glm::sin(glm::radians(Pitch));
	front.z = glm::cos(glm::radians(Pitch))*glm::cos(glm::radians(Yaw));
	front = glm::normalize(front);*/
	glm::mat3 b_uvn = glm::mat3(u, m_up, n);
	glm::vec3 dr = glm::vec3(du, dv, dn);
	m_pos = m_pos + b_uvn * dr;
	m_target = m_target + b_uvn * dr;
	//setModelViewMatrix();
}

void MNWCamera::roll(float angle)
{
	angle = glm::radians(angle*mouseSensitivity);
	float cs = cos(angle);
	float sn = sin(angle);
	glm::vec3 t(u);
	glm::vec3 s(v);
	
	u = t * cs - s * sn;
	v = t * sn + s * cs;
	

	//setModelViewMatrix();          //每次计算完坐标轴变化后调用此函数更新视点矩阵
}

void MNWCamera::yaw(float angle)
{
	/*angle = glm::radians(angle*mouseSensitivity);
	float cs = cos(angle);
	float sn = sin(angle);
	glm::vec3 t(n);
	glm::vec3 s(u);

	n = t * cs - s * sn;
	u = t * sn + s * cs;*/
	//setModelViewMatrix();
}

void MNWCamera::pitch(float angle)
{
	/*angle = glm::radians(angle*mouseSensitivity);
	float cs = cos(angle);
	float sn = sin(angle);
	glm::vec3 t(v);
	glm::vec3 s(n);

	v = t * cs - s * sn;
	n = t * sn + s * cs;

	setModelViewMatrix();*/
}

/* 
    y
    |__x
  z/
*/
void MNWCamera::updateCamVecs()
{
	glm::vec3 front;
	front.x = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	n = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	u = glm::normalize(glm::cross(m_up,front));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	v = glm::normalize(glm::cross(front,u));

	//setModelViewMatrix();
}

//float MNWCamera::getDist()
//{
//	float dist = glm::length(m_pos);
//	return dist;
//	
//}

void MNWCamera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f) {
			Pitch = 89.0f;
			//yoffset = 0;
		}

		if (Pitch < -89.0f) {
			Pitch = -89.0f;
			//yoffset = 0;
		}
	}
	if (Yaw > 360.0f)
		Yaw -= 360.0f;
	if (Yaw < -360.f)
		Yaw += 360.0f;
	//yaw(xoffset);
	//pitch(yoffset);
	updateCamVecs();
}


void MNWCamera::processMouseScroll(float yoffset)
{

	if (Zoom >= ZOOMLEFT && Zoom <= ZOOMRIGHT)
		Zoom -= yoffset;
	if (Zoom <= ZOOMLEFT)
		Zoom = ZOOMLEFT;
	if (Zoom >= ZOOMRIGHT)
		Zoom = ZOOMRIGHT;
	//setModelViewMatrix();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();                                   //设置当前矩阵模式为投影矩阵并归一化  
	gluPerspective(Zoom, AsNF.x, AsNF.y, AsNF.z);

}

void MNWCamera::processKeyboard(Camera_Movement direction, float deltaTime)
{
	//printf("deltaTime:%f\n", deltaTime);
	float velocity = movementSpeed * deltaTime;
	if (direction == FORWARD)
		//m_pos += n * velocity;
		slide(0, 0, velocity);
	if (direction == BACKWARD)
		//m_pos -= n * velocity;
		slide(0, 0, -velocity);
	if (direction == LEFT)
		//m_pos -= u * velocity;
		slide(-velocity,0,0);
	if (direction == RIGHT)
		//m_pos += u * velocity;
		slide(velocity, 0, 0);
	if (direction == UP)
		slide(0, velocity, 0);
	if (direction == DOWN)
		slide(0, -velocity, 0);

	//setModelViewMatrix();
}

void MNWCamera::restore()
{
	//printf("restore\n");
	m_pos = Rv[0];
	u = Rv[1];
	v = Rv[2];
	n = Rv[3];
	Pitch = CPitch;
	Yaw = CYaw;
	Zoom = ZOOM;
	//updateCamVecs();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();                                   //设置当前矩阵模式为投影矩阵并归一化  
	gluPerspective(Zoom, AsNF.x, AsNF.y, AsNF.z);
}

//void MNWCamera::rotateX(float angle)
//{
//	float d = getDist();
//	//d = sqrt((d + m_pos.y)*(d - m_pos.y));
//	int cnt = 100;
//	float theta = angle * mouseSensitivity / cnt;
//	float slide_d = -2 * d*sin(glm::radians(theta/2));
//	yaw(theta / 2);
//	for (; cnt != 0; --cnt)
//	{
//		slide(slide_d, 0, 0);
//		yaw(theta);
//	}
//	yaw(-theta / 2);
//}

void MNWCamera::rotateY(float angle,float radius,glm::vec3 position)
{
	glm::vec3 r;
	r.x = position.x * glm::cos(angle) + position.z*sin(angle);
	r.y = 0;
	r.z = position.z * glm::cos(angle) - position.x *glm::sin(angle);
	r = glm::normalize(r);
	n = glm::normalize(glm::cross(r,m_up));
	u = glm::normalize(glm::cross(m_up, n));
	v = glm::normalize(glm::cross(n, u));
	m_pos = radius * r;
	
}

float MNWCamera::getZoom()
{
	return Zoom;
}

glm::vec3 MNWCamera::getPosition()
{
	return m_pos;
}

glm::vec3 MNWCamera::getDirection()
{
	return n;
}

const double * MNWCamera::getViewMatrix()
{
	return m;
}


//void MNWCamera::setYawPitch(float yaw, float pitch)
//{
//	Yaw = CYaw = yaw;
//	Pitch = CPitch = pitch;
//	updateCamVecs();
//}

void MNWCamera::updateYawPitch() {
	Pitch = CPitch = glm::degrees(glm::asin(n.y));
	Yaw = CYaw = glm::degrees(glm::asin(n.x / glm::sqrt(1 - n.y*n.y)));
	//printf("n:%f,%f,%f, Pitch:%f,Yaw:%f\n",n.x,n.y,n.z,Pitch,Yaw);
}


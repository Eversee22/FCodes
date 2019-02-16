#ifndef COMMOUSEOP_H
#define COMMOUSEOP_H

#include <GL/glut.h>
#include <stdio.h>
#include "mnwCamera.h"

int lastx;
int lasty;
bool mouseLeftDown;
bool mouseRightDown;
bool mouseMiddleDown;

//camera parameter
glm::vec3 Position = glm::vec3(0.0, 0.0, 10.0f);
glm::vec3 Front = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 Up = glm::vec3(0.0f, 1.0f, .0f);
glm::vec3 WorldUp = Up;
glm::vec3 Right;
glm::vec3 Target;
float Yaw = 0.0f;
float Pitch = 0.0f;

//main camera
MNWCamera mnwCam(Position, Position + Front, Up);
const bool constrainPitch = true;

//mouse click event
void onMouse(int button, int state, int x, int y) {
	lastx = x;
	lasty = y;

	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			mouseLeftDown = true;
		}
		else if (state == GLUT_UP)
			mouseLeftDown = false;
		//printf("mouseleft&down(%s)\n", mouseLeftDown ? "true" : "false");
	}
	else if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_DOWN)
			mouseRightDown = true;
		else if (state == GLUT_UP)
			mouseRightDown = false;
		//printf("mouseright&down(%s)\n", mouseRightDown ? "true" : "false");
	}
	else if (button == GLUT_MIDDLE_BUTTON) {
		if (state == GLUT_DOWN)
			mouseMiddleDown = true;
		else if (state == GLUT_UP)
			mouseMiddleDown = false;
		//printf("mousemiddle&down(%s)\n",mouseMiddleDown ? "true" : "false");
	}
	else {
		//printf("mouseunknown&(code:%d,state:%d)\n", button, state);
	}
}

//mouse move event
void onMouseMove(int x, int y) {
	int dx = x - lastx;
	int dy = y - lasty;
	if (mouseLeftDown&&mouseRightDown) {
		//mnwCam.rotateX(dx);
		//mnwCam.rotateY(dy);
		//printf("mouseleftdown&move\n");
	}
	else if (mouseLeftDown) {
		mnwCam.processMouseMovement(dx, dy,constrainPitch);
	}
	else if (mouseRightDown) {
		mnwCam.roll(dx);
		//printf("mouserightdown&move\n");
	}
	else if (mouseMiddleDown) {
		mnwCam.slide(dx, dy, 0);
		//printf("mousemiddledown&move\n");
	}
	else if(!mouseLeftDown&&!mouseRightDown){
		//mnwCam.processMouseMovement(dx, dy, constrainPitch);
	}
	//printf("mouse move\n");
	lastx = x;
	lasty = y;
	glutPostRedisplay();
}

//mouse wheel event
void onMouseScroll(int button, int dir, int x, int y) {
	if (dir > 0) {
		mnwCam.processMouseScroll(1.0f);
	}
	else {
		mnwCam.processMouseScroll(-1.0f);
	}
	printf("mousewheel&up(%s)\n", dir>0 ? "true" : "false");
	glutPostRedisplay();
}

//void updateCamVecs() {
//	// Calculate the new Front vector
//	glm::vec3 front;
//	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
//	front.y = sin(glm::radians(Pitch));
//	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
//	Front = glm::normalize(front);
//	// Also re-calculate the Right and Up vector
//	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
//	Up = glm::normalize(glm::cross(Right, Front));
//
//}
#endif // !COMMOUSEOP_H

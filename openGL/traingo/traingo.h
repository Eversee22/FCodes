#pragma once
#include <GL/freeglut.h>
#include <glm/glm.hpp>

#include "mnwCamera.h"
//#include "mmodel.h"

MNWCamera trainCam;

const std::string CWP = "";

//light
//light-0:point,1:spot,2:direct,3:ambient
bool el0 = true;
bool el1 = false;
bool el2 = false;
bool el3 = false;

float radius = 320.0f;

unsigned int gtexId;

//Model rock;
//Model planet;
//Model nanosuit;

void drawTrain(float radius,float theta) {
	GLfloat train_mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };  //定义材质的环境光颜色
	GLfloat train_mat_diffuse[] = { 0.2f, 0.3f, 0.3f, 1.0f };  //定义材质的漫反射光颜色
	GLfloat train_mat_specular[] = { 1.0f, 1.0f, 1.0f, 0.0f };   //定义材质的镜面反射光颜色
	GLfloat train_mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };   //定义材质的辐射广颜色
	GLfloat train_mat_shininess = 10.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, train_mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, train_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, train_mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, train_mat_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, train_mat_shininess);

	glPushMatrix();
	glRotatef(theta, 0.0f, 1.0f, 0.0f);
	//cylinder1
	glTranslatef(radius, 0.0f, 0.0f);
	glutSolidCylinder(10, 25, 64, 32);
	//cylinder2
	glTranslatef(0.0f, 0.0f, 30.0f);
	glutSolidCylinder(10, 25, 64, 32);
	//cylinder3
	glTranslatef(0.0f, 0.0f, -60.0f);
	glutSolidCylinder(10, 25, 64, 32);
	//head
	GLfloat th_mat_diffuse[] = { 1.0f,0.85f,0.1f,1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, th_mat_diffuse);
	glTranslatef(0.0f, 0.0f, -7.0f);
	glutSolidCube(10.0f);
	glPopMatrix();
}

void drawScene() {
	//start
	glPushMatrix();

	//sun light
	GLfloat sun_light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat sun_light_ambient[] = { 0.6f, 0.2f, 0.1f, 1.0f };
	GLfloat sun_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat sun_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);
	//sun material
	GLfloat sun_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };  //定义材质的环境光颜色
	GLfloat sun_mat_diffuse[] = { 0.9f, 0.6f, 0.2f, 1.0f };  //定义材质的漫反射光颜色
	GLfloat sun_mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };   //定义材质的镜面反射光颜色
	GLfloat sun_mat_emission[] = { 0.9f, 0.2f, 0.1f, 1.0f };   //定义材质的辐射广颜色，为偏红色
	GLfloat sun_mat_shininess = 0.0f;
	//glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);
	//glMaterialf(GL_FRONT, GL_SHININESS, sun_mat_shininess);
	//glEnable(GL_LIGHT0);
	if (el0)
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);
	//draw sun
	glPushMatrix();
	glTranslatef(0.0f, 100.0f, -10.0f);
	glutSolidSphere(3.0, 64, 32);
	//glColor3f(0.1f, 1.0f, 0.1f);

	//spot light
	glm::vec3 position = trainCam.getPosition();
	glm::vec3 direction = trainCam.getDirection()+glm::vec3(0.0f,0.0f,0.0f);
	GLfloat spot_light_position[] = { position.x,position.y,position.z,1.0f };
	GLfloat spot_light_direction[] = { -direction.x,-direction.y,-direction.z };
	GLfloat spot_light_diffuse[] = { 1.0f, 246/255.0f, 143/255.0f, 1.0f };
	GLfloat spot_light_specular[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat spot_light_ambient[] = { 0.5f,0.5f,0.5f,1.0f };
	//spot light material
	//GLfloat spot_mat_specular[] = { 1.0f,1.0f,1.0f,1.0f };
	//GLfloat spot_mat_diffuse[] = { 1.0f, 0.5f, 0.1f, 1.0f };
	//GLfloat spot_mat_shininess = 128.0f;
	glLightfv(GL_LIGHT1, GL_DIFFUSE, spot_light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, spot_light_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, spot_light_ambient);
	glLightfv(GL_LIGHT1, GL_POSITION, spot_light_position);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_light_direction);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 60.0f);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 3.0f);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, spot_mat_specular);
	//glMaterialfv(GL_FRONT, GL_AMBIENT, spot_mat_diffuse);
	//glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, spot_mat_shininess);

	//direction light
	float dir_light_direction[] = { 0.0f,-20.0f,10.0f,0.0f };
	GLfloat dir_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat dir_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat dir_light_ambient[] = { 0.5f,0.5f,0.5f,1.0f };
	glLightfv(GL_LIGHT2, GL_POSITION, dir_light_direction);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, dir_light_diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, dir_light_specular);
	glLightfv(GL_LIGHT2, GL_AMBIENT, dir_light_ambient);
	

	//ambient light
	float amb_light_direction[] = { 0.0f,0.0f,0.0f,0.0f };
	GLfloat amb_light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat amb_light_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat amb_light_ambient[] = { 0.9f,0.9f,0.9f,1.0f };
	//glLightfv(GL_LIGHT3, GL_POSITION, dir_light_direction);
	glLightfv(GL_LIGHT3, GL_DIFFUSE, amb_light_diffuse);
	glLightfv(GL_LIGHT3, GL_SPECULAR, amb_light_specular);
	glLightfv(GL_LIGHT3, GL_AMBIENT, amb_light_ambient);
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb_light_ambient);
	

	//draw ground
	GLfloat ground_mat_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };  //定义材质的环境光颜色
	GLfloat ground_mat_diffuse[] = { 0.1f, 0.8f, 0.5f, 1.0f };  //定义材质的漫反射光颜色
	GLfloat ground_mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };   //定义材质的镜面反射光颜色
	GLfloat ground_mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };   //定义材质的辐射广颜色
	GLfloat ground_mat_shininess = 0.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, ground_mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ground_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ground_mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, ground_mat_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, ground_mat_shininess);

	float width = 300.0f;
	float length = 300.0f;
	glPopMatrix();

	glTranslatef(0.0f, -10.0f, 0.0f);
	//unsigned int textureId = loadTexture((CWP + "textures/grass.png").c_str(), GL_REPEAT,true);
	glBindTexture(GL_TEXTURE_2D, gtexId);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3d(-width, 0.0f, length);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3d(width, 0.0f, length);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3d(width, 0.0f, -length);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3d(-width, 0.0f, -length);
	glEnd();
	
	//draw cube
	GLfloat cube_mat_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };  //定义材质的环境光颜色
	GLfloat cube_mat_diffuse[] = { 240/255.0, 1.0f, 240/255.0, 1.0f };  //定义材质的漫反射光颜色
	GLfloat cube_mat_specular[] = { 0.9f, 0.9f, 0.9f, 1.0f };   //定义材质的镜面反射光颜色
	GLfloat cube_mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };   //定义材质的辐射广颜色
	GLfloat cube_mat_shininess = 10.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, cube_mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cube_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, cube_mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, cube_mat_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, cube_mat_shininess);
	float cubesize = 20.0f;

	//cube1
	glTranslatef(-width, cubesize / 2 + 1.0f, -length);
	//glColor3f(1.0f, 1.0f, 0.0f);
	glutSolidCube(cubesize);
	glTranslatef(0.0f, cubesize, 0.0f);
	glutSolidTeapot(cubesize / 2);
	glTranslatef(0.0f, -cubesize, 0.0f);

	//cube2
	glTranslatef(0.0f, 0.0f, 2*length);
	GLfloat cube_mat_diffuse1[] = { 1.0f,250/255.0,205/255.0,1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cube_mat_diffuse1);
	//glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidCube(cubesize);
	glTranslatef(0.0f, cubesize, 0.0f);
	glutSolidTeapot(cubesize / 2);
	glTranslatef(0.0f, -cubesize, 0.0f);

	//cube3
	glTranslatef(2*width, 0.0f, 0.0f);
	GLfloat cube_mat_diffuse2[] = { 65/255.0,105/255.0,225/255.0,1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cube_mat_diffuse2);
	//glColor3f(0.0f, 0.0f, 1.0f);
	glutSolidCube(cubesize);
	glTranslatef(0.0f, cubesize, 0.0f);
	glutSolidTeapot(cubesize / 2);
	glTranslatef(0.0f, -cubesize, 0.0f);

	//cube4
	glTranslatef(0.0f, 0.0f, -2*length);
	GLfloat cube_mat_diffuse3[] = { 147/255.0,112/255.0,219/255.0,1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, cube_mat_diffuse3);
	//glColor3f(1.0f, 0.5f, 0.5f);
	glutSolidCube(cubesize);
	glTranslatef(0.0f, cubesize, 0.0f);
	glutSolidTeapot(cubesize / 2);
	glTranslatef(0.0f, -cubesize, 0.0f);
	
	//scene over
	glPopMatrix();

	/*objects*/
	//obj1
	glPushMatrix();
	GLfloat obj1_mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };  //定义材质的环境光颜色
	GLfloat obj1_mat_diffuse[] = { 0.5f, 0.5f,0.5f,1.0f };  //定义材质的漫反射光颜色
	GLfloat obj1_mat_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };   //定义材质的镜面反射光颜色
	GLfloat obj1_mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };   //定义材质的辐射广颜色
	GLfloat obj1_mat_shininess = 5.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, obj1_mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, obj1_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, obj1_mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, obj1_mat_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, obj1_mat_shininess);
	glTranslatef(0.0f, -5.0f, -50.0f);
	//model.loadModel(CWP + "model/rock/rock.obj");
	glScalef(10.0f, 10.0f, 10.0f);
	//rock.Draw();
	glPopMatrix();

	//obj2
	glPushMatrix();
	GLfloat obj2_mat_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };  //定义材质的环境光颜色
	GLfloat obj2_mat_diffuse[] = { 0.5f, 0.5f,0.5f,1.0f };  //定义材质的漫反射光颜色
	GLfloat obj2_mat_specular[] = { 0.1f, 0.1f, 0.1f, 1.0f };   //定义材质的镜面反射光颜色
	GLfloat obj2_mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };   //定义材质的辐射广颜色
	GLfloat obj2_mat_shininess = 5.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, obj2_mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, obj2_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, obj2_mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, obj2_mat_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, obj2_mat_shininess);
	glTranslatef(0.0f, 108.0f,-50.0f);
	
	//planet.Draw();
	glPopMatrix();

	//obj3
	/*glPushMatrix();
	nanosuit.Draw();
	glPopMatrix();*/

}
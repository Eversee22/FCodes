#ifndef __SKYBOXH__
#define __SKYBOXH__

//#include<glad/glad.h>
#include <GL/freeglut.h>

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include<iostream>
#include<vector>

#include "stbimage.h"

#define GL_CLAMP_TO_EDGE 0x812F

unsigned int textureIDs[6];

//std::string dir = "F:/openGL/skybox/";

//std::vector<std::string> faces
//{
//	"right.jpg",
//	"left.jpg",
//	"top.jpg",
//	"bottom.jpg",
//	"front.jpg",
//	"back.jpg"
//};

typedef enum {
	SKYBOX_RIGHT,
	SKYBOX_LEFT,
	SKYBOX_TOP,
	SKYBOX_BOTTOM,
	SKYBOX_FRONT,
	SKYBOX_BACK
};

//const unsigned int MAP_WIDTH = 1024;
//const unsigned int CELL_WIDTH = 16;
//const unsigned int MAP = MAP_WIDTH * CELL_WIDTH / 2;

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path,int WrapWay=GL_CLAMP,bool GenMIPS=false)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;

	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapWay);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapWay);
		if(GenMIPS){
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			gluBuild2DMipmaps(GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, data);
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
			
		/** 创建纹理 */
		//gluBuild2DMipmaps(GL_TEXTURE_2D,format,width,height,format,GL_UNSIGNED_BYTE,data);
		stbi_image_free(data);
	}
	else
	{
		//std::cout << "Texture failed to load at path: " << path << std::endl;
		MessageBox(NULL,path,"Load Error", MB_OK);    /**< 如果载入失败则弹出对话框 */

		stbi_image_free(data);
		exit(0);
	}

	return textureID;
}

// loads a cubemap texture from 6 individual texture faces
// order:
//---------------------------------------------------
unsigned int loadCubemap(std::string path,std::vector<std::string> faces)
{
	for (int i = 0; i < faces.size(); ++i) {
		textureIDs[i] = loadTexture((path + faces[i]).c_str());
	}
	return 1;
}

void createSkyBox(float x, float y, float z,float box_width, float box_height, float box_length,bool noli=false) {
	/** 获得场景中光照状态 */
	//GLboolean lp;
	//glGetBooleanv(GL_LIGHTING, &lp);
	if(noli)
		glDisable(GL_LIGHTING);            /**< 关闭光照 */
	
	//start paint
	glPushMatrix();
	glTranslatef(x, y, z);
	//w:x, h:y, len:z, right hand
	glScalef(box_width, box_height, box_length);

	/** 绘制背面 */
	glBindTexture(GL_TEXTURE_2D, textureIDs[SKYBOX_BACK]);

	glBegin(GL_QUADS);

	/** 指定纹理坐标和顶点坐标 */
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, -1.0f);
	glEnd();

	/** 绘制前面 */
	glBindTexture(GL_TEXTURE_2D, textureIDs[SKYBOX_FRONT]);

	glBegin(GL_QUADS);

	/** 指定纹理坐标和顶点坐标 */
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,-1.0f,1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,1.0f,1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f,1.0f,1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f,1.0f);

	glEnd();

	/** 绘制底面 */
	glBindTexture(GL_TEXTURE_2D, textureIDs[SKYBOX_BOTTOM]);

	glBegin(GL_QUADS);

	/** 指定纹理坐标和顶点坐标 */
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);

	glEnd();

	/** 绘制顶面 */
	glBindTexture(GL_TEXTURE_2D, textureIDs[SKYBOX_TOP]);

	glBegin(GL_QUADS);

	/** 指定纹理坐标和顶点坐标 */
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0, -1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);

	glEnd();

	/** 绘制右面 */
	glBindTexture(GL_TEXTURE_2D, textureIDs[SKYBOX_RIGHT]);

	glBegin(GL_QUADS);

	/** 指定纹理坐标和顶点坐标 */
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);

	glEnd();

	/** 绘制左面 */
	glBindTexture(GL_TEXTURE_2D, textureIDs[SKYBOX_LEFT]);

	glBegin(GL_QUADS);

	/** 指定纹理坐标和顶点坐标 */
	glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);
	
	glEnd();

	//end paint
	glPopMatrix();

	if (noli)                         /** 恢复光照状态 */
		glEnable(GL_LIGHTING);
}

void deleteTextures() {
	for (int i = 0; i < 6; ++i)
		glDeleteTextures(1, &textureIDs[i]);
}

#endif //__SKYBOXH__

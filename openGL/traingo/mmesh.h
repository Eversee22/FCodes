#pragma once
#ifndef MMESH_H
#define MMESH_H

//#include <glad/glad.h> // holds all OpenGL type declarations
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#include <shaders/shader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	//glm::vec3 Tangent;
	// bitangent
	//glm::vec3 Bitangent;
};

struct Texture {
	unsigned int id;
	string type;
	string path;
};

class Mesh {
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;
	//unsigned int VAO;

	/*  Functions  */
	// constructor
	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

		for (int i = 0; i < vertices.size(); ++i) {
			vs.push_back(vertices[i].Position);
			vts.push_back(vertices[i].TexCoords);
			vns.push_back(vertices[i].Normal);
		}
		//glEnableClientState(GL_VERTEX_ARRAY);
		//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		// now that we have all the required data, set the vertex buffers and its attribute pointers.
		//setupMesh();
	}

	//~Mesh() {
	//	//glDisableClientState(GL_VERTEX_ARRAY);
	//	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//}

	// render the mesh
	void Draw(/*Shader shader*/)
	{

		//cout << "texture size:" << textures.size() << endl;
		//cout << "vertex size:" << vertices.size() << endl;
		//cout << " index size:" << indices.size() << endl;
		/*unsigned int max = 0,max3;
		for (int i = 0; i < indices.size()/3; i++) {
			max3 = indices[i * 3] >= indices[i * 3 + 1] ? (indices[i * 3] >= indices[i * 3 + 2] ? indices[i * 3] : indices[i * 3 + 2]) : (indices[i * 3 + 1] >= indices[i * 3 + 2] ? indices[i * 3 + 1] : indices[i * 3 + 2]);
			if (max3 > max)
				max = max3;
			cout << indices[i * 3] << "/" << indices[i * 3 + 1] <<"/"<< indices[i * 3 + 2] << endl;
		}
		cout << "max indices: " << max <<endl;*/
		/*for (int i = 0; i < vertices.size(); ++i) {
			cout <<i<<": "<< vertices[i].TexCoords.x << "," << vertices[i].TexCoords.y << endl;
		}*/
		//glm::vec2 vt;
		//glm::vec3 v;
		
		

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glVertexPointer(3, GL_FLOAT, 0, &vs[0]);
		
			glNormalPointer(GL_FLOAT, 0, &vns[0]);

			glTexCoordPointer(2, GL_FLOAT, 0, &vts[0]);
			
			
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, &indices[0]);

			//glBegin(GL_TRIANGLES);
			//if (indices.size() % 3 != 0) {
			//	cout << "indices error" << endl;
			//}
			//else {
			//	for (int i = 0; i < indices.size() / 3; ++i) {
			//		
			//		for (int j = 0; j < 3; ++j) {
			//			vt = vertices[indices[i * 3 + j]].TexCoords;
			//			v = vertices[indices[i * 3 + j]].Position;
			//			glTexCoord2f(vt.x, vt.y); 
			//			//glColor3f(sin(j)*0.5+0.5, 0.8, 0.5);
			//			glVertex3f(v.x, v.y, v.z);
			//		}
			//		
			//	}
			//
			//}
			//glEnd();

		}
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		// draw mesh
	}

private:
	vector<glm::vec3> vs;
	vector<glm::vec2> vts;
	vector<glm::vec3> vns;
	/*  Render data  */
	//unsigned int VBO, EBO;

	/*  Functions    */
	// initializes all the buffer objects/arrays
	void setupMesh()
	{
		// create buffers/arrays
		//glGenVertexArrays(1, &VAO);
		//glGenBuffers(1, &VBO);
		//glGenBuffers(1, &EBO);

		//glBindVertexArray(VAO);
		//// load data into vertex buffers
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//// A great thing about structs is that their memory layout is sequential for all its items.
		//// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
		//// again translates to 3/2 floats which translates to a byte array.
		//glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		//// set the vertex attribute pointers
		//// vertex Positions
		//glEnableVertexAttribArray(0);
		//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		//// vertex normals
		//glEnableVertexAttribArray(1);
		//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		//// vertex texture coords
		//glEnableVertexAttribArray(2);
		//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		////// vertex tangent
		////glEnableVertexAttribArray(3);
		////glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		////// vertex bitangent
		////glEnableVertexAttribArray(4);
		////glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

		//glBindVertexArray(0);
	}
};
#endif
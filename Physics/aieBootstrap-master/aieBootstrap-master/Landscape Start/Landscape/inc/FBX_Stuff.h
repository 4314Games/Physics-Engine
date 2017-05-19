#pragma once
#include "FBXFile.h"
#include "gl_core_4_4.h"
#include <glm/glm.hpp>

class GLMesh
{
public:
	unsigned int vao;
	unsigned int vbo;
	unsigned int ibo;
};

class FBX_Stuff
{
public:
	FBX_Stuff();
	~FBX_Stuff();

	void FBXAnimationShader(unsigned int p_shader);
	void FBXModelShader();
	void CreateFBXOpenGLBuffers(FBXFile *fbx);
	void CleanupFBXOpenGLBuffers(FBXFile *file);
	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);

	FBXSkeleton *skeleton;
	FBXAnimation *animation;
	FBXFile *m_myFbxModel;
	glm::vec3 spearPos;
	unsigned int m_fbxModelShader;  //FBX Model/Text only
	FBXFile *m_ghoulModel;
	unsigned int m_fbxShader;		//FBX Animation 
	
};


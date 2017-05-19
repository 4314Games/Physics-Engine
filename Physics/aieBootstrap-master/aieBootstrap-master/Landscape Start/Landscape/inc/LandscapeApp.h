#pragma once
#include "Application.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include "FBXFile.h"
#include "ParticleEmitter.h"
#include "PostProcessing.h"
#include <gl_core_4_4.h>
#include "FBX_Stuff.h"

class Camera;

namespace aie
{
	class Texture;
}

class LandscapeApp : public aie::Application {
public:
	
	LandscapeApp();
	virtual ~LandscapeApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void DrawGrid();

	//Shaders
	void LoadShader();
	void UnloadShader();

	//Landscape
	void CreateLandscape();
	void DrawLandscape();

	//FBX stuff
	void FBXModelLoader(FBXFile *fbxModel, float p_scale, unsigned int p_shader, glm::vec3 p_position);	

	//Frustum
	vec3 spherePos = vec3(1, 1, 1);
	float radius = 5.0f;
	bool CalculateFrustum(glm::mat4& transform, const GLfloat& xPos, const GLfloat &yPos, const GLfloat &zPos, const GLfloat& radius);	
	bool sphereRendering;

	//Shader Debugging
	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);	
	
protected:

	Camera *m_camera;
	
	PostProcessing *m_postProcessing;

	//Textures
	aie::Texture *m_texture;
	aie::Texture *m_heightMap;
	aie::Texture *m_grass;
	aie::Texture *m_snow;
	aie::Texture *m_rock;
	aie::Texture *m_splatMap;
	glm::mat4	m_projectionMatrix;

	//Shader
	unsigned int m_shader;
	unsigned int m_vertCount;
	unsigned int m_IndicesCount;
	unsigned int m_Vao;
	unsigned int m_Vbo;
	unsigned int m_Ibo;
	
	//Landscape
	const int m_landWidth = 256;
	const int m_landLength = 256;
	const float m_vertseperation = 0.1f;
	const float m_heighMax = 5.0f;
	glm::vec3 pointA;
	glm::vec3 pointB;
	glm::vec3 pointC;
	glm::vec3 pointD;
	glm::vec3 normal;

	//Lighting
	glm::vec3 m_lightPosition;
	glm::vec3 m_lightColor;
	float m_ambientLightStrength;
	float specPower = 100.0f;
	glm::vec3 camPos;
	glm::vec3 specColor = glm::vec3(1,1,1);

	struct Vertex
	{
		glm::vec4 pos;
		glm::vec2 uv;
		glm::vec4 vNormal;
		
		static void SetupVertexAttribPointers();
	};

	//FBX	
	FBX_Stuff *m_FBX_Stuff;

	//particles
	ParticleEmitter *m_emitter;
	unsigned int m_particleShader;
	
};
#pragma once
#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "FBXFile.h"

using glm::vec3;
using glm::vec4;

struct Particle {
	vec3 position;
	vec3 velocity;
	vec4 colour;
	float size;
	float lifetime;
	float lifespan;
};

struct ParticleVertex {
	vec4 position;
	vec4 colour;
};

class ParticleEmitter {
public:
	ParticleEmitter();
	virtual ~ParticleEmitter();
	void initalise(unsigned int a_maxParticles, unsigned int a_emitRate, float a_lifetimeMin, float a_lifetimeMax, float a_velocityMin, float a_velocityMax, float a_startSize, float a_endSize, const vec4& a_startColour, const vec4& a_endColour, vec3 a_position);
	void shader(unsigned int &p_shader);
	void emit(glm::vec3 p_position);
	void update(float a_deltaTime, const glm::mat4 & a_cameraTransform, glm::vec3 camPos, glm::vec3 p_position, glm::vec4 p_startColor, glm::vec4 p_endColor);
	void draw();
	vec4 m_startColour;
	vec4 m_endColour;

protected:
	Particle* m_particles;
	unsigned int m_firstDead;
	unsigned int m_maxParticles;
	unsigned int m_vao, m_vbo, m_ibo;
	
	ParticleVertex* m_vertexData;

	vec3 m_position;
	float m_emitTimer;
	float m_emitRate;
	float m_lifespanMin;
	float m_lifespanMax;
	float m_velocityMin;
	float m_velocityMax;
	float m_startSize;
	float m_endSize;
	
};
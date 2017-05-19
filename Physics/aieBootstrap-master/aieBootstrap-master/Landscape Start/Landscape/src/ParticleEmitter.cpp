#include "ParticleEmitter.h"
#include "gl_core_4_4.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Camera.h"

ParticleEmitter::ParticleEmitter()
	: m_particles(nullptr),
	m_firstDead(0),
	m_maxParticles(0),
	m_position(0, 0, 0),
	m_vao(0), m_vbo(0), m_ibo(0),
	m_vertexData(nullptr) {

}

ParticleEmitter::~ParticleEmitter()
{
	delete[] m_particles;
	delete[] m_vertexData;
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}

void ParticleEmitter::initalise(unsigned int a_maxParticles, unsigned int a_emitRate, float a_lifetimeMin, float a_lifetimeMax, float a_velocityMin, float a_velocityMax, float a_startSize, float a_endSize, const vec4 & a_startColour, const vec4 & a_endColour, vec3 a_position)
{
	// set up emit timers
	m_emitTimer = 0;
	m_emitRate = 1.0f / a_emitRate;
	// store all variables passed in
	m_startColour = a_startColour;
	m_endColour = a_endColour;
	m_startSize = a_startSize;
	m_endSize = a_endSize;
	m_velocityMin = a_velocityMin;
	m_velocityMax = a_velocityMax;
	m_lifespanMin = a_lifetimeMin;
	m_lifespanMax = a_lifetimeMax;
	m_maxParticles = a_maxParticles;
	m_position = a_position;
	// create particle array
	m_particles = new Particle[m_maxParticles];
	m_firstDead = 0;
	// create the array of vertices for the particles
	// 4 vertices per particle for a quad.
	// will be filled during update
	m_vertexData = new ParticleVertex[m_maxParticles * 4];

	// continued next page...
	// create the index buffeer data for the particles
	// 6 indices per quad of 2 triangles
	// fill it now as it never changes
	unsigned int* indexData = new unsigned int[m_maxParticles * 6];

	for (unsigned int i = 0; i < m_maxParticles; ++i) {
		indexData[i * 6 + 0] = i * 4 + 0;
		indexData[i * 6 + 1] = i * 4 + 1;
		indexData[i * 6 + 2] = i * 4 + 2;
		indexData[i * 6 + 3] = i * 4 + 0;
		indexData[i * 6 + 4] = i * 4 + 2;
		indexData[i * 6 + 5] = i * 4 + 3;
	}

	// create opengl buffers
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * 4 *
		sizeof(ParticleVertex), m_vertexData,
		GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxParticles * 6 *
		sizeof(unsigned int), indexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // colour
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
		sizeof(ParticleVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
		sizeof(ParticleVertex), ((char*)0) + 16);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	delete[] indexData;
}

void ParticleEmitter::shader(unsigned int &p_shader)
{
	static const char* vertex_shader =
		"#version 400\n									\
	in vec4 vPosition;\n							\
	in vec4 vColor;\n								\
	out vec4 fColor;\n								\
	uniform mat4 projectionView; \n					\
	void main ()\n									\
	{\n												\
	  fColor = vColor;\n							\
	  gl_Position = projectionView * vPosition;\n	\
	}";

	static const char* fragment_shader =
		"#version 400\n				\
	in vec4 fColor;\n			\
	out vec4 frag_color;\n		\
	void main ()\n				\
	{\n							\
	  frag_color = fColor;\n	\
	}";

	// Step 1:
	// Load the vertex shader, provide it with the source code and compile it.
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);

	// Step 2:
	// Load the fragment shader, provide it with the source code and compile it.
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	// step 3:
	// Create the shader program
	p_shader = glCreateProgram();

	// Step 4:
	// attach the vertex and fragment shaders to the m_shader program
	glAttachShader(p_shader, vs);
	glAttachShader(p_shader, fs);

	// Step 5:
	// describe the location of the shader inputs the link the program
	glBindAttribLocation(p_shader, 0, "vPosition");
	glBindAttribLocation(p_shader, 1, "vColor");
	glLinkProgram(p_shader);

	// step 6:
	// delete the vs and fs shaders
	glDeleteShader(vs);
	glDeleteShader(fs);
}

void ParticleEmitter::emit(glm::vec3 p_position) {
	// only emit if there is a dead particle to use
	if (m_firstDead >= m_maxParticles)
		return;

	// resurrect the first dead particle
	Particle& particle = m_particles[m_firstDead++];

	// assign its starting position
	particle.position = p_position;

	// randomise its lifespan
	particle.lifetime = 0;
	particle.lifespan = (rand() / (float)RAND_MAX) * (m_lifespanMax - m_lifespanMin) + m_lifespanMin;

	// set starting size and colour
	particle.colour = m_startColour;
	particle.size = m_startSize;

	// randomise velocity direction and strength
	float velocity = (rand() / (float)RAND_MAX) * (m_velocityMax - m_velocityMin) + m_velocityMin;
	particle.velocity.x = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity.y = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity.z = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity = glm::normalize(particle.velocity) * velocity;
}

void ParticleEmitter::update(float a_deltaTime, const glm::mat4 & a_cameraTransform, glm::vec3 camPos, glm::vec3 p_position, glm::vec4 p_startColor, glm::vec4 p_endColor)
{
	using glm::vec3;
	using glm::vec4;

	m_endColour = p_endColor;
	m_startColour = p_startColor;
	// spawn particles
	m_emitTimer += a_deltaTime;
	while (m_emitTimer > m_emitRate) {
		emit(p_position);
		m_emitTimer -= m_emitRate;
	}
	unsigned int quad = 0;
	// update particles and turn live particles into billboard quads
	for (unsigned int i = 0; i < m_firstDead; i++) {
		Particle* particle = &m_particles[i];
		particle->lifetime += a_deltaTime;
		if (particle->lifetime >= particle->lifespan) {
			// swap last alive with this one
			*particle = m_particles[m_firstDead - 1];
			m_firstDead--;
		}
		else {
			// move particle
			particle->position += particle->velocity * a_deltaTime;
			// size particle
			particle->size = glm::mix(m_startSize, m_endSize,
				particle->lifetime / particle->lifespan);
			// colour particle
			particle->colour = glm::mix(m_startColour, m_endColour,
				particle->lifetime / particle->lifespan);
			// make a quad the correct size and colour
			float halfSize = particle->size * 0.5f;
			m_vertexData[quad * 4 + 0].position = vec4(halfSize,
				halfSize, 0, 1);
			m_vertexData[quad * 4 + 0].colour = particle->colour;
			m_vertexData[quad * 4 + 1].position = vec4(-halfSize,
				halfSize, 0, 1);
			m_vertexData[quad * 4 + 1].colour = particle->colour;
			m_vertexData[quad * 4 + 2].position = vec4(-halfSize,
				-halfSize, 0, 1);
			m_vertexData[quad * 4 + 2].colour = particle->colour;
			m_vertexData[quad * 4 + 3].position = vec4(halfSize,
				-halfSize, 0, 1);
			m_vertexData[quad * 4 + 3].colour = particle->colour;

			// create billboard transform
			vec3 zAxis = glm::normalize(camPos - particle->position);
			vec3 xAxis = glm::cross(vec3(a_cameraTransform[1]), zAxis);
			vec3 yAxis = glm::cross(zAxis, xAxis);
			glm::mat4 billboard(vec4(xAxis, 0),
				vec4(yAxis, 0),
				vec4(zAxis, 0),
				vec4(0, 0, 0, 1));
			m_vertexData[quad * 4 + 0].position = billboard *
				m_vertexData[quad * 4 + 0].position +
				vec4(particle->position, 0);
			m_vertexData[quad * 4 + 1].position = billboard *
				m_vertexData[quad * 4 + 1].position +
				vec4(particle->position, 0);
			m_vertexData[quad * 4 + 2].position = billboard *
				m_vertexData[quad * 4 + 2].position +
				vec4(particle->position, 0);
			m_vertexData[quad * 4 + 3].position = billboard *
				m_vertexData[quad * 4 + 3].position +
				vec4(particle->position, 0);
			++quad;
		}
	}
}

void ParticleEmitter::draw() {
	// sync the particle vertex buffer
	// based on how many alive particles there are
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_firstDead * 4 *
		sizeof(ParticleVertex), m_vertexData);
	// draw particles
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_firstDead * 6, GL_UNSIGNED_INT, 0);
}
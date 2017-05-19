#include "PostProcessing.h"
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

PostProcessing::PostProcessing()
{
}

PostProcessing::~PostProcessing()
{
}

void PostProcessing::CreateFrameBufferObject(int p_windowWidth, int p_windowHeight)
{
	glGenFramebuffers(1, &pp_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, pp_fbo);
	glGenTextures(1, &pp_fboTexture);
	glBindTexture(GL_TEXTURE_2D, pp_fboTexture);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, p_windowWidth, p_windowHeight);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, pp_fboTexture, 0);
	glGenRenderbuffers(1, &pp_fboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, pp_fboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, p_windowWidth, p_windowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, pp_fboDepth);
	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PostProcessing::SettingFullscreenQuad(int p_windowWidth, int p_windowHeight)
{
	glm::vec2 texelSize = 1.0f / glm::vec2(p_windowWidth,p_windowHeight);
	// fullscreen quad 
	glm::vec2 halfTexel = 1.0f / glm::vec2(p_windowWidth, p_windowHeight) * 0.5f;
	float vertexData[] = {
		-1, -1, 0, 1, halfTexel.x, halfTexel.y,
		1, 1, 0, 1, 1 - halfTexel.x, 1 - halfTexel.y,
		-1, 1, 0, 1, halfTexel.x, 1 - halfTexel.y,
		-1, -1, 0, 1, halfTexel.x, halfTexel.y, 1,
		-1, 0, 1, 1 - halfTexel.x, halfTexel.y,
		1, 1, 0, 1, 1 - halfTexel.x, 1 - halfTexel.y,
	};

	glGenVertexArrays(1, &pp_vao);
	glBindVertexArray(pp_vao);
	glGenBuffers(1, &pp_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pp_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 6, vertexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, ((char*)0) + 16);
}

void PostProcessing::PostProcessingShader()
{
	static const char* vertex_shader =
		"#version 410 \n\
		layout (location = 0) in vec4 position; \n\
		layout (location = 1) in vec2 texCoord; \n\
		out vec2 fTexCoord; \n\
		void main() \n\
		{ \n\
			gl_Position = position; \n\
			fTexCoord = texCoord;\n\
		 }";

	static const char* fragment_shader =
		"#version 410 \n\
		in vec2 fTexCoord; \n\
		out vec4 fragColour; \n\
	uniform sampler2D target; \n\
	// just sample the target and return the colour \n\
	vec4 Simple() { \n\
		return texture(target, fTexCoord);\n\
	} \n\
	\n\
	vec4 Distort() {									   \n\
		vec2 mid = vec2(0.5f);							   \n\
	float distanceFromCentre = distance(fTexCoord, mid);   \n\
	vec2 normalizedCoord = normalize(fTexCoord - mid);	   \n\
	float bias = distanceFromCentre +					   \n\
		sin(distanceFromCentre * 15) * 0.02f;			   \n\
	vec2 newCoord = mid + bias * normalizedCoord;		   \n\
	return texture(target, newCoord);					   \n\
	}													   \n\
	\n\
	vec4 BoxBlur() {\n\
		vec2 texel = 1.0f / textureSize(target, 0).xy;\n\
		// 9-tap box kernel\n\
		vec4 colour = texture(target, fTexCoord);						   \n\
		colour += texture(target, fTexCoord + vec2(-texel.x, texel.y));	   \n\
		colour += texture(target, fTexCoord + vec2(-texel.x, 0));		   \n\
		colour += texture(target, fTexCoord + vec2(-texel.x, -texel.y));   \n\
		colour += texture(target, fTexCoord + vec2(0, texel.y));		   \n\
		colour += texture(target, fTexCoord + vec2(0, -texel.y));		   \n\
		colour += texture(target, fTexCoord + vec2(texel.x, texel.y));	   \n\
		colour += texture(target, fTexCoord + vec2(texel.x, 0));		   \n\
		colour += texture(target, fTexCoord + vec2(texel.x, -texel.y));	   \n\
		return colour / 9;												   \n\
	}\n\
	void main() { \n\
		fragColour = Distort(); \n\
	}";



	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	pp_postShaderProgram = glCreateProgram();

	glAttachShader(pp_postShaderProgram, vs);
	glAttachShader(pp_postShaderProgram, fs);

	glBindAttribLocation(pp_postShaderProgram, 0, "position");
	glBindAttribLocation(pp_postShaderProgram, 1, "texCoord");
	glLinkProgram(pp_postShaderProgram);

	glDeleteShader(vs);
	glDeleteShader(fs);
}

void PostProcessing::DrawPostProcessing(int p_windowWidth, int p_windowHeight)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, p_windowWidth, p_windowHeight);
	// just clear the back-buffer depth as
	// each pixel will be filled
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(pp_postShaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, pp_fboTexture);
	int loc = glGetUniformLocation(pp_postShaderProgram, "target");
	glUniform1i(loc, 0);
	glBindVertexArray(pp_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glUseProgram(0);
}

#pragma once
#include <gl_core_4_4.h>

class PostProcessing
{
public:
	PostProcessing();
	~PostProcessing();

	void CreateFrameBufferObject(int p_windowWidth, int p_windowHeight);
	void SettingFullscreenQuad(int p_windowWidth, int p_windowHeight);
	void PostProcessingShader();
	void DrawPostProcessing(int p_windowWidth, int p_windowHeight);
	bool pp_usePostProcessing = false;
	unsigned int getVBO() { return pp_fbo; }
	unsigned int getPPShader() { return pp_postShaderProgram;  }

protected:
	unsigned int pp_vbo;
	unsigned int pp_vao;
	unsigned int pp_fbo;
	unsigned int pp_fboTexture;
	unsigned int pp_fboDepth;
	unsigned int pp_postShaderProgram;
	
};


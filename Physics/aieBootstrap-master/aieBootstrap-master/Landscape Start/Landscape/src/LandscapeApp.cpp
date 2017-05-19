#include "LandscapeApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Camera.h"
#include "gl_core_4_4.h"
#include <Texture.h>
#include <imgui.h>
#include <iostream>
using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

LandscapeApp::LandscapeApp() {

}

LandscapeApp::~LandscapeApp() {

}

bool LandscapeApp::startup() {
	
	setBackgroundColour(0.0f, 0.0f, 0.0f);	//Sets background color
	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	//Setup camera starting pos
	m_camera = new Camera(this);
	m_camera->SetPosition(glm::vec3(10.0f, 10.0f, 10.0f));
	m_camera->LookAt(glm::vec3(0.0f, 0.0f, 0.0f));

	//light
	m_lightPosition = glm::vec3(0.0f, 5.0f, 0.0f);
	m_lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	m_ambientLightStrength = 0.25f;

	//textures
	m_texture = new aie::Texture();
	m_texture->load("./textures/Tile.png");

	m_heightMap = new aie::Texture();
	m_heightMap->load("./textures/myHeight.bmp");
	
	m_grass = new aie::Texture();
	m_grass->load("./textures/grass.png");

	m_rock = new aie::Texture();
	m_rock->load("./textures/rock.png");

	m_snow = new aie::Texture();
	m_snow->load("./textures/water.jpg");

	m_splatMap = new aie::Texture();
	m_splatMap->load("./textures/mySplat.bmp");

	//Partciles
	m_emitter = new ParticleEmitter();
	m_emitter->initalise(1000, 500, 0.1f, 0.5f, 1, 5, 0.5f, 0.1f, glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1), glm::vec3(10,3,10));
	m_emitter->shader(m_particleShader);	

	//FBX
	m_FBX_Stuff = new FBX_Stuff();

	//Soul Spear
	m_FBX_Stuff->m_myFbxModel = new FBXFile();
	m_FBX_Stuff->m_myFbxModel->load("./models/soulspear/soulspear.fbx", FBXFile::UNITS_CENTIMETER);
	m_FBX_Stuff->CreateFBXOpenGLBuffers(m_FBX_Stuff->m_myFbxModel);
	m_FBX_Stuff->FBXModelShader();

	//Ghoul with animation
	m_FBX_Stuff->m_ghoulModel = new FBXFile();
	m_FBX_Stuff->m_ghoulModel->load("./micro_ghoul/models/micro_ghoul_animated.fbx");
	m_FBX_Stuff->CreateFBXOpenGLBuffers(m_FBX_Stuff->m_ghoulModel);
	m_FBX_Stuff->skeleton = new FBXSkeleton();
	m_FBX_Stuff->animation = new FBXAnimation();
	m_FBX_Stuff->FBXAnimationShader(m_FBX_Stuff->m_fbxShader);

	//Post Processing
	m_postProcessing = new PostProcessing();	
	m_postProcessing->CreateFrameBufferObject(getWindowWidth(), getWindowHeight());
	m_postProcessing->SettingFullscreenQuad(getWindowWidth(), getWindowHeight());
	m_postProcessing->PostProcessingShader();	

	//Landscape
	CreateLandscape();	
	LoadShader();					
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	return true;
}

void LandscapeApp::shutdown() {
	UnloadShader();
	Gizmos::destroy();		
	m_FBX_Stuff->CleanupFBXOpenGLBuffers(m_FBX_Stuff->m_myFbxModel);
	m_FBX_Stuff->CleanupFBXOpenGLBuffers(m_FBX_Stuff->m_ghoulModel);
	m_FBX_Stuff->m_myFbxModel->unload();
	delete m_FBX_Stuff->m_myFbxModel;
}

void LandscapeApp::update(float deltaTime) {

	//Light GUI
	float time = getTime();
	ImGui::Begin("PostProcessing");	
	ImGui::Checkbox("Distortion", &m_postProcessing->pp_usePostProcessing);
	ImGui::End();	

	bool visiblae = m_camera->isVisible(glm::vec3(1, 1, 1), glm::vec3(1, 2, 1));

	//Partcikes Gui
	ImGui::Begin("Particles");
	ImGui::ColorEdit4("Start Color", glm::value_ptr(m_emitter->m_startColour));
	ImGui::ColorEdit4("End Color", glm::value_ptr(m_emitter->m_endColour));	
	ImGui::End();
	
	//Light Gui
	ImGui::Begin("Light");
	ImGui::ColorEdit3("Color", glm::value_ptr(m_lightColor));
	ImGui::DragFloat("Spec Level", &specPower, 0.1f, 0, 1000.0f);
	ImGui::DragFloat("Light Strength", &m_ambientLightStrength, 0.1f, 0, 100);
	ImGui::End();

	//Frustum
	ImGui::Begin("Frustum");
	ImGui::Checkbox("Rendering Sphere", &sphereRendering);
	ImGui::End();
	
	m_camera->Update(deltaTime);	// rotate camera
	
	m_lightPosition = glm::vec3(vec3(glm::sin(time) * 3, 3, glm::cos(time) * 3));	//LightPos
	
	m_FBX_Stuff->skeleton = m_FBX_Stuff->m_ghoulModel->getSkeletonByIndex(0);		//Ghoul Skeleton	
	
	m_emitter->update(deltaTime, -m_camera->GetView(), m_camera->GetPos(), vec3((m_FBX_Stuff->spearPos.x + m_FBX_Stuff->skeleton->m_nodes[24]->m_globalTransform[3][0]), m_FBX_Stuff->spearPos.y + 4 + m_FBX_Stuff->skeleton->m_nodes[24]->m_globalTransform[3][1], m_FBX_Stuff->spearPos.z + m_FBX_Stuff->skeleton->m_nodes[24]->m_globalTransform[3][2]), m_emitter->m_startColour, m_emitter->m_endColour);	//particle emiiter
	
	//Ghoul	FBX Aniamtion
	m_FBX_Stuff->animation = m_FBX_Stuff->m_ghoulModel->getAnimationByIndex(0);
	m_FBX_Stuff->skeleton->evaluate(m_FBX_Stuff->animation, time);
	for (unsigned int bone_index = 0; bone_index < m_FBX_Stuff->skeleton->m_boneCount; ++bone_index)
	{
		m_FBX_Stuff->skeleton->m_nodes[bone_index]->updateGlobalTransform();
	}	
	
	//Gizmos
	Gizmos::clear();	
	Gizmos::draw(m_projectionMatrix * m_camera->GetView());	
	Gizmos::addSphere(spherePos, radius, 5, 5, glm::vec4(1, 1, 0, 0.5f));	
	
	//Frustum
	sphereRendering = CalculateFrustum(m_projectionMatrix * m_camera->GetView(), spherePos.x, spherePos.y, spherePos.z, radius);

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void LandscapeApp::DrawGrid()
{
	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}
}

void LandscapeApp::draw() {
	
	// wipe the screen to the background colour
	clearScreen();
	
	if (m_postProcessing->pp_usePostProcessing == true)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_postProcessing->getVBO());
		glViewport(0, 0, getWindowWidth(), getWindowHeight());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glPolygonMode(GL_FRONT_AND_BACK, GL_FRONT);

	// update perspective in case window resized
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
										  getWindowWidth() / (float)getWindowHeight(),
										  0.1f, 1000.f);

	// STEP 1: enable the shader program for rendering
	glUseProgram(m_shader);

	// Step 2: send uniform variables to the shader
	glm::mat4 projectionView = m_projectionMatrix * m_camera->GetView();
	glUniformMatrix4fv(
		glGetUniformLocation(m_shader, "projectionView"), 
		1, 
		false, 
		glm::value_ptr(projectionView));

	//Setup texture in opengl select the first texture as active then bind it
	//also set it up as a uniform variable for the shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture->getHandle());
	glUniform1i(glGetUniformLocation(m_shader, "texture"), 0);

	//Setup grass texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_grass->getHandle());
	glUniform1i(glGetUniformLocation(m_shader, "grass"), 1);

	//Setup snow texture
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_snow->getHandle());
	glUniform1i(glGetUniformLocation(m_shader, "snow"), 2);

	//Setup rock texture
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_rock->getHandle());
	glUniform1i(glGetUniformLocation(m_shader, "rock"), 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, m_splatMap->getHandle());
	glUniform1i(glGetUniformLocation(m_shader, "splat"), 4);	

	camPos = m_camera->GetPos();
	//Set up the light color information
	glUniform3fv(glGetUniformLocation(m_shader, "lightPosition"), 1, &m_lightPosition[0]);
	glUniform3fv(glGetUniformLocation(m_shader, "lightColor"), 1, &m_lightColor[0]);
	glUniform1fv(glGetUniformLocation(m_shader, "lightAmbientStrength"), 1, &m_ambientLightStrength);
	glUniform1fv(glGetUniformLocation(m_shader, "specPower"),1,&specPower);
	glUniform3fv(glGetUniformLocation(m_shader, "specColor"), 1, &specColor[0]);
	glUniform3fv(glGetUniformLocation(m_shader, "camPos"), 1, &camPos[0]);
	
	// Step 3: Bind the VAO
	// When we setup the geometry, we did a bunch of glEnableVertexAttribArray and glVertexAttribPointer method calls
	// we also Bound the vertex array and index array via the glBindBuffer call.
	// if we where not using VAO's we would have to do thoes method calls each frame here.
	glBindVertexArray(m_Vao);
	DrawLandscape();
	
	Gizmos::draw(m_projectionMatrix * m_camera->GetView());	

	//Draw FBX Models
	m_FBX_Stuff->spearPos = glm::vec3(m_FBX_Stuff->skeleton->m_nodes[23]->m_globalTransform[3][0] + 1.8f, m_FBX_Stuff->skeleton->m_nodes[23]->m_globalTransform[3][1]  + 1.2f, m_FBX_Stuff->skeleton->m_nodes[23]->m_globalTransform[3][2] + 3.0f);
	FBXModelLoader(m_FBX_Stuff->m_myFbxModel, 0.5f, m_FBX_Stuff->m_fbxModelShader, m_FBX_Stuff->spearPos);
	FBXModelLoader(m_FBX_Stuff->m_ghoulModel,2.0f, m_FBX_Stuff->m_fbxShader, glm::vec3(1,3,3));
	
	//Particle Shader
	glUseProgram(m_particleShader);
	int locc = glGetUniformLocation(m_particleShader, "projectionView");
	glUniformMatrix4fv(locc, 1, GL_FALSE, &projectionView[0][0]);
	m_emitter->draw();
	glUseProgram(0);	

	//PostProcessing
	if(m_postProcessing->pp_usePostProcessing == true) m_postProcessing->DrawPostProcessing(getWindowWidth(), getWindowHeight());

}

void LandscapeApp::FBXModelLoader(FBXFile *fbxModel, float p_scale, unsigned int p_shader, glm::vec3 p_position)
{
	//FBX Transform
	glm::mat4 projectionView = m_projectionMatrix * m_camera->GetView();
	float s = p_scale;	
	glm::mat4 model = glm::mat4(
		s, 0, 0, 0,
		0, s, 0, 0,
		0, 0, s, 0,
		p_position.x, p_position.y, p_position.z, 1	//position		
	);
	//Shader Setup
	glUseProgram(p_shader);
	m_FBX_Stuff->skeleton->updateBones();
	// send uniform variables, in this case the "projectionViewWorldMatrix"
	unsigned int mvpLoc = glGetUniformLocation(p_shader, "projectionViewWorldMatrix");
	glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, glm::value_ptr(projectionView * model));
	glUniform3fv(glGetUniformLocation(p_shader, "lightPosition"), 1, &m_lightPosition[0]);
	glUniform3fv(glGetUniformLocation(p_shader, "lightColor"), 1, &m_lightColor[0]);
	glUniform1fv(glGetUniformLocation(p_shader, "lightAmbientStrength"), 1, &m_ambientLightStrength);
	glUniform1fv(glGetUniformLocation(p_shader, "specPower"), 1, &specPower);
	glUniform3fv(glGetUniformLocation(p_shader, "specColor"), 1, &specColor[0]);
	glUniform3fv(glGetUniformLocation(p_shader, "camPos"), 1, &camPos[0]);
	// loop through each mesh within the fbx file
	for (unsigned int i = 0; i < fbxModel->getMeshCount(); ++i)
	{
		FBXMeshNode* mesh = fbxModel->getMeshByIndex(i);
		GLMesh* glData = (GLMesh*)mesh->m_userData;
		// get the texture from the model
		unsigned int diffuseTexture = fbxModel->getTextureByIndex(mesh->m_material->DiffuseTexture);
	
		int bones_location = glGetUniformLocation(p_shader, "bones");
		glUniformMatrix4fv(bones_location, m_FBX_Stuff->skeleton->m_boneCount, GL_FALSE, (float*)m_FBX_Stuff->skeleton->m_bones);

		// bid the texture and send it to our shader
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseTexture);
		glUniform1i(glGetUniformLocation(p_shader, "diffuseTexture"), 0);
		// draw the mesh
		glBindVertexArray(glData->vao);
		glDrawElements(GL_TRIANGLES, mesh->m_indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	glUseProgram(0);
}

void LandscapeApp::LoadShader()
{
	//Plane Landscape
	static const char* vertex_shader =
		"#version 400\n								\
	layout (location = 0) in vec4 vPosition;\n							\
	layout (location = 1) in vec2 vUv;\n									\
	layout (location = 2) in vec4 vNormal;\n								\
	out vec2 fUv;\n 								\
	out vec3 fPos;\n 								\
	out vec4 fNormal;\n 							\
	uniform mat4 projectionView; \n					\
	void main ()\n									\
	{\n												\
		fNormal = vNormal;\n						\
		fPos = vPosition.xyz;\n						\
		fUv = vUv;\n								\
	  gl_Position = projectionView * vPosition;\n	\
	}";

	static const char* fragment_shader =
		"#version 400\n											\
	in vec2 fUv;\n												\
	in vec3 fPos;\n 											\
	in vec4 fNormal;\n 											\
	out vec4 frag_color;\n										\
	uniform sampler2D grass;\n								\
	uniform sampler2D snow;\n								\
	uniform sampler2D rock;\n								\
	uniform sampler2D splat;\n\
	uniform float lightAmbientStrength;\n						\
	uniform vec3 lightPosition;\n								\
	uniform vec3 lightColor;\n									\
	uniform vec3 specColor;\n	\
	uniform vec3 camPos;\n\
	uniform float specPower;\n\
	void main ()\n												\
	{\n															\
	  vec3 norm = normalize(fNormal.xyz);\n						\
	  vec3 lightDir = normalize(lightPosition - fPos);\n		\
	  float diff = max(dot(norm,lightDir),0.0f);\n				\
	  vec3 diffColor = diff * lightColor;\n						\
	  vec3 ambient = lightColor * lightAmbientStrength;\n		\
	  vec3 r = reflect(-lightDir, norm);\n	\
	  vec3 temp = normalize(camPos - fPos);\n\
	  float specThing = pow(max(0.0f,dot(r,temp)), specPower);\n\
	  vec3 spec = specColor * specThing;\n\
	vec4 texColor;\n\
	vec4 sp = texture2D(splat, fUv);\n\
	texColor =  sp.x * texture2D(rock,fUv);\n\
	texColor += sp.y * texture2D(grass,fUv);\n\
	texColor += sp.z * texture2D(snow,fUv);\n\
	frag_color = texColor * vec4(ambient + diffColor + spec,1.0);\n\
}";	

	// Step 1:
	// Load the vertex shader, provide it with the source code and compile it.
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	CheckShaderError(vs, GL_COMPILE_STATUS, false, "Error:  Compiling Shader");

	// Step 2:
	// Load the fragment shader, provide it with the source code and compile it.
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);
	CheckShaderError(fs, GL_COMPILE_STATUS, false, "Error:  Compiling Shader");

	// step 3:
	// Create the shader program
	m_shader = glCreateProgram();

	// Step 4:
	// attach the vertex and fragment shaders to the m_shader program
	glAttachShader(m_shader, vs);
	glAttachShader(m_shader, fs);

	// Step 5:
	// describe the location of the shader inputs the link the program
	//glBindAttribLocation(m_shader, 0, "vPosition");
	//glBindAttribLocation(m_shader, 1, "vUv");
	//glBindAttribLocation(m_shader, 2, "vNormal");
	glLinkProgram(m_shader);
	CheckShaderError(m_shader, GL_LINK_STATUS, true, "Error: Linking Program");

	// step 6:
	// delete the vs and fs shaders
	glDeleteShader(vs);
	glDeleteShader(fs);
}

void LandscapeApp::UnloadShader()
{
	glDeleteProgram(m_shader);
	glDeleteProgram(m_FBX_Stuff->m_fbxShader);
	glDeleteProgram(m_particleShader);
	glDeleteProgram(m_FBX_Stuff->m_fbxModelShader);
	glDeleteProgram(m_postProcessing->getPPShader());
}

void LandscapeApp::Vertex::SetupVertexAttribPointers()
{
	// enable vertex position element
	// notice when we loaded the shader, we described the "position" element to be location 0.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute 0 (position)
		4,                  // size - how many floats make up the position (x, y, z, w)
		GL_FLOAT,           // type - our x,y,z, w are float values
		GL_FALSE,           // normalized? - not used
		sizeof(Vertex),     // stride - size of an entire vertex
		(void*)0            // offset - bytes from the beginning of the vertex
	);

	// enable vertex color element
	// notice when we loaded the shader, we described the "color" element to be location 1.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,                  // attribute 1 (texture)
		2,                  // size - how many floats make up the color (r, g, b, a)
		GL_FLOAT,           // type - our x,y,z are float values
		GL_FALSE,           // normalized? - not used
		sizeof(Vertex),     // stride - size of an entire vertex
		(void*)(sizeof(float) * 4)            // offset - bytes from the beginning of the vertex
	);

	//enabled vertex normal
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,                  // attribute 2 (position)
		4,                  // size - how many floats make up the position (x, y, z, w)
		GL_FLOAT,           // type - our x,y,z, w are float values
		GL_FALSE,           // normalized? - not used
		sizeof(Vertex),     // stride - size of an entire vertex
		(void*)(sizeof(float) * 6)          // offset - bytes from the beginning of the vertex
	);
}

void LandscapeApp::CreateLandscape()
{
	std::vector<Vertex> verts;
	std::vector<unsigned  int> indices;

	const unsigned char *pixels = m_heightMap->getPixels();
	
	for (int y = 0; y < m_landLength; y++)
	{
		for ( int x = 0; x < m_landWidth; x++)
		{
			//int i = y * m_landWidth + x;

			int sampleX = (float)x / m_landWidth * m_heightMap->getWidth();
			int sampleZ = (float)y / m_landLength * m_heightMap->getWidth();

			int i = sampleZ * m_heightMap->getWidth() + sampleX;
			
			float xPos = (x * m_vertseperation) - (m_landWidth * m_vertseperation * 0.5f);
			/*int dick = (rand() % 10);
			float cunt = (float)dick / 100;*/
			float yPos = (pixels[i * 3] / 255.0f) *  m_heighMax;	
			float zPos = (y * m_vertseperation) - (m_landLength * m_vertseperation * 0.5f);
		
			float u = (float)y / (m_landWidth - 1);
			float v = (float)x / (m_landLength - 1);

			Vertex vert
			{				
				glm::vec4( xPos, yPos, zPos, 1.0f ),	//position
				glm::vec2( u,v ),						//texture cords(uv)
				glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)		//normal
			};
			verts.push_back(vert);
		}	
		
	}

	//calculate iindices
	for (int z = 0; z < m_landLength - 1; z++)
	{
		for (int x = 0; x < m_landWidth - 1; x++)
		{
			int i = z * m_landWidth + x; //the address of the vertices in the single dimension vector
		
			indices.push_back(i + 1);			// b--a
			indices.push_back(i);				// | /
			indices.push_back(i + m_landWidth);	// c

			indices.push_back(i + 1);					//   a
			indices.push_back(i + m_landWidth);			// / |
			indices.push_back(i +  m_landWidth + 1);	//c--d 

			pointA = (glm::vec3)verts[i + 1].pos;
			pointC = (glm::vec3)verts[i + m_landWidth].pos;
			pointB = (glm::vec3)verts[i].pos;
			
			normal = glm::cross(pointC - pointB, pointA - pointB);
			normal += glm::cross(pointB - pointA, pointC - pointA);
			normal += glm::cross(pointB - pointC, pointA - pointC);

			pointD = (glm::vec3)verts[i + m_landWidth + 1].pos;		//D			

			normal += glm::cross(pointD - pointA, pointC - pointA);
			normal += glm::cross(pointD - pointC, pointA - pointC);
			normal += glm::cross(pointA - pointD, pointC - pointD);

			glm::normalize(normal);			
			verts[i].vNormal = glm::vec4(normal, 0.0f);

			//float x = glm::reflect(camPos);
		}
	}


	m_vertCount = verts.size();
	m_IndicesCount = indices.size();

	// Generate the VAO and Bind bind it.
	// Our VBO (vertex buffer object) and IBO (Index Buffer Object) will be "grouped" with this VAO
	// other settings will also be grouped with the VAO. this is used so we can reduce draw calls in the render method.
	glGenVertexArrays(1, &m_Vao);
	glBindVertexArray(m_Vao);

	// Create our VBO and IBO.
	// Then tell Opengl what type of buffer they are used for
	// VBO a buffer in graphics memory to contains our vertices
	// IBO a buffer in graphics memory to contain our indices.
	// Then Fill the buffers with our generated data.
	// This is taking our verts and indices from ram, and sending them to the graphics card
	glGenBuffers(1, &m_Vbo);
	glGenBuffers(1, &m_Ibo);

	glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);

	glBufferData(GL_ARRAY_BUFFER, m_vertCount * sizeof(Vertex), &verts[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndicesCount * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	Vertex::SetupVertexAttribPointers();

	//unbind things after weve finished using them
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void LandscapeApp::DrawLandscape()
{
	glDrawElements(GL_TRIANGLES, m_IndicesCount, GL_UNSIGNED_INT, 0);
}

bool LandscapeApp::CalculateFrustum(glm::mat4& transform, const GLfloat &xPos, const GLfloat &yPos, const GLfloat &zPos, const GLfloat& radius)
{	
	// right side
	glm::vec4* planes;
	planes = new glm::vec4[6];
	planes[0] = glm::vec4(transform[0][3] - transform[0][0],
		transform[1][3] - transform[1][0],
		transform[2][3] - transform[2][0],
		transform[3][3] - transform[3][0]);
	GLfloat length = sqrtf(planes[0].x * planes[0].x + planes[0].y * planes[0].y + planes[0].z * planes[0].z);
	planes[0].x /= length;
	planes[0].y /= length;
	planes[0].z /= length;
	planes[0].w /= length;
	GLfloat distance = planes[0].x * xPos + planes[0].y * yPos + planes[0].z * zPos + planes[0].w;
	if (distance <= -radius)
	{
		return false; // Bounding sphere is completely outside the left plane
	}
	// left side
	planes[1] = glm::vec4(transform[0][3] + transform[0][0],
		transform[1][3] + transform[1][0],
		transform[2][3] + transform[2][0],
		transform[3][3] + transform[3][0]);
	length = sqrtf(planes[1].x * planes[1].x + planes[1].y * planes[1].y + planes[1].z * planes[1].z);
	planes[1].x /= length;
	planes[1].y /= length;
	planes[1].z /= length;
	planes[1].w /= length;
	distance = planes[1].x * xPos + planes[1].y * yPos + planes[1].z * zPos + planes[1].w;
	if (distance <= -radius)
	{
		return false; // Bounding sphere is completely outside the left plane
	}
	// top
	planes[2] = glm::vec4(transform[0][3] - transform[0][1],
		transform[1][3] - transform[1][1],
		transform[2][3] - transform[2][1],
		transform[3][3] - transform[3][1]);

	length = sqrtf(planes[2].x * planes[2].x + planes[2].y * planes[2].y + planes[2].z * planes[2].z);
	planes[2].x /= length;
	planes[2].y /= length;
	planes[2].z /= length;
	planes[2].w /= length;
	distance = planes[2].x * xPos + planes[2].y * yPos + planes[2].z * zPos + planes[2].w;
	if (distance <= -radius)
	{
		return false; // Bounding sphere is completely outside the left plane
	}
	// bottom
	planes[3] = glm::vec4(transform[0][3] + transform[0][1],
		transform[1][3] + transform[1][1],
		transform[2][3] + transform[2][1],
		transform[3][3] + transform[3][1]);
	length = sqrtf(planes[3].x * planes[3].x + planes[3].y * planes[3].y + planes[1].z * planes[3].z);
	planes[3].x /= length;
	planes[3].y /= length;
	planes[3].z /= length;
	planes[3].w /= length;
	distance = planes[3].x * xPos + planes[3].y * yPos + planes[3].z * zPos + planes[3].w;
	if (distance <= -radius)
	{
		return false; // Bounding sphere is completely outside the left plane
	}
	// far 
	planes[4] = glm::vec4(transform[0][3] - transform[0][2],
		transform[1][3] - transform[1][2],
		transform[2][3] - transform[2][2],
		transform[3][3] - transform[3][2]);
	length = sqrtf(planes[4].x * planes[4].x + planes[4].y * planes[4].y + planes[4].z * planes[4].z);
	planes[4].x /= length;
	planes[4].y /= length;
	planes[4].z /= length;
	planes[4].w /= length;
	distance = planes[4].x * xPos + planes[4].y * yPos + planes[4].z * zPos + planes[4].w;
	if (distance <= -radius)
	{
		return false; // Bounding sphere is completely outside the left plane
	}
	// near
	planes[5] = glm::vec4(transform[0][3] + transform[0][2],
		transform[1][3] + transform[1][2],
		transform[2][3] + transform[2][2],
		transform[3][3] + transform[3][2]);
	length = sqrtf(planes[5].x * planes[5].x + planes[5].y * planes[5].y + planes[5].z * planes[5].z);
	planes[5].x /= length;
	planes[5].y /= length;
	planes[5].z /= length;
	planes[5].w /= length;
	distance = planes[5].x * xPos + planes[5].y * yPos + planes[5].z * zPos + planes[5].w;
	if (distance <= -radius)
	{
		return false; // Bounding sphere is completely outside the left plane
	}
	return true;
}

void LandscapeApp::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string & errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);
	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}

}








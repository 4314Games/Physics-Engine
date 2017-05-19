#include "FBX_Stuff.h"
#include <iostream>

FBX_Stuff::FBX_Stuff()
{
}


FBX_Stuff::~FBX_Stuff()
{
}

void FBX_Stuff::FBXAnimationShader(unsigned int p_shader)
{
	//FBX Model
		const char* vsSource =
			"#version 410\n \
	layout(location=0) in vec4 position;\n\
layout(location = 1) in vec4 normal;\n\
	layout(location = 2) in vec4 tangent;\n\
	layout(location = 3) in vec2 texcoord;\n\
	layout(location = 4) in vec4 weights;\n\
	layout(location = 5) in vec4 indices;\n\
		out vec3 frag_normal; \n\
		out vec3 frag_position; \n\
		out vec3 frag_tangent;\n\
		out vec3 frag_bitangent;\n\
		out vec2 frag_texcoord;\n\
		uniform mat4 projectionViewWorldMatrix; \n\
		const int MAX_BONES = 128; \n\
		uniform mat4 bones[MAX_BONES];\n\
		void main() { \n\
			frag_position = position.xyz;\n\
				frag_normal = normal.xyz;\n\
			frag_tangent = tangent.xyz;\n\
			frag_bitangent = cross(normal.xyz, tangent.xyz);\n\
			frag_texcoord = texcoord;\n\
			// cast the indices to integer's so they can index an array\n\
			ivec4 index = ivec4(indices); \n\
			// sample bones and blend up to 4\n\
			vec4 P = bones[index.x] * position * weights.x;\n\
			P += bones[index.y] * position * weights.y;\n\
			P += bones[index.z] * position * weights.z;\n\
			P += bones[index.w] * position * weights.w;\n\
			gl_Position = projectionViewWorldMatrix  * P;\n\
		}";

	const char* fsSource =
		"#version 410\n \
		in vec3 frag_normal; \n\
		in vec2 frag_texcoord;\n\
		out vec4 frag_color;\n\
		uniform sampler2D diffuseTexture;\n\
		void main()\n\
		{\n\
			frag_color = texture2D(diffuseTexture,frag_texcoord);\n\
		}";

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	CheckShaderError(vertexShader, GL_COMPILE_STATUS, false, "Error: Compiling shader");
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);
	CheckShaderError(fragmentShader, GL_COMPILE_STATUS, false, "Error: Compiling shader");
	m_fbxShader = glCreateProgram();
	glAttachShader(m_fbxShader, vertexShader);
	glAttachShader(m_fbxShader, fragmentShader);
	glBindAttribLocation(m_fbxShader, 0, "position");
	glBindAttribLocation(m_fbxShader, 1, "normal");
	glBindAttribLocation(m_fbxShader, 2, "uv");
	glBindAttribLocation(m_fbxShader, 3, "tangent");
	glBindAttribLocation(m_fbxShader, 4, "weights");
	glBindAttribLocation(m_fbxShader, 5, "indicies");
	glLinkProgram(m_fbxShader);
	CheckShaderError(m_fbxShader, GL_LINK_STATUS, true, "Error: Linking Program");
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void FBX_Stuff::FBXModelShader()
{
	const char* vsSource =
				"#version 410\n \
		in vec4 position; \n\
in vec4 normal; \n\
layout(location = 3)in vec2 uv; \n\
out vec4 vNormal; \n\
out vec2 vuv; \n\
out vec3 fPos;\n\
uniform mat4 projectionViewWorldMatrix; \n\
void main() { \n\
 vNormal = normal; \n\
 vuv = uv; \n\
 gl_Position = projectionViewWorldMatrix*position; \n\
 fPos = position.xyz;\n\
}";
	const char* fsSource =
		"#version 410\n \
in vec4 vNormal; \n\
in vec2 vuv; \n\
in vec3 fPos;\n	\
out vec4 FragColor; \n\
uniform float lightAmbientStrength;\n						\
uniform vec3 lightPosition;\n								\
uniform vec3 lightColor;\n									\
uniform vec3 specColor;\n	\
uniform vec3 camPos;\n\
uniform float specPower;\n\
uniform sampler2D diffuseTexture; \n\
void main() { \n\
	  vec3 norm = normalize(vNormal.xyz);\n						\
	  vec3 lightDir = normalize(lightPosition - fPos);\n		\
	  float diff = max(dot(norm,lightDir),0.0f);\n				\
	  vec3 diffColor = diff * lightColor;\n						\
	  vec3 ambient = lightColor * lightAmbientStrength;\n		\
	  vec3 r = reflect(-lightDir, norm);\n	\
	  vec3 temp = normalize(camPos - fPos);\n\
	  float specThing = pow(max(0.0f,dot(r,temp)), specPower);\n\
	  vec3 spec = specColor * specThing;\n\
	  FragColor = texture2D(diffuseTexture, vuv) * vec4(ambient + diffColor + spec,1.0); \n\
}";
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const char**)&vsSource, 0);
	glCompileShader(vertexShader);
	CheckShaderError(vertexShader, GL_COMPILE_STATUS, false, "Error: compiling shader");
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const char**)&fsSource, 0);
	glCompileShader(fragmentShader);
	CheckShaderError(fragmentShader, GL_COMPILE_STATUS, false, "Error: compiling shader");
	m_fbxModelShader = glCreateProgram();
	glAttachShader(m_fbxModelShader, vertexShader);
	glAttachShader(m_fbxModelShader, fragmentShader);
	glBindAttribLocation(m_fbxModelShader, 0, "position");
	glBindAttribLocation(m_fbxModelShader, 1, "normal");
	glBindAttribLocation(m_fbxModelShader, 2, "vuv");
	glLinkProgram(m_fbxModelShader);
	CheckShaderError(m_fbxModelShader, GL_LINK_STATUS, true, "Error: program linking");
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void FBX_Stuff::CreateFBXOpenGLBuffers(FBXFile * fbx)
{
	// FBX Files contain multiple meshes, each with seperate material information
	//	// loop through each mesh within the FBX file and cretae VAO, VBO and IBO buffers for each mesh.
	//	// We can store that information within the mesh object via its "user data" void pointer variable.
		for (unsigned int i = 0; i < fbx->getMeshCount(); i++)
		{
			// get the current mesh from file
			FBXMeshNode *fbxMesh = fbx->getMeshByIndex(i);
			GLMesh *glData = new GLMesh();
			glGenVertexArrays(1, &glData->vao);
			glBindVertexArray(glData->vao);
			glGenBuffers(1, &glData->vbo);
			glGenBuffers(1, &glData->ibo);
			glBindBuffer(GL_ARRAY_BUFFER, glData->vbo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glData->ibo);
			// fill the vbo with our vertices.
			// the FBXLoader has convinently already defined a Vertex Structure for us.
			glBufferData(GL_ARRAY_BUFFER,
				fbxMesh->m_vertices.size() * sizeof(FBXVertex),
				fbxMesh->m_vertices.data(), GL_STATIC_DRAW);
			// fill the ibo with the indices.
			// fbx meshes can be large, so indices are stored as an unsigned int.
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				fbxMesh->m_indices.size() * sizeof(unsigned int),
				fbxMesh->m_indices.data(), GL_STATIC_DRAW);
			// Setup Vertex Attrib pointers
			// remember, we only need to setup the approprate attributes for the shaders that will be rendering
			// this fbx object.
			glEnableVertexAttribArray(0); //position
			glEnableVertexAttribArray(1); //normals
			glEnableVertexAttribArray(2); //tangents
			glEnableVertexAttribArray(3); //texcoords
			glEnableVertexAttribArray(4); //weights
			glEnableVertexAttribArray(5); //indices
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex),
				(void*)FBXVertex::PositionOffset);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex),
				(void*)FBXVertex::NormalOffset);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(FBXVertex),
				(void*)FBXVertex::TangentOffset);
			glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(FBXVertex),
				(void*)FBXVertex::TexCoord1Offset);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex),
				(void*)FBXVertex::WeightsOffset);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(FBXVertex),
				(void*)FBXVertex::IndicesOffset);
			// TODO: add any additional attribute pointers required for shader use.
			// unbind
			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			// attach our GLMesh object to the m_userData pointer.
			fbxMesh->m_userData = glData;
		}
}

void FBX_Stuff::CleanupFBXOpenGLBuffers(FBXFile * file)
{
	for (unsigned int i = 0; i < file->getMeshCount(); i++)
	{
		FBXMeshNode *fbxMesh = file->getMeshByIndex(i);
		GLMesh *glData = (GLMesh *)fbxMesh->m_userData;
		glDeleteVertexArrays(1, &glData->vao);
		glDeleteBuffers(1, &glData->vbo);
		glDeleteBuffers(1, &glData->ibo);
		delete glData;
	}
}

void FBX_Stuff::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string & errorMessage)
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

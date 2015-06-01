#include "PrecompiledHeader.h"
#include "ShaderProgram.h"
#include <vector>
#include "Model.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include "Camera.h"
#include <list>
#include <tuple>
#include "Light.h"
#include <random>
#include "Object.h"

class Renderer
{
public:
	Renderer();

	GLFWwindow* GetWindow() const { return m_Window; }

	ShaderProgram m_ShaderProgram;
	bool Initialize();
	void AddObjectToDraw(Object* object, bool visible);
	void AddLightToDraw();
	void AddLightToDraw(float Raidus, float SpecularExponent, glm::vec3 Position, glm::vec3 DiffuseColor, glm::vec3 SpecularColor);
	void Clear();

	bool InitializeShaders();
	void Draw(double dt);
	
	GLchar* m_glVendor;
	GLint m_glVersion[2];
	int WIDTH;
	int HEIGHT;
	bool m_VSync;
	std::shared_ptr<Camera> m_Camera;
	void DrawScene();
	void LoadContent();

	glm::vec3 GetCameraPosition();
	glm::vec3 GetCameraForward();

	void GeometryPass();
	void SSAOPass();
	void LightingPass();
	void FinalPass();

	void FrameBufferObjectInitialization();


	float Samples[16 * 3];

private:
	std::list<std::tuple<Model*, glm::mat4, bool>> m_ModelsToRender;
	GLFWwindow* m_Window;

	ShaderProgram m_GeometryProgram;
	ShaderProgram m_LightingProgram;
	ShaderProgram m_FinalProgram;
	ShaderProgram m_DrawQuadProgram;
	ShaderProgram m_SSAOProgram;
	ShaderProgram m_BlurHorizontalProgram;
	ShaderProgram m_BlurVerticalProgram;

	//FrameBuffers
	GLuint m_GeometryPass;
	GLuint m_SSAOPass;
	GLuint m_LightingPass;
	GLuint m_FinalPass;

	//Textures
	GLuint m_AmbientOcclusionTexture;
	GLuint m_SSAONormalTexture;
	GLuint m_NoiseTexture;
	GLuint m_DepthTexture;
	GLuint m_DiffuseTexture;
	GLuint m_PositionTexture;
	GLuint m_NormalTexture;
	GLuint m_LightingTexture;
	GLuint m_FinalDiffuseTexture;

	std::list<Light> m_Lights;

	enum RenderState 
	{ 
		FirstPassDiffuse, 
		FirstPassNormal,
		FirstPassPosition,
		SecondPassLight,
		SSAO,
		SSAOBLUR,
		FINAL,
	}m_RenderState;

	GLuint CreateQuad();
	GLuint m_Quad;


	struct SSAOVariables
	{
		SSAOVariables()
		{ 
			TotalStrength = 0.2f;
			Strength = 0.07f;
			Offset = 1.0f;
			Falloff = 0.000000001f;
			Radius = 0.01f;
		};
		float TotalStrength;
		float Strength;
		float Offset;
		float Falloff;
		float Radius;
	}m_SSAO;
};


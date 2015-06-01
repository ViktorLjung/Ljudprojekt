#include "Renderer.h"

Model m_SphereModel = Model("Assets/Models/Sphere.obj");

Renderer::Renderer()
{
	Initialize();
	m_VSync = false;
	m_RenderState = FINAL;
	m_NoiseTexture = 0;
}

void Renderer::AddObjectToDraw(Object* object, bool visible)
{
	m_ModelsToRender.push_back(std::make_tuple(object->m_Model.get(), object->ModelMatrix(), visible));
}

bool Renderer::Initialize()
{
	// Initialize GLFW
	if (!glfwInit()) 
	{
		LOG_ERROR("GLFW: Initialization failed");
		exit(EXIT_FAILURE);
	}

	// Create a window
	WIDTH = 1280;
	HEIGHT = 720;
	glfwWindowHint(GLFW_SAMPLES, 16);
	m_Window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr);
	if (!m_Window) 
	{
		LOG_ERROR("GLFW: Failed to create window");
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(m_Window);

	// GL version info
	glGetIntegerv(GL_MAJOR_VERSION, &m_glVersion[0]);
	glGetIntegerv(GL_MINOR_VERSION, &m_glVersion[1]);
	m_glVendor = (GLchar*)glGetString(GL_VENDOR);
	std::stringstream ss;
	ss << m_glVendor << " OpenGL " << m_glVersion[0] << "." << m_glVersion[1];
#ifdef DEBUG
	ss << " DEBUG";
#endif
	LOG_INFO(ss.str().c_str());
	glfwSetWindowTitle(m_Window, ss.str().c_str());

	// Initialize GLEW
	if (glewInit() != GLEW_OK) 
	{
		LOG_ERROR("GLEW: Initialization failed");
		exit(EXIT_FAILURE);
	}

	
	

	glfwSwapInterval(m_VSync);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	
}

void Renderer::LoadContent()
{
	// Create Camera
	m_Camera = std::make_shared<Camera>(45.f, (float)WIDTH / HEIGHT, 0.02f, 10000.f);
	m_Camera->Position(glm::vec3(5.f, 5.f, 5.f));
	m_Camera->Pitch(glm::radians(35.f));
	m_Camera->Yaw(glm::radians(-45.f));
	m_Camera->MoveSpeed(2.5f);

	InitializeShaders();
	FrameBufferObjectInitialization();
	

	m_NoiseTexture = SOIL_load_OGL_texture("Assets/Textures/ssao_random_normals.png", SOIL_FLAG_TEXTURE_REPEATS, 0, SOIL_FLAG_INVERT_Y);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	m_Quad = CreateQuad();

	Samples[0] = 0.53812504; Samples[1] = 0.18565957; Samples[2] = -0.43192;
	Samples[3] = 0.13790712; Samples[4] = 0.24864247; Samples[5] = 0.44301823;
	Samples[6] = 0.33715037; Samples[7] = 0.56794053; Samples[8] = -0.005789503;
	Samples[9] = -0.6999805; Samples[10] = -0.04511441; Samples[11] = -0.0019965635;
	Samples[12] = 0.06896307; Samples[13] = -0.15983082; Samples[14] = -0.85477847;
	Samples[15] = 0.056099437; Samples[16] = 0.006954967; Samples[17] = -0.1843352;
	Samples[18] = -0.014653638; Samples[19] = 0.14027752; Samples[20] = 0.0762037;
	Samples[21] = 0.010019933; Samples[22] = -0.1924225; Samples[23] = -0.034443386;
	Samples[24] = 0.53812504; Samples[25] = 0.18565957; Samples[26] = -0.43192;
	Samples[27] = 0.13790712; Samples[28] = 0.24864247; Samples[29] = 0.44301823;
	Samples[30] = 0.33715037; Samples[31] = 0.56794053; Samples[32] = -0.005789503;
	Samples[33] = -0.6999805; Samples[34] = -0.04511441; Samples[35] = -0.0019965635;
	Samples[36] = 0.06896307; Samples[37] = -0.15983082; Samples[38] = -0.85477847;
	Samples[39] = 0.056099437; Samples[40] = 0.006954967; Samples[41] = -0.1843352;
	Samples[42] = -0.014653638; Samples[43] = 0.14027752; Samples[44] = 0.0762037;
	Samples[45] = 0.010019933; Samples[46] = -0.1924225; Samples[47] = -0.034443386;



}

bool Renderer::InitializeShaders()
{
	m_ShaderProgram.AddShader(std::shared_ptr<Shader>(new VertexShader("Shaders/vertex.glsl")));
	m_ShaderProgram.AddShader(std::shared_ptr<Shader>(new FragmentShader("Shaders/fragment.glsl")));
	m_ShaderProgram.Compile();
	m_ShaderProgram.Link();

	m_GeometryProgram.AddShader(std::shared_ptr<Shader>(new VertexShader("Shaders/GeometryPass.vert.glsl")));
	m_GeometryProgram.AddShader(std::shared_ptr<Shader>(new FragmentShader("Shaders/GeometryPass.frag.glsl")));
	m_GeometryProgram.Compile();
	glBindFragDataLocation(m_GeometryProgram.GetHandle(), 0, "frag_Diffuse");
	glBindFragDataLocation(m_GeometryProgram.GetHandle(), 1, "frag_Position");
	glBindFragDataLocation(m_GeometryProgram.GetHandle(), 2, "frag_Normal");
	glBindFragDataLocation(m_GeometryProgram.GetHandle(), 3, "frag_SSAONormal");
	m_GeometryProgram.Link();

	m_LightingProgram.AddShader(std::shared_ptr<Shader>(new VertexShader("Shaders/LightingPass.vert.glsl")));
	m_LightingProgram.AddShader(std::shared_ptr<Shader>(new FragmentShader("Shaders/LightingPass.frag.glsl")));
	m_LightingProgram.Compile();
	glBindFragDataLocation(m_LightingProgram.GetHandle(), 0, "frag_Light");
	m_LightingProgram.Link();

	m_FinalProgram.AddShader(std::shared_ptr<Shader>(new VertexShader("Shaders/FinalPass.vert.glsl")));
	m_FinalProgram.AddShader(std::shared_ptr<Shader>(new FragmentShader("Shaders/FinalPass.frag.glsl")));
	m_FinalProgram.Compile();
	glBindFragDataLocation(m_FinalProgram.GetHandle(), 0, "frag_Final");
	m_FinalProgram.Link();

	m_DrawQuadProgram.AddShader(std::shared_ptr<Shader>(new VertexShader("Shaders/DrawQuad.vert.glsl")));
	m_DrawQuadProgram.AddShader(std::shared_ptr<Shader>(new FragmentShader("Shaders/DrawQuad.frag.glsl")));
	m_DrawQuadProgram.Compile();
	m_DrawQuadProgram.Link();


	m_SSAOProgram.AddShader(std::shared_ptr<Shader>(new VertexShader("Shaders/SSAO.vert.glsl")));
	m_SSAOProgram.AddShader(std::shared_ptr<Shader>(new FragmentShader("Shaders/SSAO.frag.glsl")));
	m_SSAOProgram.Compile();
	glBindFragDataLocation(m_SSAOProgram.GetHandle(), 0, "frag_SSAO");
	m_SSAOProgram.Link();


	m_BlurHorizontalProgram.AddShader(std::shared_ptr<Shader>(new VertexShader("Shaders/BlurHorizontal.vert.glsl")));
	m_BlurHorizontalProgram.AddShader(std::shared_ptr<Shader>(new FragmentShader("Shaders/BlurHorizontal.frag.glsl")));
	m_BlurHorizontalProgram.Compile();
	glBindFragDataLocation(m_BlurHorizontalProgram.GetHandle(), 0, "fResult");
	m_BlurHorizontalProgram.Link();

	m_BlurVerticalProgram.AddShader(std::shared_ptr<Shader>(new VertexShader("Shaders/BlurVertical.vert.glsl")));
	m_BlurVerticalProgram.AddShader(std::shared_ptr<Shader>(new FragmentShader("Shaders/BlurVertical.frag.glsl")));
	m_BlurVerticalProgram.Compile();
	glBindFragDataLocation(m_BlurVerticalProgram.GetHandle(), 0, "fResult");
	m_BlurVerticalProgram.Link();

	return true;
}

void Renderer::Clear()
{
	m_ModelsToRender.clear();
}


void Renderer::Draw(double dt)
{
	m_Camera->Input(m_Window, dt, WIDTH, HEIGHT);

	if (glfwGetKey(m_Window, GLFW_KEY_1) == GLFW_PRESS)
	{
		m_RenderState = FirstPassDiffuse;
	}
	else if (glfwGetKey(m_Window, GLFW_KEY_2) == GLFW_PRESS)
	{
		m_RenderState = FirstPassNormal;
	}
	else if (glfwGetKey(m_Window, GLFW_KEY_3) == GLFW_PRESS)
	{
		m_RenderState = FirstPassPosition;
	}
	else if (glfwGetKey(m_Window, GLFW_KEY_4) == GLFW_PRESS)
	{
		m_RenderState = SecondPassLight;
	}
	else if (glfwGetKey(m_Window, GLFW_KEY_5) == GLFW_PRESS)
	{
		m_RenderState = SSAO;
	}
	else if (glfwGetKey(m_Window, GLFW_KEY_6) == GLFW_PRESS)
	{
		m_RenderState = FINAL;
	}
	else if (glfwGetKey(m_Window, GLFW_KEY_7) == GLFW_PRESS)
	{
		m_RenderState = SSAOBLUR;
	}

	else if (glfwGetKey(m_Window, GLFW_KEY_F1) == GLFW_PRESS)
	{
		if (glfwGetKey(m_Window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			m_SSAO.TotalStrength -= 1.f * dt;
		}
		else
		{
			m_SSAO.TotalStrength += 1.f * dt;
		}

		LOG_INFO("TotalStrength: %f", m_SSAO.TotalStrength);
	}
	else if (glfwGetKey(m_Window, GLFW_KEY_F2) == GLFW_PRESS)
	{
		if (glfwGetKey(m_Window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			m_SSAO.Strength -= 1.f * dt;
		}
		else
		{
			m_SSAO.Strength += 1.f * dt;
		}
		LOG_INFO("Strength: %f", m_SSAO.Strength);
	}
	else if (glfwGetKey(m_Window, GLFW_KEY_F3) == GLFW_PRESS)
	{
		if (glfwGetKey(m_Window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			m_SSAO.Offset -= 0.05f * dt;
		}
		else
		{
			m_SSAO.Offset += 0.05f * dt;
		}
		LOG_INFO("Offset: %f", m_SSAO.Offset);
	}
	else if (glfwGetKey(m_Window, GLFW_KEY_F4) == GLFW_PRESS)
	{
		if (glfwGetKey(m_Window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			m_SSAO.Falloff -= 0.0001f * dt;
		}
		else
		{
			m_SSAO.Falloff += 0.0001f * dt;
		}
		LOG_INFO("Falloff: %f", m_SSAO.Falloff);
	}
	else if (glfwGetKey(m_Window, GLFW_KEY_F5) == GLFW_PRESS)
	{
		if (glfwGetKey(m_Window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			m_SSAO.Radius -= 0.01f * dt;
		}
		else
		{
			m_SSAO.Radius += 0.01f * dt;
		}
		LOG_INFO("Radius: %f", m_SSAO.Radius);
	}

	

	DrawScene();


	glfwSwapBuffers(m_Window);
}


void Renderer::DrawScene()
{

	if (m_RenderState == FINAL || m_RenderState == FirstPassDiffuse || m_RenderState == FirstPassNormal || m_RenderState == FirstPassPosition || m_RenderState == SSAO || m_RenderState == SecondPassLight || m_RenderState == SSAOBLUR)
		GeometryPass();
	if (m_RenderState == FINAL || m_RenderState == SSAO || m_RenderState == SSAOBLUR)
		SSAOPass();
	if (m_RenderState == FINAL || m_RenderState == SecondPassLight)
		LightingPass();
	if (m_RenderState == FINAL)
		FinalPass();
}


void Renderer::GeometryPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_GeometryPass);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	

	for (auto tuple : m_ModelsToRender)
	{
		Model* model;
		glm::mat4 modelMatrix;
		bool visable;
		std::tie(model, modelMatrix, visable) = tuple;
		if (!visable)
			continue;

		m_GeometryProgram.Bind();
		glUniformMatrix4fv(glGetUniformLocation(m_GeometryProgram.GetHandle(), "M"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(glGetUniformLocation(m_GeometryProgram.GetHandle(), "V"), 1, GL_FALSE, glm::value_ptr(m_Camera->viewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(m_GeometryProgram.GetHandle(), "P"), 1, GL_FALSE, glm::value_ptr(m_Camera->projectionMatrix()));

		model->Bind();
		glDrawArrays(GL_TRIANGLES, 0, model->NumberOfVertices());
		
	}

	if (m_RenderState == FirstPassDiffuse)
	{	
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WIDTH, HEIGHT);
		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		m_DrawQuadProgram.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_DiffuseTexture);
		glBindVertexArray(m_Quad);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	else if (m_RenderState == FirstPassNormal)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WIDTH, HEIGHT);
		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		m_DrawQuadProgram.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_SSAONormalTexture);
		glBindVertexArray(m_Quad);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	else if (m_RenderState == FirstPassPosition)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WIDTH, HEIGHT);
		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		m_DrawQuadProgram.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_PositionTexture);
		glBindVertexArray(m_Quad);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

}

GLuint Renderer::CreateQuad()
{

	float quadVertices[] =
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,

		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};
	float quadTexCoords[] =
	{
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
	};
	GLuint vbo[2], vao;
	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(float), quadVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 2 * 6 * sizeof(float), quadTexCoords, GL_STATIC_DRAW);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return vao;

}



void Renderer::LightingPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_LightingPass);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);

	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	m_LightingProgram.Bind();
	GLuint shaderHandle = m_LightingProgram.GetHandle();

	for (auto l : m_Lights)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_PositionTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_NormalTexture);

		glUniformMatrix4fv(glGetUniformLocation(shaderHandle, "M"), 1, GL_FALSE, glm::value_ptr(l.ModelMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(shaderHandle, "V"), 1, GL_FALSE, glm::value_ptr(m_Camera->viewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(shaderHandle, "P"), 1, GL_FALSE, glm::value_ptr(m_Camera->projectionMatrix()));
		glUniform2fv(glGetUniformLocation(shaderHandle, "ViewportSize"), 1, glm::value_ptr(glm::vec2(WIDTH, HEIGHT)));
		glUniform3fv(glGetUniformLocation(shaderHandle, "ls"), 1, glm::value_ptr(l.SpecularColor()));
		glUniform3fv(glGetUniformLocation(shaderHandle, "ld"), 1, glm::value_ptr(l.DiffuseColor()));
		glUniform3fv(glGetUniformLocation(shaderHandle, "lp"), 1, glm::value_ptr(l.Position()));
		glUniform1f(glGetUniformLocation(shaderHandle, "specularExponent"), l.SpecularExponent());
		glUniform3fv(glGetUniformLocation(shaderHandle, "CameraPosition"), 1, glm::value_ptr(m_Camera->Position()));
		glUniform1f(glGetUniformLocation(shaderHandle, "LightRadius"), l.Radius());

		
		glBindVertexArray(m_SphereModel.m_VertexArrayObject);

		glDrawArrays(GL_TRIANGLES, 0, m_SphereModel.NumberOfVertices());
	}



	if (m_RenderState == SecondPassLight)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WIDTH, HEIGHT);
		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		m_DrawQuadProgram.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_LightingTexture);
		glBindVertexArray(m_Quad);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}


void Renderer::SSAOPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_SSAOPass);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glViewport(0, 0, WIDTH, HEIGHT);

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	m_SSAOProgram.Bind();
	GLuint shaderHandle = m_SSAOProgram.GetHandle();



	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_SSAONormalTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_NoiseTexture);
	

	glUniform3fv(glGetUniformLocation(shaderHandle, "Sample"), 16, Samples);
	glUniform1f(glGetUniformLocation(shaderHandle, "TotalStrength"), m_SSAO.TotalStrength);
	glUniform1f(glGetUniformLocation(shaderHandle, "Strength"), m_SSAO.Strength);
	glUniform1f(glGetUniformLocation(shaderHandle, "Offset"), m_SSAO.Offset);
	glUniform1f(glGetUniformLocation(shaderHandle, "Falloff"), m_SSAO.Falloff);
	glUniform1f(glGetUniformLocation(shaderHandle, "Radius"), m_SSAO.Radius);

	glBindVertexArray(m_Quad);
	glDrawArrays(GL_TRIANGLES, 0, 6);



	if (m_RenderState == SSAO)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WIDTH, HEIGHT);

		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		m_DrawQuadProgram.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_AmbientOcclusionTexture);

		glBindVertexArray(m_Quad);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	if (m_RenderState == FINAL || m_RenderState == SSAOBLUR)
	{
		//Blur Horizontal
		glBindFramebuffer(GL_FRAMEBUFFER, m_SSAOPass);
		glViewport(0, 0, WIDTH, HEIGHT);

		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		m_BlurHorizontalProgram.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_AmbientOcclusionTexture);

		glBindVertexArray(m_Quad);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		//Blur Vertical
		glBindFramebuffer(GL_FRAMEBUFFER, m_SSAOPass);
		glViewport(0, 0, WIDTH, HEIGHT);

		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		m_BlurVerticalProgram.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_AmbientOcclusionTexture);

		glBindVertexArray(m_Quad);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	if (m_RenderState == SSAOBLUR)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WIDTH, HEIGHT);

		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		m_DrawQuadProgram.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_AmbientOcclusionTexture);

		glBindVertexArray(m_Quad);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	
}


void Renderer::FinalPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FinalPass);

	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	m_FinalProgram.Bind();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_DiffuseTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_LightingTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_AmbientOcclusionTexture);

	glBindVertexArray(m_Quad);
	glDrawArrays(GL_TRIANGLES, 0, 6);


	if (m_RenderState == FINAL)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, WIDTH, HEIGHT);

		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		m_DrawQuadProgram.Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_FinalDiffuseTexture);

		glBindVertexArray(m_Quad);
		glEnableVertexAttribArray(0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}

void Renderer::FrameBufferObjectInitialization()
{
	m_GeometryPass = 0;
	glGenFramebuffers(1, &m_GeometryPass);

	//Generate and bind diffuse texture
	glGenTextures(1, &m_DiffuseTexture);
	glBindTexture(GL_TEXTURE_2D, m_DiffuseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	//Generate and bind position texture
	glGenTextures(1, &m_PositionTexture);
	glBindTexture(GL_TEXTURE_2D, m_PositionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Generate and bind normal texture
	glGenTextures(1, &m_NormalTexture);
	glBindTexture(GL_TEXTURE_2D, m_NormalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Generate and bind SSAOnormal texture
	glGenTextures(1, &m_SSAONormalTexture);
	glBindTexture(GL_TEXTURE_2D, m_SSAONormalTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenTextures(1, &m_DepthTexture);
	glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WIDTH, HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	
	//Bind FramebufferObject
	glBindFramebuffer(GL_FRAMEBUFFER, m_GeometryPass);
	
	//Attach textures to the FramebufferObject
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_DiffuseTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_PositionTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_NormalTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_SSAONormalTexture, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);

	GLenum GeometryPassAttachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, GeometryPassAttachments);

	GLenum fbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fbStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_ERROR("DeferredLighting:Init: m_fbBasePass incomplete: 0x%x\n", fbStatus);
		//exit(1);
	}
	

	//Lighting pass
	m_LightingPass = 0;
	glGenFramebuffers(1, &m_LightingPass);

	glGenTextures(1, &m_LightingTexture);
	glBindTexture(GL_TEXTURE_2D, m_LightingTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glBindFramebuffer(GL_FRAMEBUFFER, m_LightingPass);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_LightingTexture, 0);
	
	GLenum lightingPassAttachments[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, lightingPassAttachments);

	fbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fbStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_ERROR("DeferredLighting:Init: m_LightingPass incomplete: 0x%x\n", fbStatus);
		//exit(1);
	}

	//Final pass
	m_FinalPass = 0;
	glGenFramebuffers(1, &m_FinalPass);
	glGenTextures(1, &m_FinalDiffuseTexture);
	glBindTexture(GL_TEXTURE_2D, m_FinalDiffuseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, m_FinalPass);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FinalDiffuseTexture, 0);

	GLenum FinalPassAttachments[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, FinalPassAttachments);

	fbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fbStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_ERROR("DeferredLighting:Init: m_FinalPass incomplete: 0x%x\n", fbStatus);
		//exit(1);
	}


	//SSAO pass
	m_SSAOPass = 0;
	glGenFramebuffers(1, &m_SSAOPass);
	
	glGenTextures(1, &m_AmbientOcclusionTexture);
	glBindTexture(GL_TEXTURE_2D, m_AmbientOcclusionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, m_SSAOPass);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_AmbientOcclusionTexture, 0);
	

	GLenum SSAOPasssAttachments[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, SSAOPasssAttachments);

	fbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fbStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_ERROR("DeferredLighting:Init: m_SSAOPass incomplete: 0x%x\n", fbStatus);
		//exit(1);
	}
}

void Renderer::AddLightToDraw(float Raidus, float SpecularExponent, glm::vec3 Position, glm::vec3 DiffuseColor, glm::vec3 SpecularColor)
{
	Light l = Light(Raidus, SpecularExponent, Position, DiffuseColor, SpecularColor);
	m_Lights.push_back(l);
}

void Renderer::AddLightToDraw()
{
	Light l = Light();
	m_Lights.push_back(l);
}

glm::vec3 Renderer::GetCameraPosition()
{
	return m_Camera->Position();
}

glm::vec3 Renderer::GetCameraForward()
{
	return m_Camera->Forward();
}

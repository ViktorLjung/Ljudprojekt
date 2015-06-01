#include "PrecompiledHeader.h"

#include "Rectangle.h"
#include"Camera.h"
#include "Model.h"
#include "Renderer.h"
#include "PhysicsSystem.h"
#include "SFX.h"
#include "SoundSystem.h"
#include "ObjectManager.h"


Renderer renderer = Renderer(); // First or break >:(
ObjectManager objectManager = ObjectManager();

static PhysicsSystem physicsSystem;
//SoundSystem ss;
SFX testljud;


void Update(double dt);

int main()
{
	renderer.LoadContent();
	physicsSystem.Initialize();
	
	objectManager.Objects.front()->m_RigidBody = physicsSystem.AddRigidBody(objectManager.Objects.front()->m_Shape, objectManager.Objects.front()->m_Mass, objectManager.Objects.front()->m_Model->ModelMatrix());

	renderer.AddModelToDraw(objectManager.Objects.front()->m_Model, true);
	
	renderer.AddLightToDraw();

	double lastTime = glfwGetTime();
	glfwSetCursorPos(renderer.GetWindow(), renderer.WIDTH / 2, renderer.HEIGHT / 2);

	while (!glfwWindowShouldClose(renderer.GetWindow()))// MAIN LOOP
	{
		double time = glfwGetTime();
		double dt = time - lastTime;
		lastTime = time;

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//  Adams sound tutorial :D
		//////////////////////////////////////////////////////////////////////////////////////////////////
		// Skapa ett sfx eller bgm, skillnaden �r att sfx kan spelas i 3D och att bgm loopar per default.
		// F�rst m�ste du skapa ett ljud. Parametrarna �r filnamn och volym mellan 0-1.
		// Sen �r det fritt fram att spela det :D bgm beh�ver ingen pos f�r att spelas ut
		//////////////////////////////////////////////////////////////////////////////////////////////////
		testljud.CreateSound("Assets/Sounds/test.wav");
		testljud.PlaySound(glm::vec3(0), 0.1f);

		objectManager.Update(dt);
		Update(dt);
		physicsSystem.Update(dt);
		renderer.Draw(dt);
		glfwPollEvents();
	}

	physicsSystem.CleanUp();
	glfwDestroyWindow(renderer.GetWindow());
	return EXIT_SUCCESS;
}

void Update(double dt)
{
	if (glfwGetKey(renderer.GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		exit(EXIT_SUCCESS);
	}
}


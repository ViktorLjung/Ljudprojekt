#include "PrecompiledHeader.h"

#include "Rectangle.h"
#include "Camera.h"
#include "Model.h"
#include "Renderer.h"
#include "PhysicsSystem.h"
#include "SFX.h"
#include "SoundSystem.h"
#include "ObjectManager.h"


Renderer renderer = Renderer();
ObjectManager objectManager = ObjectManager();

//std::shared_ptr<Model> model;
//std::shared_ptr<Model> Plane;

PhysicsSystem physicsSystem;

//SoundSystem ss;
SFX testljud;


void Update(double dt);

int main()
{
	renderer.LoadContent();
	physicsSystem.Initialize();
	
	//model = std::make_shared<Model>("Assets/Models/SSAOTest3.obj");
	//Plane = std::make_shared<Model>("Assets/Models/Raptor.obj", glm::vec3(3, 0, -3), glm::quat(), glm::vec3(1, 1, 1));
	renderer.AddModelToDraw(objectManager.Objects.front()->m_Model, true);
	//renderer.AddModelToDraw(Plane, true);
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
		// Skapa ett sfx eller bgm, skillnaden är att sfx kan spelas i 3D och att bgm loopar per default.
		// Först måste du skapa ett ljud. Parametrarna är filnamn och volym mellan 0-1.
		// Sen är det fritt fram att spela det :D bgm behöver ingen pos för att spelas ut
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


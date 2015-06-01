#include "PrecompiledHeader.h"

#include "Rectangle.h"
#include "Camera.h"
#include "Model.h"
#include "Renderer.h"
#include "PhysicsSystem.h"
#include "ObjectManager.h"

#include "SoundSystem.h"
#include "SFX.h"
#include "BGM.h"


Renderer renderer = Renderer(); // First or break >:(
ObjectManager objectManager = ObjectManager();

static PhysicsSystem physicsSystem;
//SoundSystem ss;
//SFX testljud;
BGM bgmHisako("Assets/Sounds/hisako.wav");
int once = 0;


void Update(double dt);

int main()
{
	renderer.LoadContent();
	physicsSystem.Initialize();
	
	btCollisionShape* shape = new btBoxShape(btBoxShape(btVector3(0.5f, 0.5f, 0.5f)));

 	objectManager.AddObject(new Object(shape, "Assets/Models/Cube.obj", 1, 0.1, glm::vec3(0, 1, 0), glm::quat()));
 	objectManager.AddObject(new Object(shape, "Assets/Models/Cube.obj", 1, 0.1, glm::vec3(0.75, 5, 0), glm::quat()));


	btCollisionShape* groundshape = new btBoxShape(btBoxShape(btVector3(75.f, 0.5f, 75.f)));
	objectManager.AddObject(new Object(groundshape, "Assets/Models/Plane.obj", 0, 0.5f, glm::vec3(0, -5, 0), glm::quat()));
	
	

	std::shared_ptr<Model>  model; 
	model = std::make_shared<Model>("Assets/Models/Sphere.obj");

	btCollisionShape* sphere = new btSphereShape(0.5f);

	for (int i = 0; i < 10; i++)
	{
		objectManager.AddObject(new Object(sphere, model, 1, 1.1f, glm::vec3(1.25f*i, 10, 0), glm::quat()));
	}


	for (Object* o : objectManager.GetObjects())
	{
		o->m_RigidBody = physicsSystem.AddRigidBody(o->m_Shape, o->m_Mass, o->m_Restitution, o->ModelMatrix());
		
	}
	
	renderer.AddLightToDraw();

	double lastTime = glfwGetTime();
	glfwSetCursorPos(renderer.GetWindow(), renderer.WIDTH / 2, renderer.HEIGHT / 2);

	while (!glfwWindowShouldClose(renderer.GetWindow()))// MAIN LOOP
	{
		double time = glfwGetTime();
		double dt = time - lastTime;
		lastTime = time;


		for (Object* o : objectManager.GetObjects())
		{
			renderer.AddObjectToDraw(o, true);
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//  Adams sound tutorial :D
		//////////////////////////////////////////////////////////////////////////////////////////////////
		// Skapa ett sfx eller bgm, skillnaden är att sfx kan spelas i 3D och att bgm loopar per default.
		// Först måste du skapa ett ljud. Parametrarna är filnamn och volym mellan 0-1.
		// Sen är det fritt fram att spela det :D bgm behöver ingen pos för att spelas ut
		//////////////////////////////////////////////////////////////////////////////////////////////////
		//testljud.CreateSound("Assets/Sounds/test.wav");
		//testljud.PlaySound(glm::vec3(0), 0.1f);
		//bgmHisako.CreateSound("Assets/Sounds/hisako.wav");
		if (once == 0)
		{
			//bgmHisako.PlaySound(0);
			once++;
		}

		objectManager.Update(dt);
		Update(dt);
		physicsSystem.Update(dt);
		physicsSystem.CheckCollisions();
		renderer.Draw(dt);
		renderer.Clear();
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


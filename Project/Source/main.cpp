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
#include <vector>

Renderer renderer = Renderer(); // First or break >:(
ObjectManager objectManager = ObjectManager();

static PhysicsSystem physicsSystem;
SoundSystem ss = SoundSystem();

std::vector<SFX> Sounds;

int once = 0;
std::list<std::tuple<btRigidBody*, btRigidBody*, btManifoldPoint>> collisions;
void Update(double dt);
void Input();

std::vector<int> previousInput;
std::vector<int> currentInput;

int main()
{
// 	Sounds.push_back(SFX("Assets/Sounds/Guitar/C.wav"));
// 	Sounds.push_back(SFX("Assets/Sounds/Guitar/D.wav"));
// 	Sounds.push_back(SFX("Assets/Sounds/Guitar/E.wav"));
// 	Sounds.push_back(SFX("Assets/Sounds/Guitar/F.wav"));
// 	Sounds.push_back(SFX("Assets/Sounds/Guitar/G.wav"));
// 	Sounds.push_back(SFX("Assets/Sounds/Guitar/ALow.wav"));
// 	Sounds.push_back(SFX("Assets/Sounds/Guitar/BLow.wav"));


	renderer.LoadContent();
	physicsSystem.Initialize();
	
	btCollisionShape* shape = new btBoxShape(btBoxShape(btVector3(0.5f, 0.5f, 0.5f)));

 	//objectManager.AddObject(new Object(shape, "Assets/Models/Cube.obj", 1, 0.1, glm::vec3(0, 1, 0), glm::quat()));
 	//objectManager.AddObject(new Object(shape, "Assets/Models/Cube.obj", 1, 0.1, glm::vec3(0.75, 5, 0), glm::quat()));
	
	btCollisionShape* groundshape = new btBoxShape(btBoxShape(btVector3(75.f, 0.5f, 75.f)));
	objectManager.AddObject(new Object(groundshape, "Assets/Models/Plane.obj", 0, 1.f, glm::vec3(0, -5, 0), glm::quat()));

	/*{
		glm::quat rot = glm::quat();
		rot = rot * glm::angleAxis(90 * 3.14f / 180, glm::vec3(0, 0, -1));
		objectManager.AddObject(new Object(groundshape, "Assets/Models/Plane.obj", 0, 1.f, glm::vec3(-75, -5, 0), rot));
	}
	{
		glm::quat rot = glm::quat();
		rot = rot * glm::angleAxis(90 * 3.14f / 180, glm::vec3(0, 0, 1));
		objectManager.AddObject(new Object(groundshape, "Assets/Models/Plane.obj", 0, 1.f, glm::vec3(75, -5, 0), rot));
	}
	{
		glm::quat rot = glm::quat();
		rot = rot * glm::angleAxis(90 * 3.14f / 180, glm::vec3(-1, 0, 0));
		objectManager.AddObject(new Object(groundshape, "Assets/Models/Plane.obj", 0, 1.f, glm::vec3(0, -5, 75), rot));
	}
	{
		glm::quat rot = glm::quat();
		rot = rot * glm::angleAxis(90 * 3.14f / 180, glm::vec3(1, 0, 0));
		objectManager.AddObject(new Object(groundshape, "Assets/Models/Plane.obj", 0, 1.f, glm::vec3(0, -5, -75), rot));
	}*/


	

	std::shared_ptr<Model>  model; 
	model = std::make_shared<Model>("Assets/Models/Sphere.obj");

	btCollisionShape* sphere = new btSphereShape(0.5f);

	for (int i = 0; i < 1; i++)
	{
		objectManager.AddObject(new Object(sphere, model, 0, 1.1f, glm::vec3(1.25f*i, 10, 0), glm::quat()));
	}


	for (Object* o : objectManager.GetObjects())
	{
		o->m_RigidBody = physicsSystem.AddRigidBody(o->m_Shape, o->m_Mass, o->m_Restitution, o->ModelMatrix());
		
	}
	
	renderer.AddLightToDraw();

	double lastTime = glfwGetTime();
	glfwSetCursorPos(renderer.GetWindow(), renderer.WIDTH / 2, renderer.HEIGHT / 2);

	int m_LastKeyPress = glfwGetMouseButton(renderer.GetWindow(), GLFW_MOUSE_BUTTON_RIGHT);
	while (!glfwWindowShouldClose(renderer.GetWindow()))// MAIN LOOP
	{
		double time = glfwGetTime();
		double dt = time - lastTime;
		lastTime = time;

		

		if (glfwGetMouseButton(renderer.GetWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE
			&& m_LastKeyPress == GLFW_PRESS)
		{
			Object* ob = new Object(sphere, model, 1, 0.5f, renderer.GetCameraPosition() + renderer.GetCameraForward(), glm::quat());
			objectManager.AddObject(ob);
			ob->m_RigidBody = physicsSystem.AddRigidBody(ob->m_Shape, ob->m_Mass, ob->m_Restitution, ob->ModelMatrix());
			glm::vec3 force = renderer.GetCameraForward();
			force *= 20;
			ob->m_RigidBody->applyImpulse(btVector3(force.x, force.y, force.z), btVector3(0, 0, 0));
		}


		m_LastKeyPress = glfwGetMouseButton(renderer.GetWindow(), GLFW_MOUSE_BUTTON_RIGHT);


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
		
		



		
		objectManager.Update(dt);
		Input();
		Update(dt);
		ss.Update(
			renderer.m_Camera->Position(),
			renderer.m_Camera->Forward(),
			renderer.m_Camera->Up());


		physicsSystem.Update(dt);
		collisions = physicsSystem.CheckCollisions();

		for (auto tup : collisions)
		{
			btRigidBody* body1;
			btRigidBody* body2;
			btManifoldPoint mani;
			std::tie(body1, body2, mani) = collisions.back();
			
			for (Object* o : objectManager.GetObjects())
			{
				if (o->m_RigidBody == body1 && mani.getAppliedImpulse() > 0)
				{
					printf("body %d, impulse %f\n", o->m_RigidBody == body1, mani.getAppliedImpulse());
					o->Boop(mani.getAppliedImpulse());
				}
				if(o->m_RigidBody == body2 && mani.getAppliedImpulse() > 0)
				{
					printf("body %d, impulse %f\n", o->m_RigidBody == body1, mani.getAppliedImpulse());
					o->Boop(mani.getAppliedImpulse());
				}
			}
		}
		collisions.clear();


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



void Input()
{

	currentInput.push_back(glfwGetKey(renderer.GetWindow(), GLFW_KEY_1));
	currentInput.push_back(glfwGetKey(renderer.GetWindow(), GLFW_KEY_2));
	currentInput.push_back(glfwGetKey(renderer.GetWindow(), GLFW_KEY_3));
	currentInput.push_back(glfwGetKey(renderer.GetWindow(), GLFW_KEY_4));
	currentInput.push_back(glfwGetKey(renderer.GetWindow(), GLFW_KEY_5));
	currentInput.push_back(glfwGetKey(renderer.GetWindow(), GLFW_KEY_6));
	currentInput.push_back(glfwGetKey(renderer.GetWindow(), GLFW_KEY_7));



	//Beats
	if (currentInput[0] == GLFW_PRESS && currentInput[0] != previousInput[0])
	{
		//Sounds[0].PlaySound(glm::vec3(), 0.2f);
		ss.PlaySFX("Assets/Sounds/bazinga.mp3", 1, glm::vec3(0));

	}
	if (currentInput[1] == GLFW_PRESS && currentInput[1] != previousInput[1])
	{
		Sounds[1].PlaySound(glm::vec3(), 0.2f);
	}
	if (currentInput[2] == GLFW_PRESS && currentInput[2] != previousInput[2])
	{
		Sounds[2].PlaySound(glm::vec3(), 0.2f);
	}
	if (currentInput[3] == GLFW_PRESS && currentInput[3] != previousInput[3])
	{
		Sounds[3].PlaySound(glm::vec3(), 0.2f);
	}
	if (currentInput[4] == GLFW_PRESS && currentInput[4] != previousInput[4])
	{
		Sounds[4].PlaySound(glm::vec3(), 0.2f);
	}
	if (currentInput[5] == GLFW_PRESS && currentInput[5] != previousInput[5])
	{
		Sounds[5].PlaySound(glm::vec3(), 0.2f);
	}
	if (currentInput[6] == GLFW_PRESS && currentInput[6] != previousInput[6])
	{
		Sounds[6].PlaySound(glm::vec3(), 0.2f);
	}

	previousInput = currentInput;
	currentInput.clear();
}
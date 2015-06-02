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

std::vector<char*> Sounds;

std::vector<std::shared_ptr<Model>> models;

float bgmVolume = 0.1;

std::list<std::tuple<btRigidBody*, btRigidBody*, btManifoldPoint>> collisions;
void Update(double dt);
void Input();
std::vector<int> previousInput;
std::vector<int> currentInput;
int m_LastKeyPress;
int main()
{

	Sounds.push_back("Assets/Sounds/Guitar/C.wav");
	Sounds.push_back("Assets/Sounds/Guitar/D.wav");
	Sounds.push_back("Assets/Sounds/Guitar/E.wav");
	Sounds.push_back("Assets/Sounds/Guitar/F.wav");
	Sounds.push_back("Assets/Sounds/Guitar/G.wav");
	Sounds.push_back("Assets/Sounds/Guitar/ALow.wav");
	Sounds.push_back("Assets/Sounds/Guitar/BLow.wav");

/*

	Sounds.push_back("Assets/Sounds/Wubs/WubSnare2.wav");
	Sounds.push_back("Assets/Sounds/Wubs/WubSnare.wav");
	Sounds.push_back("Assets/Sounds/Wubs/HeavySnare.wav");
	Sounds.push_back("Assets/Sounds/Wubs/WubKick.wav");
	Sounds.push_back("Assets/Sounds/Wubs/Kick2.wav");
	Sounds.push_back("Assets/Sounds/Wubs/Snare.wav");
	Sounds.push_back("Assets/Sounds/Wubs/Wub.wav");

*/

	renderer.LoadContent();
	physicsSystem.Initialize();
	
	btCollisionShape* shape = new btBoxShape(btBoxShape(btVector3(0.5f, 0.5f, 0.5f)));

 	//objectManager.AddObject(new Object(shape, "Assets/Models/Cube.obj", 1, 0.1, glm::vec3(0, 1, 0), glm::quat()));
 	//objectManager.AddObject(new Object(shape, "Assets/Models/Cube.obj", 1, 0.1, glm::vec3(0.75, 5, 0), glm::quat()));
	
	btCollisionShape* groundshape = new btBoxShape(btBoxShape(btVector3(75.f, 0.5f, 75.f)));
	objectManager.AddObject(new Object(groundshape, "Assets/Models/Plane.obj", "Assets/Sounds/Wubs/Wub.wav", 0, 1.f, glm::vec3(0, -5, 0), glm::quat()));

	{
		glm::quat rot = glm::quat();
		rot = rot * glm::angleAxis(90 * 3.14f / 180, glm::vec3(0, 0, -1));
		objectManager.AddObject(new Object(groundshape, "Assets/Models/Plane.obj", "Assets/Sounds/Wubs/Wub.wav", 0, 1.f, glm::vec3(-75, -5, 0), rot));
	}
	{
		glm::quat rot = glm::quat();
		rot = rot * glm::angleAxis(90 * 3.14f / 180, glm::vec3(0, 0, -1));
		objectManager.AddObject(new Object(groundshape, "Assets/Models/Plane.obj",  "Assets/Sounds/Wubs/Wub.wav", 0, 1.f, glm::vec3(75, -5, 0), rot));
	}
	{
		glm::quat rot = glm::quat();
		rot = rot * glm::angleAxis(90 * 3.14f / 180, glm::vec3(-1, 0, 0));
		objectManager.AddObject(new Object(groundshape, "Assets/Models/Plane.obj", "Assets/Sounds/Wubs/Wub.wav", 0, 1.f, glm::vec3(0, -5, 75), rot));
	}
	{
		glm::quat rot = glm::quat();
		objectManager.AddObject(new Object(groundshape, "Assets/Models/Plane.obj", "Assets/Sounds/Wubs/Wub.wav", 0, 1.f, glm::vec3(0, 60, 0), rot));
	}


	

	models.push_back(std::make_shared<Model>("Assets/Models/Sphere.obj"));
	models.push_back(std::make_shared<Model>("Assets/Models/Sphere1.obj"));
	models.push_back(std::make_shared<Model>("Assets/Models/Sphere2.obj"));
	models.push_back(std::make_shared<Model>("Assets/Models/Sphere3.obj"));
	models.push_back(std::make_shared<Model>("Assets/Models/Sphere4.obj"));
	models.push_back(std::make_shared<Model>("Assets/Models/Sphere5.obj"));
	models.push_back(std::make_shared<Model>("Assets/Models/Sphere6.obj"));


	for (Object* o : objectManager.GetObjects())
	{
		o->m_RigidBody = physicsSystem.AddRigidBody(o->m_Shape, o->m_Mass, o->m_Restitution, o->ModelMatrix());

	}
	
	renderer.AddLightToDraw();

	double lastTime = glfwGetTime();
	glfwSetCursorPos(renderer.GetWindow(), renderer.WIDTH / 2, renderer.HEIGHT / 2);

	m_LastKeyPress = glfwGetMouseButton(renderer.GetWindow(), GLFW_MOUSE_BUTTON_RIGHT);
	while (!glfwWindowShouldClose(renderer.GetWindow()))// MAIN LOOP
	{
		double time = glfwGetTime();
		double dt = time - lastTime;
		lastTime = time;

		

		

		for (Object* o : objectManager.GetObjects())
		{
			renderer.AddObjectToDraw(o, true);
		}

		objectManager.Update(dt);
		Input();
		Update(dt);
		ss.Update(
			renderer.m_Camera->Position(),
			renderer.m_Camera->Forward(),
			renderer.m_Camera->Up());


		physicsSystem.Update(dt);
		collisions = physicsSystem.CheckCollisions();

		bool played = false;
		for (auto tup : collisions)
		{
			btRigidBody* body1;
			btRigidBody* body2;
			btManifoldPoint mani;
			std::tie(body1, body2, mani) = collisions.back();
			
			for (Object* o : objectManager.GetObjects())
			{
				if (o->m_RigidBody->isStaticObject())
					continue;

				if ((o->m_RigidBody == body1 || o->m_RigidBody == body2))
				{
					//printf("body %d, impulse %f\n", mani.getAppliedImpulse());
					ss.PlaySFX(o->m_Sound, 0.5f, glm::vec3(o->m_Position));

					played = true;
					break;
				}
			}

			if (played)
				break;
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
	currentInput.push_back(glfwGetKey(renderer.GetWindow(), GLFW_KEY_Q));
	currentInput.push_back(glfwGetKey(renderer.GetWindow(), GLFW_KEY_E));
	currentInput.push_back(glfwGetKey(renderer.GetWindow(), GLFW_KEY_R));

	btCollisionShape* sphere = new btSphereShape(0.5f);
	Object* ob = new Object(sphere, models[0], Sounds[0], 1.f, 1.f, renderer.GetCameraPosition() + renderer.GetCameraForward(), glm::quat());
	
	
	//Beats
	if (currentInput[0] == GLFW_PRESS )//&& currentInput[0] != previousInput[0])
	{
		ob = new Object(sphere, models[0], Sounds[0], 1.f, 1.f, renderer.GetCameraPosition() + renderer.GetCameraForward(), glm::quat());
	}
	if (currentInput[1] == GLFW_PRESS )//&& currentInput[1] != previousInput[1])
	{
		ob = new Object(sphere, models[1], Sounds[1], 1, 1.f, renderer.GetCameraPosition() + renderer.GetCameraForward(), glm::quat());
	}
	if (currentInput[2] == GLFW_PRESS)//&& currentInput[2] != previousInput[2])
	{
		ob = new Object(sphere, models[2], Sounds[2], 1, 1.f, renderer.GetCameraPosition() + renderer.GetCameraForward(), glm::quat());
	}
	if (currentInput[3] == GLFW_PRESS)//&& currentInput[3] != previousInput[3])
	{
		ob = new Object(sphere, models[3], Sounds[3], 1, 1.f, renderer.GetCameraPosition() + renderer.GetCameraForward(), glm::quat());
	}
	if (currentInput[4] == GLFW_PRESS)//&& currentInput[4] != previousInput[4])
	{
		ob = new Object(sphere, models[4], Sounds[4], 1, 1.f, renderer.GetCameraPosition() + renderer.GetCameraForward(), glm::quat());
	}
	if (currentInput[5] == GLFW_PRESS)// && currentInput[5] != previousInput[5])
	{
		ob = new Object(sphere, models[5], Sounds[5], 1, 1.f, renderer.GetCameraPosition() + renderer.GetCameraForward(), glm::quat());
	}
	if (currentInput[6] == GLFW_PRESS)// && currentInput[6] != previousInput[6])
	{
		ob = new Object(sphere, models[6], Sounds[6], 1, 1.f, renderer.GetCameraPosition() + renderer.GetCameraForward(), glm::quat());
	}

	if (currentInput[7] == GLFW_PRESS && currentInput[7] != previousInput[7])
	{
		Sounds.clear();
		Sounds.push_back("Assets/Sounds/Wubs/WubSnare2.wav");
		Sounds.push_back("Assets/Sounds/Wubs/WubSnare.wav");
		Sounds.push_back("Assets/Sounds/Wubs/HeavySnare.wav");
		Sounds.push_back("Assets/Sounds/Wubs/WubKick.wav");
		Sounds.push_back("Assets/Sounds/Wubs/Kick2.wav");
		Sounds.push_back("Assets/Sounds/Wubs/Snare.wav");
		Sounds.push_back("Assets/Sounds/Wubs/Wub.wav");
	}

	if (currentInput[8] == GLFW_PRESS && currentInput[8] != previousInput[8])
	{
		Sounds.clear();
		Sounds.push_back("Assets/Sounds/Guitar/C.wav");
		Sounds.push_back("Assets/Sounds/Guitar/D.wav");
		Sounds.push_back("Assets/Sounds/Guitar/E.wav");
		Sounds.push_back("Assets/Sounds/Guitar/F.wav");
		Sounds.push_back("Assets/Sounds/Guitar/G.wav");
		Sounds.push_back("Assets/Sounds/Guitar/ALow.wav");
		Sounds.push_back("Assets/Sounds/Guitar/BLow.wav");
	}

	if (currentInput[9] == GLFW_PRESS && currentInput[9] != previousInput[9])
	{
		Sounds.clear();
		Sounds.push_back("Assets/Sounds/Blip/C6.wav");
		Sounds.push_back("Assets/Sounds/Blip/D6.wav");
		Sounds.push_back("Assets/Sounds/Blip/E6.wav");
		Sounds.push_back("Assets/Sounds/Blip/F6.wav");
		Sounds.push_back("Assets/Sounds/Blip/G6.wav");
		Sounds.push_back("Assets/Sounds/Blip/A6.wav");
		Sounds.push_back("Assets/Sounds/Blip/H6.wav");
	}

	

	previousInput = currentInput;
	currentInput.clear();


	if (glfwGetMouseButton(renderer.GetWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE
		&& m_LastKeyPress == GLFW_PRESS)
	{
		objectManager.AddObject(ob);
		ob->m_RigidBody = physicsSystem.AddRigidBody(ob->m_Shape, ob->m_Mass, ob->m_Restitution, ob->ModelMatrix());
		glm::vec3 force = renderer.GetCameraForward();
		if (glfwGetKey(renderer.GetWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			force *= 0;
		}
		else
		{
			force *= 50;
		}
		ob->m_RigidBody->applyImpulse(btVector3(force.x, force.y, force.z), btVector3(0, 0, 0));
	}




	if (glfwGetKey(renderer.GetWindow(), GLFW_KEY_F1) == GLFW_PRESS)
	{
		ss.PlayBGM("Assets/Sounds/beat1.wav", bgmVolume);
	}
	if (glfwGetKey(renderer.GetWindow(), GLFW_KEY_F2) == GLFW_PRESS)
	{
		ss.PlayBGM("Assets/Sounds/beat2.wav", bgmVolume);
	}
	if (glfwGetKey(renderer.GetWindow(), GLFW_KEY_F3) == GLFW_PRESS)
	{
		ss.PlayBGM("Assets/Sounds/beat3.wav", bgmVolume);
	}
	if (glfwGetKey(renderer.GetWindow(), GLFW_KEY_F4) == GLFW_PRESS)
	{
		ss.PlayBGM("Assets/Sounds/beat4.wav", bgmVolume);
	}
	if (glfwGetKey(renderer.GetWindow(), GLFW_KEY_F5) == GLFW_PRESS)
	{
		ss.PlayBGM("Assets/Sounds/beat5.wav", bgmVolume);
	}


	m_LastKeyPress = glfwGetMouseButton(renderer.GetWindow(), GLFW_MOUSE_BUTTON_RIGHT);

}
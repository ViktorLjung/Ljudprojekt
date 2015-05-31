#include "PrecompiledHeader.h"

#include "Rectangle.h"
#include"Camera.h"
#include "Model.h"
#include "Renderer.h"
#include "PhysicsSystem.h"

Renderer renderer = Renderer();
std::shared_ptr<Model> model;
//std::shared_ptr<Model> Plane;

PhysicsSystem physicsSystem;

void Update(double dt);

int main()
{
	renderer.LoadContent();
	physicsSystem.Initialize();
	
	model = std::make_shared<Model>("Assets/Models/SSAOTest3.obj", glm::vec3(0), glm::quat(), glm::vec3(1));
	//Plane = std::make_shared<Model>("Assets/Models/Raptor.obj", glm::vec3(3, 0, -3), glm::quat(), glm::vec3(1, 1, 1));
	renderer.AddModelToDraw(model, true);
	//renderer.AddModelToDraw(Plane, true);
	renderer.AddLightToDraw();

	double lastTime = glfwGetTime();
	glfwSetCursorPos(renderer.GetWindow(), renderer.WIDTH / 2, renderer.HEIGHT / 2);


	while (!glfwWindowShouldClose(renderer.GetWindow()))// MAIN LOOP
	{
		double time = glfwGetTime();
		double dt = time - lastTime;
		lastTime = time;

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


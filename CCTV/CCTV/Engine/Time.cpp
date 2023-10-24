#include "Time.h"
#include <GLFW/glfw3.h>

using namespace Engine;

std::vector<RepeatFunc*> Time::REPEATOR_FUNCS;
double Time::lastTime = 0;
double Time::deltaTime = 0;

void Time::Update()
{
	double t = FixedTime();

	deltaTime = t - lastTime;
	lastTime = t;

	for (RepeatFunc* i : REPEATOR_FUNCS)
	{
		if (i->nextcall > t)
			continue;

		i->nextcall = t + i->time;
		i->func();
	}
}

void Time::OnExit()
{
	for (RepeatFunc* i : REPEATOR_FUNCS)
		delete i;
	REPEATOR_FUNCS.clear();
}

double Time::FixedTime()
{
	return glfwGetTime();
}

double Time::DeltaTime()
{
	return deltaTime;
}

void Time::Repeat(double x, void (*ptr)(void))
{
	RepeatFunc r;
	r.time = x;
	r.func = ptr;
}
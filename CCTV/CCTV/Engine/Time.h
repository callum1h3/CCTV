#pragma once

#include <vector>

namespace Engine
{
	struct RepeatFunc
	{
	public:
		double time;
		double nextcall = 0;
		void (*func)(void);
	};

	class __declspec(dllexport) Time
	{
	public:
		static void Update();
		static void OnExit();

		static double FixedTime();
		static double DeltaTime();

		static void Repeat(double x, void (*ptr)(void));
	private:
		static double lastTime;
		static double deltaTime;

		static std::vector<RepeatFunc*> REPEATOR_FUNCS;
	};
}
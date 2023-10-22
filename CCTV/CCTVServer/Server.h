#pragma once

// How many times a second will the server run.
constexpr int SERVER_TICK_SPEED = 32;

class Server
{
public:
	void Run();
private:
	void ThreadSleep(int milliseconds);
};
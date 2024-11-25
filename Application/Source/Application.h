
#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"

class Application
{
public:
	Application();
	~Application();
	void Init();
	void Run();
	static void Exit();
	static bool IsKeyPressed(unsigned short key);

	static void stopPointer();
	static void startPointer();

	static void quit();

	static bool stop;
	static bool enablePointer;
	static bool showPointer;
private:

	//Declare a window object
	StopWatch m_timer;

	
};

#endif
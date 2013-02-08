// BattleChess3D.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Position.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	Position position;
	position.start();
	position.showPosition();
	
	// Prevents program from exiting.
	while(1)
	{
		this_thread::sleep_for(chrono::milliseconds(1000));
		position.showPosition();
	}

	return 0;
}

#include "ClickerGame.h"
#include <windows.h>
#include <atomic>

namespace ClickerGame {

	int power = 0;

	const int DECREASE_AMOUNT = 1;
	const int INCREASE_AMOUNT = 1;
	const int LOOP_TIME = 1000;

	void StartGameLoop()
	{
		for (;;)
		{
			SubtractPower();
			GameLoopTriggered();
			Sleep(LOOP_TIME);
		}
	}

	void Click()
	{
		power += INCREASE_AMOUNT;
	}

	void SubtractPower()
	{
		if (power > 0)
		{
			power -= DECREASE_AMOUNT;
		}
	}
	
	int GetPower()
	{
		return power;
	}
}
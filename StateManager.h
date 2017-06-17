#pragma once
#include "Singleton.h"

class StateManager : Singleton <StateManager>
{
public:
	enum GameState { MainMenu, Play, Pause };

	enum EnemyState { Patrol, Attack };

	enum PlayerState { Walk, Run };
};
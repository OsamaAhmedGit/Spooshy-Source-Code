#pragma once

#include "DrawableGameComponent.h"
#include "Globals.h"
#include "d3dx11effect.h"
#include <vector>
#include <iostream>

using namespace Library;
using namespace std;

namespace Rendering
{
	class Enemy;

	class EnemyManager : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(EnemyManager, DrawableGameComponent)

	public:
		EnemyManager(Game& game, Camera& camera);
		virtual ~EnemyManager();

		virtual void Initialize(int level);
		Enemy *enemy;
		vector <Enemy*> enemyVector;
	};
}

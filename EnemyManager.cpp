#pragma once
#include "EnemyManager.h"
#include "Game.h"
#include "GameException.h"
#include "MatrixHelper.h"
#include "Camera.h"
#include "Utility.h"
#include "D3DCompiler.h"
#include "WICTextureLoader.h"
#include <vector>
#include "d3dx11effect.h"
#include "d3d11.h"
#include "RenderingGame.h"
#include <iostream>
#include "Enemy.h"

using namespace std;
using namespace Library;

namespace Rendering
{
#pragma region Constructor
	RTTI_DEFINITIONS(EnemyManager, DrawableGameComponent)

		EnemyManager::EnemyManager(Game& game, Camera& camera)
		: DrawableGameComponent(game, camera),
		enemy(nullptr)
	{
	}
#pragma endregion

#pragma region Destructor
	EnemyManager::~EnemyManager()
	{
	}
#pragma endregion

#pragma region Initialization
	void EnemyManager::Initialize(int level)
	{
		enemyVector.clear();

		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		if (level == 1)
		{
			enemy = new Enemy(*mGame, *mCamera, "Content\\Models\\knight.3ds");
			enemy->pos = { 0, 0, 0 };
			enemyVector.push_back(enemy);

			enemy = new Enemy(*mGame, *mCamera, "Content\\Models\\knight.3ds");
			enemy->pos = { 100, 0, 100 };
			enemyVector.push_back(enemy);
		}

		if (level == 2)
		{
			enemy = new Enemy(*mGame, *mCamera, "Content\\Models\\knight.3ds");
			enemy->pos = { 150, 0, -20 };
			enemyVector.push_back(enemy);

			enemy = new Enemy(*mGame, *mCamera, "Content\\Models\\knight.3ds");
			enemy->pos = { 100, 0, 100 };
			enemyVector.push_back(enemy);

			enemy = new Enemy(*mGame, *mCamera, "Content\\Models\\knight.3ds");
			enemy->pos = { 100, 0, 100 };
			enemyVector.push_back(enemy);
		}

	}
#pragma endregion
}

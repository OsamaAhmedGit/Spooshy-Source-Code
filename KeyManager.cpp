#pragma once
#include "KeyManager.h"
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

using namespace std;
using namespace Library;

namespace Rendering
{
#pragma region Constructor
	RTTI_DEFINITIONS(KeyManager, DrawableGameComponent)

		KeyManager::KeyManager(Game& game, Camera& camera)
		: DrawableGameComponent(game, camera),
		key1(nullptr), key2(nullptr)
	{
	}
#pragma endregion

#pragma region Destructor
	KeyManager::~KeyManager()
	{
	}
#pragma endregion

#pragma region Initialization
	void KeyManager::Initialize(int level)
	{
		keyVector.clear();
		keyColBoxesVector.clear();

		enabled = true;

		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		if (level == 1)
		{
			key1 = new Key(*mGame, *mCamera, "Content\\Models\\Key2.3ds");
			key1->pos = { 80, 6, 40 };
			keyVector.push_back(key1);

			keyColBoxes.vecMin = { 78, -100, 38 };
			keyColBoxes.vecMax = { 80, 100, 40 };
			keyColBoxesVector.push_back(keyColBoxes);

			key1 = new Key(*mGame, *mCamera, "Content\\Models\\Key2.3ds");
			key1->pos = { -170, 6, -160 };
			keyVector.push_back(key1);

			keyColBoxes.vecMin = { -172, -100, -162 };
			keyColBoxes.vecMax = { -170, 100, -160 };
			keyColBoxesVector.push_back(keyColBoxes);

		}

		if (level == 2)
		{
			key1 = new Key(*mGame, *mCamera, "Content\\Models\\Key2.3ds");
			key1->pos = { -170, 6, -160 };
			keyVector.push_back(key1);

			keyColBoxes.vecMin = { -172, -100, -162 };
			keyColBoxes.vecMax = { -170, 100, -160 };
			keyColBoxesVector.push_back(keyColBoxes);

			key1 = new Key(*mGame, *mCamera, "Content\\Models\\Key2.3ds");
			key1->pos = { 120, 6, 180 };
			keyVector.push_back(key1);

			keyColBoxes.vecMin = { 118, -100, 178 };
			keyColBoxes.vecMax = { 120, 100, 180 };
			keyColBoxesVector.push_back(keyColBoxes);

		}

	}

#pragma endregion

	void KeyManager::Update(const GameTime &gameTime)
	{
		for (int i = 0; i < keyVector.size(); i++)
		{
			keyVector[i]->SetPosition(0, keyVector[i]->mAngle, 0, 0.5, keyVector[i]->pos.x, keyVector[i]->pos.y, keyVector[i]->pos.z);
		}
	}
}

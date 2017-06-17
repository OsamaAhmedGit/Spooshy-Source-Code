#pragma once
#include "WallManager.h"
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
#include "Wall2.h"

using namespace std;
using namespace Library;

namespace Rendering
{
#pragma region Constructor
	RTTI_DEFINITIONS(WallManager)

		WallManager::WallManager(Game& game, Camera& camera)
		: DrawableGameComponent(game, camera),
		mOuterWalls(nullptr), mLevelWalls(nullptr)
	{
	}
#pragma endregion

#pragma region Destructor
	WallManager::~WallManager()
	{
		DeleteObject(mOuterWalls);
		DeleteObject(mLevelWalls);
	}
#pragma endregion

#pragma region Initialization
	void WallManager::Initialize(int level)
	{
		collisionBoxesWalls.clear();
		outerWallsVec.clear();

		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

#pragma region OuterWalls
		/*RIGHT OUTER WALL*/
		if (level > 0)
		{
			mOuterWalls = new Wall(*mGame, *mCamera);
			mOuterWalls->SetPosition(200, 200, 200, 200, 50, 50, 0, 0, -100, -100);
			outerWallsVec.push_back(mOuterWalls);

			mOuterWalls = new Wall(*mGame, *mCamera);
			mOuterWalls->SetPosition(200, 200, 200, 200, 50, 50, 0, 0, 100, 100);
			outerWallsVec.push_back(mOuterWalls);

			mOuterWalls = new Wall(*mGame, *mCamera);
			mOuterWalls->SetPosition(200, 200, 200, 200, 50, 50, 100, 100, 200, 200);
			outerWallsVec.push_back(mOuterWalls);

			mOuterWalls = new Wall(*mGame, *mCamera);
			mOuterWalls->SetPosition(200, 200, 200, 200, 50, 50, -100, -100, -200, -200);
			outerWallsVec.push_back(mOuterWalls);

			outerWallsColBoxes.vecMax = { 200, 50, 200 };
			outerWallsColBoxes.vecMin = { 200, 0, -200 };
			collisionBoxesWalls.push_back(outerWallsColBoxes);

			/*BACK OUTER WALL*/
			mOuterWalls = new Wall(*mGame, *mCamera);
			mOuterWalls->SetPosition(-100, -100, 0, 0, 50, 50, 200, 200, 200, 200);
			outerWallsVec.push_back(mOuterWalls);

			mOuterWalls = new Wall(*mGame, *mCamera);
			mOuterWalls->SetPosition(-200, -200, -100, -100, 50, 50, 200, 200, 200, 200);
			outerWallsVec.push_back(mOuterWalls);

			mOuterWalls = new Wall(*mGame, *mCamera);
			mOuterWalls->SetPosition(100, 100, 0, 0, 50, 50, 200, 200, 200, 200);
			outerWallsVec.push_back(mOuterWalls);

			mOuterWalls = new Wall(*mGame, *mCamera);
			mOuterWalls->SetPosition(200, 200, 100, 100, 50, 50, 200, 200, 200, 200);
			outerWallsVec.push_back(mOuterWalls);

			outerWallsColBoxes.vecMax = { 200, 50, 200 };
			outerWallsColBoxes.vecMin = { -200, 0, 200 };
			collisionBoxesWalls.push_back(outerWallsColBoxes);

			/*FRONT OUTER WALL*/
			mOuterWalls = new Wall(*mGame, *mCamera);
			mOuterWalls->SetPosition(-200, -200, -100, -100, 50, 50, -200, -200, -200, -200);
			outerWallsVec.push_back(mOuterWalls);

			mOuterWalls = new Wall(*mGame, *mCamera);
			mOuterWalls->SetPosition(-100, -100, 0, 0, 50, 50, -200, -200, -200, -200);
			outerWallsVec.push_back(mOuterWalls);

			mOuterWalls = new Wall(*mGame, *mCamera);
			mOuterWalls->SetPosition(0, 0, 100, 100, 50, 50, -200, -200, -200, -200);
			outerWallsVec.push_back(mOuterWalls);

			mOuterWalls = new Wall(*mGame, *mCamera);
			mOuterWalls->SetPosition(100, 100, 200, 200, 50, 50, -200, -200, -200, -200);
			outerWallsVec.push_back(mOuterWalls);

			outerWallsColBoxes.vecMax = { 200, 50, -200 };
			outerWallsColBoxes.vecMin = { -200, 0, -200 };
			collisionBoxesWalls.push_back(outerWallsColBoxes);

			/*LEFT OUTER WALL*/
			mOuterWalls = new Wall(*mGame, *mCamera);
			mOuterWalls->SetPosition(-200, -200, -200, -200, 50, 50, 0, 0, -100, -100);
			outerWallsVec.push_back(mOuterWalls);

			mOuterWalls = new Wall(*mGame, *mCamera);
			mOuterWalls->SetPosition(-200, -200, -200, -200, 50, 50, -100, -100, -200, -200);
			outerWallsVec.push_back(mOuterWalls);

			mOuterWalls = new Wall(*mGame, *mCamera);
			mOuterWalls->SetPosition(-200, -200, -200, -200, 50, 50, 0, 0, 100, 100);
			outerWallsVec.push_back(mOuterWalls);

			mOuterWalls = new Wall(*mGame, *mCamera);
			mOuterWalls->SetPosition(-200, -200, -200, -200, 50, 50, 100, 100, 200, 200);
			outerWallsVec.push_back(mOuterWalls);

			outerWallsColBoxes.vecMax = { -200, 50, 200 };
			outerWallsColBoxes.vecMin = { -200, 0, -200 };
			collisionBoxesWalls.push_back(outerWallsColBoxes);
		}
#pragma endregion

#pragma region Level 1
		if (level == 1)
		{
			textureName = L"Content\\Textures\\wall.jpg";

			mLevelWalls = new Wall2(*mGame, *mCamera, textureName);
			mLevelWalls->SetPosition(-200, -100, -100, -200, -200, -100, -100, -200,
				0, 0, 0, 0, 50, 50, 50, 50,
				65, 65, 70, 70, 65, 65, 70, 70);
			levelWallsVec.push_back(mLevelWalls);

			levelWallColBoxes.vecMax = { -100, 50, 70 };
			levelWallColBoxes.vecMin = { -200, 0, 65 };
			collisionBoxesWalls.push_back(levelWallColBoxes);

			mLevelWalls = new Wall2(*mGame, *mCamera, textureName);
			mLevelWalls->SetPosition(-125, -55.5, -55.5, -125, -125, -55.5, -55.5, -125,
				0, 0, 0, 0, 50, 50, 50, 50,
				135, 135, 140, 140, 135, 135, 140, 140);
			levelWallsVec.push_back(mLevelWalls);

			levelWallColBoxes.vecMax = { -55.5, 50, 140 };
			levelWallColBoxes.vecMin = { -125, 0, 135 };
			collisionBoxesWalls.push_back(levelWallColBoxes);

			mLevelWalls = new Wall2(*mGame, *mCamera, textureName);
			mLevelWalls->SetPosition(50.5, 160.5, 160.5, 50.5, 50.5, 160.5, 160.5, 50.5,
				0, 0, 0, 0, 50, 50, 50, 50,
				55, 55, 60, 60, 55, 55, 60, 60);
			levelWallsVec.push_back(mLevelWalls);

			levelWallColBoxes.vecMax = { 160.5, 50, 60 };
			levelWallColBoxes.vecMin = { 50.5, 0, 55 };
			collisionBoxesWalls.push_back(levelWallColBoxes);

			mLevelWalls = new Wall2(*mGame, *mCamera, textureName);
			mLevelWalls->SetPosition(50.5, 160.5, 160.5, 50.5, 50.5, 160.5, 160.5, 50.5,
				0, 0, 0, 0, 50, 50, 50, 50,
				-75, -75, -80, -80, -75, -75, -80, -80);
			levelWallsVec.push_back(mLevelWalls);

			levelWallColBoxes.vecMax = { 160.5, 50, -75 };
			levelWallColBoxes.vecMin = { 50.5, 0, -80 };
			collisionBoxesWalls.push_back(levelWallColBoxes);

			mLevelWalls = new Wall2(*mGame, *mCamera, textureName);
			mLevelWalls->SetPosition(-160, -155, -155, -160, -160, -155, -155, -160,
				0, 0, 0, 0, 50, 50, 50, 50,
				125, 125, 200, 200, 125, 125, 200, 200);
			levelWallsVec.push_back(mLevelWalls);

			levelWallColBoxes.vecMax = { -155, 50, 200 };
			levelWallColBoxes.vecMin = { -160, 0, 125 };
			collisionBoxesWalls.push_back(levelWallColBoxes);

			mLevelWalls = new Wall2(*mGame, *mCamera, textureName);
			mLevelWalls->SetPosition(-140, -135, -135, -140, -140, -135, -135, -140,
				0, 0, 0, 0, 50, 50, 50, 50,
				-200, -200, 0, 0, -200, -200, 0, 0);
			levelWallsVec.push_back(mLevelWalls);

			levelWallColBoxes.vecMax = { -135, 50, 0 };
			levelWallColBoxes.vecMin = { -140, 0, -200 };
			collisionBoxesWalls.push_back(levelWallColBoxes);

			mLevelWalls = new Wall2(*mGame, *mCamera, textureName);
			mLevelWalls->SetPosition(140, 135, 135, 140, 140, 135, 135, 140,
				0, 0, 0, 0, 50, 50, 50, 50,
				105, 105, 200, 200, 105, 105, 200, 200);
			levelWallsVec.push_back(mLevelWalls);

			levelWallColBoxes.vecMax = { 140, 50, 200 };
			levelWallColBoxes.vecMin = { 135, 0, 105 };
			collisionBoxesWalls.push_back(levelWallColBoxes);

			mLevelWalls = new Wall2(*mGame, *mCamera, textureName);
			mLevelWalls->SetPosition(105, 100, 100, 105, 105, 100, 100, 105,
				0, 0, 0, 0, 50, 50, 50, 50,
				-40, -40, 55, 55, -40, -40, 55, 55);
			levelWallsVec.push_back(mLevelWalls);

			levelWallColBoxes.vecMax = { 105, 50, 55 };
			levelWallColBoxes.vecMin = { 100, 0,-40 };
			collisionBoxesWalls.push_back(levelWallColBoxes);

			mLevelWalls = new Wall2(*mGame, *mCamera, textureName);
			mLevelWalls->SetPosition(-55.5, -50.5, -50.5, -55.5, -55.5, -50.5, -50.5, -55.5,
				0, 0, 0, 0, 50, 50, 50, 50,
				-125, -125, 200, 200, -125, -125, 200, 200);
			levelWallsVec.push_back(mLevelWalls);

			levelWallColBoxes.vecMax = { -50.5, 50, 200 };
			levelWallColBoxes.vecMin = { -55.5, 0, -125 };
			collisionBoxesWalls.push_back(levelWallColBoxes);

			mLevelWalls = new Wall2(*mGame, *mCamera, textureName);
			mLevelWalls->SetPosition(50.5, 45.5, 45.5, 50.5, 50.5, 45.5, 45.5, 50.5,
				0, 0, 0, 0, 50, 50, 50, 50,
				-200, -200, 125, 125, -200, -200, 125, 125);
			levelWallsVec.push_back(mLevelWalls);

			levelWallColBoxes.vecMax = { 50.5, 50, 125 };
			levelWallColBoxes.vecMin = { 45.5, 0, -200 };
			collisionBoxesWalls.push_back(levelWallColBoxes);
		}

		if (level == 2)
		{
			textureName = L"Content\\Textures\\clouds.jpg";

			mLevelWalls = new Wall2(*mGame, *mCamera, textureName);
			mLevelWalls->SetPosition(-200, -100, -100, -200, -200, -100, -100, -200,
				0, 0, 0, 0, 50, 50, 50, 50,
				65, 65, 70, 70, 65, 65, 70, 70);
			levelWallsVec.push_back(mLevelWalls);

			levelWallColBoxes.vecMax = { -100, 50, 70 };
			levelWallColBoxes.vecMin = { -200, 0, 65 };
			collisionBoxesWalls.push_back(levelWallColBoxes);

			mLevelWalls = new Wall2(*mGame, *mCamera, textureName);
			mLevelWalls->SetPosition(-140, -135, -135, -140, -140, -135, -135, -140,
				0, 0, 0, 0, 50, 50, 50, 50,
				150, 150, 200, 200, 150, 150, 200, 200);
			levelWallsVec.push_back(mLevelWalls);

			levelWallColBoxes.vecMax = { -135, 50, 200 };
			levelWallColBoxes.vecMin = { -140, 0, 150 };
			collisionBoxesWalls.push_back(levelWallColBoxes);


			mLevelWalls = new Wall2(*mGame, *mCamera, textureName);
			mLevelWalls->SetPosition(-140, -135, -135, -140, -140, -135, -135, -140,
				0, 0, 0, 0, 50, 50, 50, 50,
				-200, -200, 0, 0, -200, -200, 0, 0);
			levelWallsVec.push_back(mLevelWalls);

			levelWallColBoxes.vecMax = { -135, 50, 0 };
			levelWallColBoxes.vecMin = { -140, 0, -200 };
			collisionBoxesWalls.push_back(levelWallColBoxes);

			mLevelWalls = new Wall2(*mGame, *mCamera, textureName);
			mLevelWalls->SetPosition(140, 135, 135, 140, 140, 135, 135, 140,
				0, 0, 0, 0, 50, 50, 50, 50,
				-20, -20, 200, 200, -20, -20, 200, 200);
			levelWallsVec.push_back(mLevelWalls);

			levelWallColBoxes.vecMax = { 140, 50, 200 };
			levelWallColBoxes.vecMin = { 135, 0, -20 };
			collisionBoxesWalls.push_back(levelWallColBoxes);
		}

		if (level == 3)
		{
			textureName = L"Content\\Textures\\stonewall.png";

			mLevelWalls = new Wall2(*mGame, *mCamera, textureName);
			mLevelWalls->SetPosition(-50.5, 150.5, 150.5, -50.5, -50.5, 150.5, 150.5, -50.5,
				0, 0, 0, 0, 50, 50, 50, 50,
				35, 35, 40, 40, 35, 35, 40, 40);
			levelWallsVec.push_back(mLevelWalls);

			levelWallColBoxes.vecMax = { 150.5, 50, 40 };
			levelWallColBoxes.vecMin = { -50.5, 0, 35 };
			collisionBoxesWalls.push_back(levelWallColBoxes);

			mLevelWalls = new Wall2(*mGame, *mCamera, textureName);
			mLevelWalls->SetPosition(-55.5, -50.5, -50.5, -55.5, -55.5, -50.5, -50.5, -55.5,
				0, 0, 0, 0, 50, 50, 50, 50,
				-125, -125, 200, 200, -125, -125, 200, 200);
			levelWallsVec.push_back(mLevelWalls);

			levelWallColBoxes.vecMax = { -50.5, 50, 200 };
			levelWallColBoxes.vecMin = { -55.5, 0, -125 };
			collisionBoxesWalls.push_back(levelWallColBoxes);
		}
#pragma endregion

	}

#pragma endregion
}

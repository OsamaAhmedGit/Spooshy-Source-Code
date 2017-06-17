#include "RenderingGame.h"
#include "GameException.h"
#include "FirstPersonCamera.h"
#include "TriangleDemo.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Wall2.h"
#include "Wall.h"
#include "VectorHelper.h"
#include "Player.h"
#include "Globals.h"
#include "Enemy.h"
#include "FpsComponent.h"
#include "RenderStateHelper.h"
#include "TextDraw.h"
#include "Game.h"
#include "Utility.h"
#include "WallManager.h"
#include "Menu.h"
#include "Key.h"
#include "StateManager.h"
#include "KeyManager.h"
#include <memory>
#include "BinaryReader.h"
#include "BinaryWriter.h"
#include "ExitDoor.h"
#include "EnemyManager.h"
#include "ObjectDiffuseLight.h"
#include "SamplerStates.h"
#include "RasterizerStates.h"

using namespace Library;
using namespace std;

namespace Rendering
{;

	const XMFLOAT4 RenderingGame::BackgroundColor = { 0.0f, 0.0f, 0.0f, 1.0f };

	RenderingGame::RenderingGame(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand)
		: Game(instance, windowClass, windowTitle, showCommand),
		mGround(nullptr), mDirectInput(nullptr), mMouse(nullptr), mKeyboard(nullptr),
		mModel(nullptr), mPlayer(nullptr), mEnemy(nullptr), mRenderStateHelper(nullptr), mTextDraw(nullptr),
		mSpriteBatch(nullptr), mSpriteFont(nullptr), mTextPosition(400.0f, 20.0f),
		mFrameCount(0), mFrameRate(0), mLastTotalElapsedTime(0.0), mWallManager(nullptr), mMenu(nullptr), level(0), gameState(0), mKeyManager(nullptr), pause(false), mDoor(nullptr)
		,mEnemyManager(nullptr)
		
    {
        mDepthStencilBufferEnabled = true;
        mMultiSamplingEnabled = true;

		mObjectDiffuseLight = nullptr;

		hud = nullptr;

		levelLoaded1 = false;
		levelLoaded2 = false;

		attackDelay = 1.0f;

		timer = 0.0f;
    }

    RenderingGame::~RenderingGame()
    {
    }

    void RenderingGame::Initialize()
	{	
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

        mCamera = new FirstPersonCamera(*this);
        mComponents.push_back(mCamera);
        mServices.AddService(Camera::TypeIdClass(), mCamera);        

		if (FAILED(DirectInput8Create(mInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&mDirectInput, nullptr)))
		{
			throw GameException("DirectInput8Create() failed");
		}

		mMenu = new Menu(*this);

		mTextDraw = new TextDraw(*this, *mCamera);

		mKeyboard = new Keyboard(*this, mDirectInput);
		mComponents.push_back(mKeyboard);
		mServices.AddService(Keyboard::TypeIdClass(), mKeyboard);

		mMouse = new Mouse(*this, mDirectInput);
		mComponents.push_back(mMouse);
		mServices.AddService(Mouse::TypeIdClass(), mMouse);
		
		mPlayer = new Player(*this, *mCamera);
		
		mModel = new Key(*this, *mCamera, "Content\\Models\\Key2.3ds");
		
		mRenderStateHelper = new RenderStateHelper(*this);

		mWallManager = new WallManager(*this, *mCamera);

		mKeyManager = new KeyManager(*this, *mCamera);

		mEnemyManager = new EnemyManager(*this, *mCamera);

		mDoor = new ExitDoor(*this, *mCamera, "Content\\Models\\door3.3ds");

		mObjectDiffuseLight = new ObjectDiffuseLight(*this, *mCamera);

		mObjectDiffuseLight->SetPosition(-1.57f, -0.0f, -0.0f, 0.01, -1.0f, 0.75f, -2.5f);

		hud = new TextDraw(*this, *mCamera);

		hud->SetPosition(-1.57f, 0.0f, 0.0f, 0.01f, -1.0f, 0.75f, -2.5f);

		if (level == 1)
		{
			textureName = L"Content\\Textures\\grass2.jpg";

			exit.vecMin = { 145, -100, -200 };
			exit.vecMax = { 165, 100, -195 };

			mDoor->SetPosition(1.57, 0, 0, 0.025, 150, 15, -197);
			mComponents.push_back(mDoor);
		}

		if (level == 2)
		{
			textureName = L"Content\\Textures\\sky.jpg";

			exit.vecMin = { 85, -100, -200 };
			exit.vecMax = { 95, 100, -195 };

			mDoor->SetPosition(1.57, 0, 0, 0.025, 90, 15, -197);
			mComponents.push_back(mDoor);
		}

		if (level == 3)
		{
			textureName = L"Content\\Textures\\lavawall.jpg";

			exit.vecMin = { 95, -100, 200 };
			exit.vecMax = { 115, 100, 195 };

			mDoor->SetPosition(1.57, 0, 0, 0.025, 100, 15, 197);
			mComponents.push_back(mDoor);
		}

		if (level == 0)
			mMenu->Initialize();

		if (level > 0)
		{
			mPlayer->Initialize(level);

			mComponents.push_back(mTextDraw);

			mGround = new TriangleDemo(*this, *mCamera, textureName);
			mComponents.push_back(mGround);

			mComponents.push_back(mPlayer);

			mWallManager->Initialize(level);

			mKeyManager->Initialize(level);

			mEnemyManager->Initialize(level);

			for (int i = 0; i < mWallManager->outerWallsVec.size(); i++)
			{
				mComponents.push_back(mWallManager->outerWallsVec[i]);
			}

			for (int i = 0; i < mWallManager->levelWallsVec.size(); i++)
			{
				mComponents.push_back(mWallManager->levelWallsVec[i]);
			}

			for (int i = 0; i < mEnemyManager->enemyVector.size(); i++)
			{
				mComponents.push_back(mEnemyManager->enemyVector[i]);
			}
			
			for (int i = 0; i < mKeyManager->keyVector.size(); i++)
			{
				mComponents.push_back(mKeyManager->keyVector[i]);
			}

			//mComponents.push_back(mObjectDiffuseLight);

			mComponents.push_back(hud);

			RasterizerStates::Initialize(mDirect3DDevice);

			SamplerStates::Initialize(mDirect3DDevice);
		}

		Game::Initialize();
    }

    void RenderingGame::Shutdown()
    {
		DeleteObject(mPlayer);
		DeleteObject(mEnemy);
		DeleteObject(mGround);
        DeleteObject(mCamera);
		DeleteObject(mKeyboard);
		DeleteObject(mMouse);
		DeleteObject(mModel);
		DeleteObject(mRenderStateHelper);
		DeleteObject(mTextDraw);
		DeleteObject(mSpriteBatch);
		DeleteObject(mSpriteFont);
		DeleteObject(mWallManager);
		DeleteObject(mKeyManager);
		DeleteObject(mEnemyManager);
		DeleteObject(mMenu);
		DeleteObject(mDoor);
		DeleteObject(mObjectDiffuseLight);
		DeleteObject(hud);
		ReleaseObject(mDirectInput);
		
        Game::Shutdown();
    }

    void RenderingGame::Update(const GameTime &gameTime)
    {
		if (mKeyboard->WasKeyPressedThisFrame(DIK_ESCAPE))
		{
			Exit();
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_RETURN) && level == 0)
		{
			level++;
			levelLoaded1 = true;
		}

		/*if (mKeyboard->WasKeyPressedThisFrame(DIK_L) && level == 1)
		{
			level++;
			levelLoaded2 = true;
			mComponents.clear();
		}*/

		if (levelLoaded1 == true)
		{
			Initialize();
			levelLoaded1 = false;
		}

		if (levelLoaded2 == true)
		{
			Initialize();
			levelLoaded2 = false;		
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_J) && level > 0)
		{
			Save();
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_L) && level > 0)
		{
			Load();
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_P) && level > 0)
		{
			pause = !pause;
		}

		if (level > 0 && pause == false)
		{
			mCamera->SetPosition(mPlayer->pos.x, mPlayer->pos.y + 7, mPlayer->pos.z);

			collisionDetection(gameTime);

			mPlayer->previousPosition = mPlayer->pos;

			if (mPlayer->maxHealth <= 0)
			{
				level = 0;
				mComponents.clear();
				Initialize();
			}

			PlayerInput(gameTime);

			mKeyManager->Update(gameTime);
		}

        Game::Update(gameTime);
    }

    void RenderingGame::Draw(const GameTime &gameTime)
    {
        mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&BackgroundColor));
        mDirect3DDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		ID3D11DeviceContext* direct3DDeviceContext = this->Direct3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mRenderStateHelper->SaveAll();

		if (level > 0)
			mTextDraw->Draw(gameTime, mPlayer->maxHealth, mPlayer->keyFragments);
		if (level == 0)
			mMenu->Draw(gameTime);

		mRenderStateHelper->RestoreAll();

        Game::Draw(gameTime);
       
        HRESULT hr = mSwapChain->Present(0, 0);

        if (FAILED(hr))
        {
            throw GameException("IDXGISwapChain::Present() failed.", hr);
        }
    }

	void RenderingGame::collisionDetection(const GameTime& gameTime) {
		
			for (int i = 0; i < mWallManager->collisionBoxesWalls.size(); i++) {
				if (mPlayer->playerCollisionBox.vecMax.x >= mWallManager->collisionBoxesWalls[i].vecMin.x && mPlayer->playerCollisionBox.vecMin.x <= mWallManager->collisionBoxesWalls[i].vecMax.x &&
					mPlayer->playerCollisionBox.vecMax.y >= mWallManager->collisionBoxesWalls[i].vecMin.y && mPlayer->playerCollisionBox.vecMin.y <= mWallManager->collisionBoxesWalls[i].vecMax.y &&
					mPlayer->playerCollisionBox.vecMax.z >= mWallManager->collisionBoxesWalls[i].vecMin.z && mPlayer->playerCollisionBox.vecMin.z <= mWallManager->collisionBoxesWalls[i].vecMax.z)
				{
					mPlayer->pos.x = mPlayer->previousPosition.x;
					mPlayer->pos.z = mPlayer->previousPosition.z;
				}
			}

			for (int i = 0; i < mKeyManager->keyColBoxesVector.size(); i++) {
				if (mPlayer->playerCollisionBox.vecMax.x >= mKeyManager->keyColBoxesVector[i].vecMin.x && mPlayer->playerCollisionBox.vecMin.x <= mKeyManager->keyColBoxesVector[i].vecMax.x &&
					mPlayer->playerCollisionBox.vecMax.y >= mKeyManager->keyColBoxesVector[i].vecMin.y && mPlayer->playerCollisionBox.vecMin.y <= mKeyManager->keyColBoxesVector[i].vecMax.y &&
					mPlayer->playerCollisionBox.vecMax.z >= mKeyManager->keyColBoxesVector[i].vecMin.z && mPlayer->playerCollisionBox.vecMin.z <= mKeyManager->keyColBoxesVector[i].vecMax.z)
				{
					mKeyManager->enabled = false;
				}

				if (mKeyManager->enabled == false)
				{
					mPlayer->keyFragments++;
					mKeyManager->keyColBoxesVector.pop_back();
					mKeyManager->keyVector.pop_back();
					mKeyManager->enabled = true;
				}
			}

			
			if (mPlayer->playerCollisionBox.vecMax.x >= exit.vecMin.x && mPlayer->playerCollisionBox.vecMin.x <= exit.vecMax.x &&
				mPlayer->playerCollisionBox.vecMax.y >= exit.vecMin.y && mPlayer->playerCollisionBox.vecMin.y <= exit.vecMax.y &&
				mPlayer->playerCollisionBox.vecMax.z >= exit.vecMin.z && mPlayer->playerCollisionBox.vecMin.z <= exit.vecMax.z && mPlayer->keyFragments == 2)
			{
				level++;
				levelLoaded2 = true;
				mComponents.clear();
			}

			//HEALTH IMPLEMENTATION + DEATH
			timer += (float)gameTime.ElapsedGameTime();

			for (int i = 0; i < mEnemyManager->enemyVector.size(); i++)
			{
				if (timer >= attackDelay && mPlayer->playerCollisionBox.vecMax.x >= mEnemyManager->enemyVector[i]->enemyCollisionBox.vecMin.x && mPlayer->playerCollisionBox.vecMin.x <= mEnemyManager->enemyVector[i]->enemyCollisionBox.vecMax.x &&
					mPlayer->playerCollisionBox.vecMax.y >= mEnemyManager->enemyVector[i]->enemyCollisionBox.vecMin.y && mPlayer->playerCollisionBox.vecMin.y <= mEnemyManager->enemyVector[i]->enemyCollisionBox.vecMax.y &&
					mPlayer->playerCollisionBox.vecMax.z >= mEnemyManager->enemyVector[i]->enemyCollisionBox.vecMin.z && mPlayer->playerCollisionBox.vecMin.z <= mEnemyManager->enemyVector[i]->enemyCollisionBox.vecMax.z && mPlayer->maxHealth > 0)
				{
					timer = 0.0f;
					mPlayer->Death();
				}
			}
	}

	


	void RenderingGame::PlayerInput(const GameTime &gameTime)
	{
		float elapsedTime = (float)gameTime.ElapsedGameTime();
		XMFLOAT3 movement = Vector3Helper::Zero;
		XMVECTOR movementRate = XMLoadFloat3(&movement) * mPlayer->moveSpeed * elapsedTime;

		if (mKeyboard != nullptr)
		{
			if (mKeyboard->IsKeyDown(DIK_LSHIFT))
			{
				mPlayer->playerState = StateManager::Run;
			}

			if (mKeyboard->IsKeyUp(DIK_LSHIFT))
			{
				mPlayer->playerState = StateManager::Walk;
			}

			if (mKeyboard->IsKeyDown(DIK_W))
			{
				mPlayer->pos.z -= mPlayer->moveSpeed * elapsedTime;
			}

			if (mKeyboard->IsKeyDown(DIK_S))
			{
				mPlayer->pos.z += mPlayer->moveSpeed * elapsedTime;
			}

			if (mKeyboard->IsKeyDown(DIK_A))
			{
				mPlayer->pos.x -= mPlayer->moveSpeed * elapsedTime;
			}

			if (mKeyboard->IsKeyDown(DIK_D))
			{
				mPlayer->pos.x += mPlayer->moveSpeed * elapsedTime;
			}

			if (mKeyboard->IsKeyDown(DIK_LEFT))
			{
				mPlayer->mAngle += XM_PI * elapsedTime;
			}

			if (mKeyboard->IsKeyDown(DIK_RIGHT))
			{
				mPlayer->mAngle -= XM_PI * elapsedTime;
			}

			if (mKeyboard->WasKeyPressedThisFrame(DIK_SPACE))
			{
				mPlayer->pos.y += 5000 * elapsedTime;

			}

			if (mKeyboard->WasKeyReleasedThisFrame(DIK_SPACE))
			{
				mPlayer->pos.y -= 5000 * elapsedTime;

			}

			mPlayer->SetPosition(0.0f, mPlayer->mAngle, -0.0f, 1.0f, mPlayer->pos.x, mPlayer->pos.y, mPlayer->pos.z);
		}
	}

	void RenderingGame::Save()
	{
		BinaryWriter writer("Save.bin");

		//Writes Everything about the Player and the Game State
		writer.WriteSingle(mPlayer->pos.x);
		writer.WriteSingle(mPlayer->pos.y);
		writer.WriteSingle(mPlayer->pos.z);
		writer.WriteInt32(mPlayer->maxHealth);
		writer.WriteInt32(mPlayer->keyFragments);

		for (int i = 0; i < mEnemyManager->enemyVector.size(); i++)
		{
			writer.WriteSingle(mEnemyManager->enemyVector[i]->pos.x);
			writer.WriteSingle(mEnemyManager->enemyVector[i]->pos.y);
			writer.WriteSingle(mEnemyManager->enemyVector[i]->pos.z);
		}		
	}

	void RenderingGame::Load()
	{
		int ReturnMessage;

		BinaryReader reader("Save.bin", &ReturnMessage);

		if (ReturnMessage != -1)
		{
			mComponents.clear();
			Initialize();

			mPlayer->pos.x = reader.ReadSingle();
			mPlayer->pos.y = reader.ReadSingle();
			mPlayer->pos.z = reader.ReadSingle();
			mPlayer->maxHealth = reader.ReadInt32();
			mPlayer->keyFragments = reader.ReadInt32();

			for (int i = 0; i < mEnemyManager->enemyVector.size(); i++)
			{
				mEnemyManager->enemyVector[i]->pos.x = reader.ReadSingle();
				mEnemyManager->enemyVector[i]->pos.y = reader.ReadSingle();
				mEnemyManager->enemyVector[i]->pos.z = reader.ReadSingle();
			}
		}
		
	}
}
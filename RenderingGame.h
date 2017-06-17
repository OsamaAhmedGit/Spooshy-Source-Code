#pragma once


#include "Game.h"
#include "Key.h"
#include "VectorHelper.h"
#include "Player.h"
#include "Wall.h"
#include "Wall2.h"
#include "WallManager.h"
#include "KeyManager.h"
#include "ExitDoor.h"
#include "EnemyManager.h"

using namespace Library;

namespace Library
{
    class FirstPersonCamera;
	class RenderStateHelper;
	class Keyboard;
	class Mouse;
}

namespace DirectX
{
	class SpriteBatch;
	class SpriteFont;
}

namespace Rendering
{
    class TriangleDemo;
	class ModelFromFile;

	class Wall;
	class Wall2;
	class Player;
	class Enemy;
	class Key;

	class TextDraw;

	class WallManager;
	class KeyManager;
	class EnemyManager;

	class Menu;

	class ExitDoor;

	class ObjectDiffuseLight;

    class RenderingGame : public Game
    {
    public:
        RenderingGame(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand);
        virtual ~RenderingGame();

        virtual void Initialize() override;		
        virtual void Update(const GameTime& gameTime) override;
        virtual void Draw(const GameTime& gameTime) override;

		void collisionDetection(const GameTime &gameTime);

		void EnemyCollision();

		bool collision;

		XMFLOAT2& TextPosition();

		float attackDelay;

		float timer;

		int level;

		ObjectDiffuseLight*mObjectDiffuseLight;

		TextDraw *hud;

		wstring textureName;

    protected:
        virtual void Shutdown() override;

    private:
		static const XMFLOAT4 BackgroundColor;
        FirstPersonCamera * mCamera;
        TriangleDemo* mGround;
		Keyboard* mKeyboard;
		Mouse* mMouse;
		LPDIRECTINPUT8 mDirectInput;
		Key* mModel;

		//Text Rendering
		RenderStateHelper* mRenderStateHelper;
		TextDraw* mTextDraw;

		Player* mPlayer;

		Enemy* mEnemy;
		
		WallManager* mWallManager;
		KeyManager* mKeyManager;
		EnemyManager* mEnemyManager;

		Menu* mMenu;

		ExitDoor* mDoor;

		bool levelLoaded1, levelLoaded2;

		int count;
		
		SpriteBatch* mSpriteBatch;
		SpriteFont* mSpriteFont;
		XMFLOAT2 mTextPosition;

		int mFrameCount;
		int mFrameRate;
		double mLastTotalElapsedTime;

		void PlayerInput(const GameTime &gameTime);

		void GameState(const GameTime &gameTime);
		int gameState;

		void Save();
		void Load();

		bool pause;

		AABB exit;
	};
}
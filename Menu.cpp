#include "Menu.h"
#include <sstream>
#include <iomanip>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include "Game.h"
#include "Utility.h"
#include "WICTextureLoader.h"
#include "GameException.h"
#include "VectorHelper.h"
#include "SimpleMath.h"
#include "MatrixHelper.h"
#include "d3dcompiler.h"
#include "d3d11.h"
#include "d3d9.h"

using namespace Library;
using namespace std;
using namespace DirectX;

namespace Rendering
{
	RTTI_DEFINITIONS(Menu)

		Menu::Menu(Game& game)
		: DrawableGameComponent(game), mSpriteBatch(nullptr), mSpriteFont(nullptr), mTextPosition(350, 20.0f)
		
	{
	}

	Menu::~Menu()
	{
		DeleteObject(mSpriteFont);
		DeleteObject(mSpriteBatch);
	}

	XMFLOAT2& Menu::TextPosition()
	{
		return mTextPosition;
	}

	void Menu::Initialize()
	{
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());
		mSpriteBatch = new SpriteBatch(mGame->Direct3DDeviceContext());
		mSpriteFont = new SpriteFont(mGame->Direct3DDevice(), L"Content\\Fonts\\Test.spritefont");
	}

	void Menu::Update(const GameTime& gameTime)
	{
	}

	void Menu::Draw(const GameTime& gameTime)
	{
		mSpriteBatch->Begin();

		std::wostringstream Title, Start, Exit, Load, Save;
		Title << std::setprecision(4) << L"SPOOSHY THE CUTE" << "\n";
		Start << std::setprecision(4) << L"START GAME -> ENTER" << "\n";
		Exit << std::setprecision(4) << L"EXIT GAME -> ESC" << "\n";
		mSpriteFont->DrawString(mSpriteBatch, Title.str().c_str(), XMFLOAT2(350, 20), Colors::Red);
		mSpriteFont->DrawString(mSpriteBatch, Start.str().c_str(), XMFLOAT2(100, 150));
		mSpriteFont->DrawString(mSpriteBatch, Exit.str().c_str(), XMFLOAT2(100, 180));

		mSpriteBatch->End();
	}
}
#include "TextDraw.h"
#include <sstream>
#include <iomanip>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include "Game.h"
#include "Utility.h"
#include "Player.h"
#include "Camera.h"
#include "RenderStateHelper.h"
#include <WICTextureLoader.h>
#include <sstream>

namespace Rendering
{
	RTTI_DEFINITIONS(TextDraw)

		TextDraw::TextDraw(Game& game, Camera& camera)
		: DrawableGameComponent(game, camera), mSpriteBatch(nullptr), mSpriteFont(nullptr), mTextPosition(0, 40.0f),
		mFrameCount(0), mFrameRate(0), mLastTotalElapsedTime(0.0), hp(0), mRenderStateHelper(nullptr), mIndexCount(0)
	{
	}

	TextDraw::~TextDraw()
	{
		DeleteObject(mSpriteFont);
		DeleteObject(mSpriteBatch);
		DeleteObject(mRenderStateHelper);
	}

	void TextDraw::SetPosition(const float rotateX, const float rotateY, const float rotateZ, const float scaleFactor, const float translateX, const float translateY, const float translateZ)
	{

		XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		XMMATRIX RotationZ = XMMatrixRotationZ(rotateZ);
		XMMATRIX RotationX = XMMatrixRotationX(rotateX);
		XMMATRIX RotationY = XMMatrixRotationY(rotateY);
		XMMATRIX Scale = XMMatrixScaling(scaleFactor, scaleFactor, scaleFactor);
		XMMATRIX Translation = XMMatrixTranslation(translateX, translateY, translateZ);
		worldMatrix = RotationZ*RotationX *RotationY* Scale * Translation;

		XMStoreFloat4x4(&mWorldMatrix, worldMatrix);
	}

	void TextDraw::Initialize()
	{
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		mSpriteBatch = new SpriteBatch(mGame->Direct3DDeviceContext());
		mSpriteFont = new SpriteFont(mGame->Direct3DDevice(), L"Content\\Fonts\\Arial_14_Regular.spritefont");

		mRenderStateHelper = new RenderStateHelper(*mGame);
	}

	void TextDraw::Update(const GameTime& gameTime)
	{
	}

	void TextDraw::Draw(const GameTime& gameTime, int hp, int keyFragments)
	{
		mRenderStateHelper->SaveAll();

		mSpriteBatch->Begin();

		std::wostringstream fpsLabel;
		fpsLabel << L"Health: " << hp << "\n";
		fpsLabel << L"Key Fragments: " << keyFragments << "\n";
		mSpriteFont->DrawString(mSpriteBatch, fpsLabel.str().c_str(), mTextPosition);

		mSpriteBatch->End();

		mRenderStateHelper->RestoreAll();
	}
}
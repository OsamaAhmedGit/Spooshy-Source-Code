#pragma once

#include "DrawableGameComponent.h"

#include "Player.h"

#include "RenderStateHelper.h"

using namespace Library;

namespace Library
{
	class RenderStateHelper;
}

namespace DirectX
{
	class SpriteBatch;
	class SpriteFont;
}

namespace Rendering
{

	class TextDraw : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(TextDraw, DrawableGameComponent)

	public:
		TextDraw(Game& game, Camera& camera);
		~TextDraw();

		XMFLOAT2& TextPosition();
		int FrameRate();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime, int hp, int keyFragments);

		void SetPosition(const float rotateX, const float rotateY, const float rotateZ, const float scaleFactor, const float translateX, const float translateY, const float translateZ);

		XMFLOAT4X4 mWorldMatrix;

		UINT mIndexCount;

	private:
		TextDraw();
		TextDraw(const TextDraw& rhs);
		TextDraw& operator=(const TextDraw& rhs);

		SpriteBatch* mSpriteBatch;
		SpriteFont* mSpriteFont;
		XMFLOAT2 mTextPosition;

		int mFrameCount;
		int mFrameRate;
		double mLastTotalElapsedTime;

		int hp;

		int keyFragments;

		RenderStateHelper* mRenderStateHelper;
	};
}
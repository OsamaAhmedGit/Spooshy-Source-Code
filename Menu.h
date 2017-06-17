#pragma once

#include "DrawableGameComponent.h"
#include "d3dx11effect.h"
#include "d3d9.h"

using namespace Library;

namespace DirectX
{
	class SpriteBatch;
	class SpriteFont;
}

namespace Rendering
{

	class Menu : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Menu, DrawableGameComponent)

	public:
		Menu(Game& game);
		~Menu();

		XMFLOAT2& TextPosition();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		Menu();
		Menu(const Menu& rhs);
		Menu& operator=(const Menu& rhs);

		SpriteBatch* mSpriteBatch;
		SpriteFont* mSpriteFont;
		XMFLOAT2 mTextPosition;

		LPDIRECT3DTEXTURE9 tex;
		XMFLOAT3 pos;
		D3DCOLOR color;
	};
}
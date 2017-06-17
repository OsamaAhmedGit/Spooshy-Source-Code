#pragma once

#include "DrawableGameComponent.h"
#include "Singleton.h"

using namespace Library;

namespace Rendering
{
	class Wall : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Wall, DrawableGameComponent)

	public:
		Wall(Game& game, Camera& camera);
		~Wall();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

		/*OUTER WALLS*/
		virtual void SetPosition(FLOAT x1, FLOAT x2, FLOAT x3, FLOAT x4, 
								FLOAT y1, FLOAT y2, 
								FLOAT z1, FLOAT z2, FLOAT z3, FLOAT z4);

		/*LEVEL WALLS*/
		virtual void SetPosition(FLOAT x1, FLOAT x2, FLOAT x3, FLOAT x4, FLOAT x5, FLOAT x6, FLOAT x7, FLOAT x8,
			FLOAT y1, FLOAT y2, FLOAT y3, FLOAT y4, FLOAT y5, FLOAT y6, FLOAT y7, FLOAT y8,
			FLOAT z1, FLOAT z2, FLOAT z3, FLOAT z4, FLOAT z5, FLOAT z6, FLOAT z7, FLOAT z8);

		/*WALL VERTICES*/
		FLOAT _x1, _x2, _x3, _x4, _x5, _x6, _x7, _x8,
			  _y1, _y2, _y3, _y4, _y5, _y6, _y7, _y8,
			  _z1, _z2, _z3, _z4, _z5, _z6, _z7, _z8;

	private:
		typedef struct _TextureMappingVertex
		{
			XMFLOAT4 Position;
			XMFLOAT2 TextureCoordinates;
			_TextureMappingVertex() { }
			_TextureMappingVertex(XMFLOAT4 position, XMFLOAT2 textureCoordinates)
				: Position(position), TextureCoordinates(textureCoordinates) { }
		} TextureMappingVertex;

		Wall();
		Wall(const Wall& rhs);
		Wall& operator=(const Wall& rhs);

		ID3DX11Effect* mEffect;
		ID3DX11EffectTechnique* mTechnique;
		ID3DX11EffectPass* mPass;
		ID3DX11EffectMatrixVariable* mWvpVariable;

		ID3D11InputLayout* mInputLayout;
		ID3D11Buffer* mVertexBuffer;

		ID3D11Buffer* mIndexBuffer;

		ID3D11ShaderResourceView* mTextureShaderResourceView;
		ID3DX11EffectShaderResourceVariable* mColorTextureVariable;

		XMFLOAT4X4 mWorldMatrix;
	};
}

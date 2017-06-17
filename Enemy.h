#pragma once

#include "DrawableGameComponent.h"
#include "VectorHelper.h"
#include "Globals.h"
#include <vector>

using namespace Library;

namespace Library
{
	class Mesh;
}

namespace Rendering
{
	class Enemy : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Enemy, DrawableGameComponent)

	public:
		Enemy(Game& game, Camera& camera, const std::string modelFilename);
		~Enemy();

		virtual void Update(const GameTime& gameTime) override;

		static const float DefaultMovementRate;

		//add to support multiple model in the scene, remove this function
		XMFLOAT3 mPosition;
		XMFLOAT3 mDirection;

		XMFLOAT3 mRight;

		XMFLOAT3 pos;

		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;

		void SetPosition(const float rotateX, const float rotateY, const float rotateZ, const float scaleFactor, const float translateX, const float translateY, const float translateZ);

		float mAngle;

		AABB enemyCollisionBox;

		float moveSpeed;

		void Patrol(const GameTime &gameTime);

		bool left, right;

	protected:
		float mMovementRate;

	private:
		typedef struct _TextureMappingVertex
		{
			XMFLOAT4 Position;
			XMFLOAT2 TextureCoordinates;

			_TextureMappingVertex() { }

			_TextureMappingVertex(XMFLOAT4 position, XMFLOAT2 textureCoordinates)
				: Position(position), TextureCoordinates(textureCoordinates) { }
		} TextureMappingVertex;

		Enemy();
		Enemy(const Enemy& rhs);
		Enemy& operator=(const Enemy& rhs);

		void CreateVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const;

		ID3DX11Effect* mEffect;
		ID3DX11EffectTechnique* mTechnique;
		ID3DX11EffectPass* mPass;
		ID3DX11EffectMatrixVariable* mWvpVariable;

		ID3D11ShaderResourceView* mTextureShaderResourceView;
		ID3DX11EffectShaderResourceVariable* mColorTextureVariable;

		ID3D11InputLayout* mInputLayout;
		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
		UINT mIndexCount;

		XMFLOAT4X4 mWorldMatrix;

		const std::string modelFile;
	};
}
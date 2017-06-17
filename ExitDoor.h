#pragma once

#include "DrawableGameComponent.h"
#include "VectorHelper.h"

using namespace Library;

namespace Library
{
	class Mesh;
}

namespace Rendering
{
	class ExitDoor : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(ExitDoor, DrawableGameComponent)

	public:
		ExitDoor(Game& game, Camera& camera, const std::string modelFilename);
		~ExitDoor();

		virtual void Update(const GameTime& gameTime) override;

		static const float DefaultMovementRate;

		//add to support multiple model in the scene, remove this function
		XMFLOAT3 mPosition;
		XMFLOAT3 mDirection;

		XMFLOAT3 mRight;

		XMFLOAT3 pos = Vector3Helper::Zero;

		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;

		void SetPosition(const float rotateX, const float rotateY, const float rotateZ, const float scaleFactor, const float translateX, const float translateY, const float translateZ);

		float mAngle;

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

		ExitDoor();
		ExitDoor(const ExitDoor& rhs);
		ExitDoor& operator=(const ExitDoor& rhs);

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

		float speed;

		const std::string modelFile;
	};
}
#pragma once

#include "DrawableGameComponent.h"
#include "VectorHelper.h"
#include "Globals.h"
#include "WallManager.h"
#include "DirectXMath.h"


using namespace Library;


namespace Library
{
	class Keyboard;
	class Mouse;
}

namespace Rendering
{
	class Player : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Player, DrawableGameComponent)

	public:
		Player(Game& game, Camera& camera);
		~Player();

		virtual void Initialize(int level);
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

		const Keyboard& GetKeyboard() const;
		void SetKeyboard(Keyboard& keyboard);

		const Mouse& GetMouse() const;
		void SetMouse(Mouse& mouse);

		float& MouseSensitivity();
		float& RotationRate();

		void ApplyRotation(float angle);
		XMFLOAT3 mUp;

		static const float DefaultMouseSensitivity;
		static const float DefaultRotationRate;

		void SetPosition(float rotateX, float rotateY, const float rotateZ, const float scaleFactor, const float translateX, const float translateY, const float translateZ);
		virtual void SetPosition(FLOAT x, FLOAT y, FLOAT z);
		virtual void SetPosition(FXMVECTOR position);
		virtual void SetPosition(const XMFLOAT3& position);

		void Death();

		XMFLOAT3 pos = Vector3Helper::Zero;

		XMFLOAT3 previousPosition;

		XMFLOAT3 mPosition;
		XMFLOAT3 mDirection;
		XMFLOAT3 mRight;

		/*const XMFLOAT3& Position() const;
		const XMFLOAT3& Direction() const;
		XMVECTOR PositionVector() const;
		XMVECTOR DirectionVector() const;
		const XMFLOAT3& Right() const;*/

		float moveSpeed;

		float width;
		float length;
		float height;

		float maxX;
		float minX;
		float maxZ;
		float minZ;

		AABB playerCollisionBox;

		int maxHealth, currentHealth;

		float attackDelay;

		float timer;

		int keyFragments;

		float mAngle;
		int playerState;

		XMFLOAT4X4 mWorldMatrix;

		void PlayerState(int state);

	protected:
		Keyboard* mKeyboard;
		Mouse* mMouse;

		float mMouseSensitivity;
		float mRotationRate;

	private:
		typedef struct _BasicEffectVertex
		{
			XMFLOAT4 Position;
			XMFLOAT4 Color;

			_BasicEffectVertex() { }

			_BasicEffectVertex(XMFLOAT4 position, XMFLOAT4 color)
				: Position(position), Color(color) { }
		} BasicEffectVertex;

		Player();
		Player(const Player& rhs);
		Player& operator=(const Player& rhs);

		ID3DX11Effect* mEffect;
		ID3DX11EffectTechnique* mTechnique;
		ID3DX11EffectPass* mPass;
		ID3DX11EffectMatrixVariable* mWvpVariable;

		ID3D11InputLayout* mInputLayout;
		ID3D11Buffer* mVertexBuffer;

		ID3D11Buffer* mIndexBuffer;
	};
}

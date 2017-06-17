#include "Player.h"
#include "Game.h"
#include "GameException.h"
#include "MatrixHelper.h"
#include "Camera.h"
#include "Utility.h"
#include "D3DCompiler.h"
#include "Model.h"
#include "Mesh.h"
#include <WICTextureLoader.h>
#include "VectorHelper.h"
#include "Camera.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "RenderingGame.h"
#include "Globals.h"
#include "DirectXMath.h"
#include "StateManager.h"

using namespace Library;
using namespace DirectX;

namespace Rendering
{
	RTTI_DEFINITIONS(Player)

	const float Player::DefaultRotationRate = XMConvertToRadians(1.0f);
	const float Player::DefaultMouseSensitivity = 850.0f;

	Player::Player(Game& game, Camera& camera)
		: DrawableGameComponent(game, camera),
		mEffect(nullptr), mTechnique(nullptr), mPass(nullptr), mWvpVariable(nullptr), mAngle(0.0f),
		mInputLayout(nullptr), mWorldMatrix(MatrixHelper::Identity), mVertexBuffer(nullptr), mIndexBuffer(nullptr), mKeyboard(nullptr), mMouse(nullptr), mPosition(0, 0, 0), maxHealth(100), keyFragments(0)
	{

	}

	Player::~Player()
	{
		mKeyboard = nullptr;
		mMouse = nullptr;

		ReleaseObject(mWvpVariable);
		ReleaseObject(mPass);
		ReleaseObject(mTechnique);
		ReleaseObject(mEffect);
		ReleaseObject(mInputLayout);
		ReleaseObject(mVertexBuffer);
		ReleaseObject(mIndexBuffer);
	}

	const Keyboard& Player::GetKeyboard() const
	{
		return *mKeyboard;
	}

	void Player::SetKeyboard(Keyboard& keyboard)
	{
		mKeyboard = &keyboard;
	}

	void Player::SetPosition(FLOAT x, FLOAT y, FLOAT z)
	{
		XMVECTOR position = XMVectorSet(x, y, z, 1.0f);
		SetPosition(position);
	}

	void Player::SetPosition(FXMVECTOR position)
	{
		XMStoreFloat3(&mPosition, position);
	}

	void Player::SetPosition(const XMFLOAT3& position)
	{
		mPosition = position;
	}

	const Mouse& Player::GetMouse() const
	{
		return *mMouse;
	}

	void Player::SetMouse(Mouse& mouse)
	{
		mMouse = &mouse;
	}

	float&Player::MouseSensitivity()
	{
		return mMouseSensitivity;
	}


	float& Player::RotationRate()
	{
		return mRotationRate;
	}

	void Player::SetPosition(float rotateX, float rotateY, const float rotateZ, const float scaleFactor, const float translateX, const float translateY, const float translateZ)
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

	void Player::Initialize(int level)
	{
		mKeyboard = (Keyboard*)mGame->Services().GetService(Keyboard::TypeIdClass());
		mMouse = (Mouse*)mGame->Services().GetService(Mouse::TypeIdClass());

		if (level == 1) {
			pos.x = -180.0f;
			pos.y = 1.0f;
			pos.z = 180.0f;
		}

		if (level == 2) {
			pos.x = 0.0f;
			pos.y = 1.0f;
			pos.z = 180.0f;
		}

		if (level == 3) {
			pos.x = -150.0f;
			pos.y = 1.0f;
			pos.z = 175.0f;
		}

		length = 2.0f;
		width = 1.0f;
		height = 1.0f;

		attackDelay = 0.5f;

		moveSpeed = 35.0f;

		timer = 0.0f;

		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		// Compile the shader
		UINT shaderFlags = 0;

#if defined( DEBUG ) || defined( _DEBUG )
		shaderFlags |= D3DCOMPILE_DEBUG;
		shaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		ID3D10Blob* compiledShader = nullptr;
		ID3D10Blob* errorMessages = nullptr;

		//1. load the effect file (vertex and pixel shader)
		//insert code here

		HRESULT hr = D3DCompileFromFile(L"Content\\Effects\\BasicEffect.fx", nullptr, nullptr, nullptr, "fx_5_0", shaderFlags, 0, &compiledShader, &errorMessages);
		if (FAILED(hr))
		{
			char* errorMessage = (errorMessages != nullptr ? (char*)errorMessages->GetBufferPointer() : "D3DX11CompileFromFile() failed");
			GameException ex(errorMessage, hr);
			ReleaseObject(errorMessages);
			throw ex;
		}

		// Create an effect object from the compiled shader
		hr = D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), 0, mGame->Direct3DDevice(), &mEffect);
		if (FAILED(hr))
		{
			throw GameException("D3DX11CreateEffectFromMemory() failed.", hr);
		}



		ReleaseObject(compiledShader);

		// Look up the technique, pass, and WVP variable from the effect
		mTechnique = mEffect->GetTechniqueByName("main11");
		if (mTechnique == nullptr)
		{
			throw GameException("ID3DX11Effect::GetTechniqueByName() could not find the specified technique.", hr);
		}

		mPass = mTechnique->GetPassByName("p0");
		if (mPass == nullptr)
		{
			throw GameException("ID3DX11EffectTechnique::GetPassByName() could not find the specified pass.", hr);
		}

		ID3DX11EffectVariable* variable = mEffect->GetVariableByName("WorldViewProjection");
		if (variable == nullptr)
		{
			throw GameException("ID3DX11Effect::GetVariableByName() could not find the specified variable.", hr);
		}

		mWvpVariable = variable->AsMatrix();
		if (mWvpVariable->IsValid() == false)
		{
			throw GameException("Invalid effect variable cast.");
		}

		// Create the input layout
		D3DX11_PASS_DESC passDesc;
		mPass->GetDesc(&passDesc);

		//2. create the vetex layout
		//   insert code here
		D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		if (FAILED(hr = mGame->Direct3DDevice()->CreateInputLayout(inputElementDescriptions, ARRAYSIZE(inputElementDescriptions), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &mInputLayout)))
		{
			throw GameException("ID3D11Device::CreateInputLayout() failed.", hr);
		}

		BasicEffectVertex vertices[] =
		{
			BasicEffectVertex(XMFLOAT4(-width, 0.0f, length, 1.0f), XMFLOAT4(1.0f, 1.0f,1.0f,1.0f)),//V0
			BasicEffectVertex(XMFLOAT4(width, 0.0f, length, 1.0f), XMFLOAT4(1.0f, 1.0f,1.0f,1.0f)),//V1
			BasicEffectVertex(XMFLOAT4(width, 0.0f, -length, 1.0f), XMFLOAT4(1.0f, 1.0f,1.0f,1.0f)),//V2
			BasicEffectVertex(XMFLOAT4(-width, 0.0f, -length, 1.0f), XMFLOAT4(1.0f, 1.0f,1.0f,1.0f)),//V3

			BasicEffectVertex(XMFLOAT4(-width, height, length, 1.0f), XMFLOAT4(1.0f, 1.0f,1.0f,1.0f)),//V4
			BasicEffectVertex(XMFLOAT4(width, height, length, 1.0f), XMFLOAT4(1.0f, 1.0f,1.0f,1.0f)),//V5
			BasicEffectVertex(XMFLOAT4(width, height, -length, 1.0f), XMFLOAT4(1.0f, 1.0f,1.0f,1.0f)),//V6
			BasicEffectVertex(XMFLOAT4(-width, height, -length, 1.0f), XMFLOAT4(1.0f, 1.0f,1.0f,1.0f)),//V7
		};


		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.ByteWidth = sizeof(BasicEffectVertex) * ARRAYSIZE(vertices);
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		D3D11_SUBRESOURCE_DATA vertexSubResourceData;
		ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
		vertexSubResourceData.pSysMem = vertices;
		if (FAILED(mGame->Direct3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &mVertexBuffer)))
		{
			throw GameException("ID3D11Device::CreateBuffer() failed.");
		}

		UINT indices[] =
		{ 
			//Bottom Face
			3,0,1,
			3,2,1,

			//Top Face
			7,4,5,
			7,6,5,

			//Left Face
			0,4,7,
			0,3,7,

			//Right Face
			2,6,5,
			2,1,5,

			//Back Face
			3,7,6,
			3,2,6,

			//Front Face
			0,4,5,
			0,1,5,
		};
		D3D11_BUFFER_DESC indexBufferDesc;
		ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
		indexBufferDesc.ByteWidth = sizeof(UINT)* ARRAYSIZE(indices);
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		D3D11_SUBRESOURCE_DATA indexSubResourceData;
		ZeroMemory(&indexSubResourceData, sizeof(indexSubResourceData));
		indexSubResourceData.pSysMem = indices;
		if (FAILED(mGame->Direct3DDevice()->CreateBuffer(&indexBufferDesc, &indexSubResourceData, &mIndexBuffer)))
		{
			throw GameException("ID3D11Device::CreateBuffer() failed.");
		}
	}

	void Player::Update(const GameTime& gameTime)
	{
		playerCollisionBox.vecMax = { pos.x + (float)1, pos.y + (float)2, pos.z + (float)1 };
		playerCollisionBox.vecMin = { pos.x - (float)1, pos.y, pos.z - (float)1 };

		PlayerState(playerState);
	}

	void Player::ApplyRotation(float angle)
	{
		XMStoreFloat4x4(&mWorldMatrix, XMMatrixRotationY(angle));
	}

	void Player::Draw(const GameTime& gameTime)
	{
		//4. draw function
		//insert the code here
		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		direct3DDeviceContext->IASetInputLayout(mInputLayout);
		UINT stride = sizeof(BasicEffectVertex);
		UINT offset = 0;

		direct3DDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
		direct3DDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		XMMATRIX wvp = worldMatrix * mCamera->ViewMatrix() * mCamera->ProjectionMatrix();
		mWvpVariable->SetMatrix(reinterpret_cast<const float*>(&wvp));
		mPass->Apply(0, direct3DDeviceContext);
		direct3DDeviceContext->DrawIndexed(36, 0, 0);
	}

	void Player::Death()
	{
		if (maxHealth > 0)
		{
			maxHealth -= 10;
		}
	}

	void Player::PlayerState(int state)
	{
		switch (playerState)
		{
		case StateManager::Walk:
			moveSpeed = 35;
			break;

		case StateManager::Run:
			moveSpeed = 70;
			break;
		}
	}
}

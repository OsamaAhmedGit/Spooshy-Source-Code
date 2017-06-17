#include "Wall2.h"
#include "Game.h"
#include "GameException.h"
#include "MatrixHelper.h"
#include "Camera.h"
#include "Utility.h"
#include "D3DCompiler.h"
#include "WICTextureLoader.h"

namespace Rendering
{
	RTTI_DEFINITIONS(Wall2)

		Wall2::Wall2(Game& game, Camera& camera, wstring TextureName)
		: DrawableGameComponent(game, camera),
		mEffect(nullptr), mTechnique(nullptr), mPass(nullptr), mWvpVariable(nullptr),
		mInputLayout(nullptr), mWorldMatrix(MatrixHelper::Identity), mVertexBuffer(nullptr), mAngle(0.0f), mIndexBuffer(nullptr), mTextureShaderResourceView(nullptr), mColorTextureVariable(nullptr)
	{
		textureName = TextureName;
	}

	Wall2::~Wall2()
	{
		ReleaseObject(mWvpVariable);
		ReleaseObject(mPass);
		ReleaseObject(mTechnique);
		ReleaseObject(mTextureShaderResourceView);
		ReleaseObject(mColorTextureVariable);
		ReleaseObject(mEffect);
		ReleaseObject(mInputLayout);
		ReleaseObject(mVertexBuffer);
	}

	void Wall2::Initialize()
	{
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

		HRESULT hr = D3DCompileFromFile(L"Content\\Effects\\TextureMapping.fx", nullptr, nullptr, nullptr, "fx_5_0", shaderFlags, 0, &compiledShader, &errorMessages);
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


		variable = mEffect->GetVariableByName("ColorTexture");
		if (variable == nullptr)
		{
			throw GameException("ID3DX11Effect::GetVariableByName() could not find the specified variable.", hr);
		}
		mColorTextureVariable = variable->AsShaderResource();
		if (mColorTextureVariable->IsValid() == false)
		{
			throw GameException("Invalid effect variable cast.");
		}
		// Create the input layout
		D3DX11_PASS_DESC passDesc;
		mPass->GetDesc(&passDesc);

		//2. create the vetex layout
		//insert code here
		D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
		{
			//{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			//{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		if (FAILED(hr = mGame->Direct3DDevice()->CreateInputLayout(inputElementDescriptions, ARRAYSIZE(inputElementDescriptions), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &mInputLayout)))
		{
			throw GameException("ID3D11Device::CreateInputLayout() failed.", hr);
		}

		// 3. Create the vertex buffer
		//insert code here


		TextureMappingVertex vertices[] =
		{
			TextureMappingVertex(XMFLOAT4(_x1, _y1, _z1, 1.0f), XMFLOAT2(0.0f, 1.0f)),//Vertex 0
			TextureMappingVertex(XMFLOAT4(_x2, _y2, _z2, 1.0f), XMFLOAT2(1.0f, 1.0f)),//Vertex 1
			TextureMappingVertex(XMFLOAT4(_x3, _y3, _z3, 1.0f), XMFLOAT2(0.0f, 1.0f)),//Vertex 2
			TextureMappingVertex(XMFLOAT4(_x4, _y4, _z4, 1.0f), XMFLOAT2(1.0f, 1.0f)),//Vertex 3

			TextureMappingVertex(XMFLOAT4(_x5, _y5, _z5, 1.0f), XMFLOAT2(0.0f, 0.0f)),//Vertex 4
			TextureMappingVertex(XMFLOAT4(_x6, _y6, _z6, 1.0f), XMFLOAT2(1.0f, 0.0f)),//Vertex 5
			TextureMappingVertex(XMFLOAT4(_x7, _y7, _z7, 1.0f), XMFLOAT2(0.0f, 0.0f)),//Vertex 6
			TextureMappingVertex(XMFLOAT4(_x8, _y8, _z8, 1.0f), XMFLOAT2(1.0f, 0.0f)),//Vertex 7
		};


		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.ByteWidth = sizeof(TextureMappingVertex) * ARRAYSIZE(vertices);
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
			//Bot
			0,3,2,
			0,1,2,

			//Top
			4,7,6,
			4,5,6,

			//Left
			0,4,7,
			0,3,7,

			//Right
			1,2,6,
			1,5,6,

			//Front
			0,4,5,
			0,1,5,

			//Back
			2,3,7,
			2,6,7,
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
		//Loads Texture Map
		/*std::wstring textureName = L"Content\\Textures\\wall.jpg";*/

		if (FAILED(hr = DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), textureName.c_str(), nullptr, &mTextureShaderResourceView)))
		{
			throw GameException("CreateWICTextureFromFile() failed.", hr);
		}
	}

	void Wall2::Update(const GameTime& gameTime)
	{
		mAngle += XM_PI * static_cast<float>(gameTime.ElapsedGameTime() / 2);
		//XMStoreFloat4x4(&mWorldMatrix, XMMatrixRotationY(mAngle));

	}

	void Wall2::Draw(const GameTime& gameTime)
	{
		//4. draw function
		//insert the code here
		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		direct3DDeviceContext->IASetInputLayout(mInputLayout);
		UINT stride = sizeof(TextureMappingVertex);
		UINT offset = 0;

		direct3DDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
		direct3DDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		//Load Texture into Rendering Pipeline
		mColorTextureVariable->SetResource(mTextureShaderResourceView);

		XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		XMMATRIX wvp = worldMatrix * mCamera->ViewMatrix() * mCamera->ProjectionMatrix();
		mWvpVariable->SetMatrix(reinterpret_cast<const float*>(&wvp));
		mPass->Apply(0, direct3DDeviceContext);
		direct3DDeviceContext->DrawIndexed(36, 0, 0);
	}

	void Wall2::SetPosition(FLOAT x1, FLOAT x2, FLOAT x3, FLOAT x4, FLOAT x5, FLOAT x6, FLOAT x7, FLOAT x8,
		FLOAT y1, FLOAT y2, FLOAT y3, FLOAT y4, FLOAT y5, FLOAT y6, FLOAT y7, FLOAT y8,
		FLOAT z1, FLOAT z2, FLOAT z3, FLOAT z4, FLOAT z5, FLOAT z6, FLOAT z7, FLOAT z8)
	{
		_x1 = x1;
		_x2 = x2;
		_x3 = x3;
		_x4 = x4;
		_x5 = x5;
		_x6 = x6;
		_x7 = x7;
		_x8 = x8;

		_y1 = y1;
		_y2 = y2;
		_y3 = y3;
		_y4 = y4;
		_y5 = y5;
		_y6 = y6;
		_y7 = y7;
		_y8 = y8;

		_z1 = z1;
		_z2 = z2;
		_z3 = z3;
		_z4 = z4;
		_z5 = z5;
		_z6 = z6;
		_z7 = z7;
		_z8 = z8;
	}





}
#pragma once

#include "DrawableGameComponent.h"
#include "Globals.h"
#include "d3dx11effect.h"
#include <vector>
#include <iostream>

using namespace Library;
using namespace std;

namespace Rendering
{
	class Key;

	class KeyManager : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(KeyManager, DrawableGameComponent)

	public:
		KeyManager(Game& game, Camera& camera);
		virtual ~KeyManager();

		Key *key1, *key2;
		vector <Key*> keyVector;

		AABB keyColBoxes;
		vector <AABB> keyColBoxesVector;

		virtual void Initialize(int level);
		virtual void Update(const GameTime &gameTime);

		bool enabled;
	};
}

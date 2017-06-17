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
	class Wall;
	class Wall2;

	class WallManager : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(WallManager, DrawableGameComponent)

	public:
		WallManager(Game& game, Camera& camera);
		~WallManager();

		virtual void Initialize(int level);

		/*Outer Walls*/
		Wall *mOuterWalls;
		vector <Wall*> outerWallsVec;

		Wall2 *mLevelWalls;
		vector <Wall2*> levelWallsVec;
		
		AABB outerWallsColBoxes, levelWallColBoxes;
		vector <AABB> collisionBoxesWalls;

		wstring textureName;

	private:

		float _x1, _x2, _x3, _x4, _y1, _y2, _z1, _z2, _z3, _z4;
	};
}

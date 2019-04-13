#pragma once
#include "Map.h"
#include "HAPIWrapper.h"

Map tempMap;// temp map just used to gain some functionality for map nothing stored
		   // so far weapon feed in the data and a temp vector it will return an vector of places in range this will be rendered in battle
          // thinking weapon will be attached to entity then it can store the vector unique to that entity
class Weapon
{
private:

public:
	Weapon();
	~Weapon();
	void showCannonRange(std::vector<std::pair<int, int >>&highLightPos,std::pair<int, int>enityPos, int gunRange, eDirection aimDirection);// basic cannon shot
};


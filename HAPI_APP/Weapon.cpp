#include "Weapon.h"



Weapon::Weapon()
{

}


Weapon::~Weapon()
{
}

void Weapon::showCannonRange(std::vector <std::pair<int,int>> &highLightPos,std::pair<int,int>enityPos,int gunRange,eDirection aimDirection)
{
	auto tempStorage = tempMap.getTileCone(enityPos,gunRange,aimDirection);
	for (auto i : tempStorage)
	{
		highLightPos.push_back(i->m_tileCoordinate);
		
	}

}

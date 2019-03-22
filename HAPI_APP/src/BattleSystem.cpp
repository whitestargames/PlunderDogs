#include "BattleSystem.h"
#include "Utilities/MapParser.h"
#include "Utilities/Utilities.h"
#include "entity.h"
////////////////////////////////////////////////////////
//move all code out of main into here
// build stage to here to place an entity 
//set move stage 
///////////////////////////////////////////////////////

using namespace HAPISPACE;

BattleSystem::BattleSystem() : 
	m_map(MapParser::parseMap(Utilities::getDataDirectory() + "Level1.tmx")),
	UIWind(),
	entityPositionInVector(0),
	coord(std::pair<int, int>(0, 0))
{
	Entity* testShip = new Entity(Utilities::getDataDirectory() + "mouseCrossHair.xml");
	Entity* testShip2 = new Entity(Utilities::getDataDirectory() + "thingy.xml");
	m_entities.push_back(std::pair<Entity*, std::pair<int, int> >(testShip, std::pair<int, int>(4, 4)));
	m_entities.push_back(std::pair<Entity*, std::pair<int, int> >(testShip2, std::pair<int, int>(5, 5)));


	for (auto i : m_entities)
	{
		m_map.insertEntity(i.first, i.second);
	}
	
}


BattleSystem::~BattleSystem()
{
	for (auto it : m_entities)
	{
		delete it.first;
	}
	m_entities.clear();
}

void BattleSystem::update()
{
	std::pair<float,float> tempTileLocation;
	
	SCREEN_SURFACE->Clear();
		
	m_map.drawMap();
	UIWind.Update();
	


	for (int i = 0; i < m_map.getMap()->size(); i++) // temp these 2 vectors not gonna be public had to get test working 
	{
		int y = i / m_map.getDimensions().first;
		UIWind.HandleCollision(UIWind.storage[0], m_map.getTile(std::pair<int, int>(i, y))->m_sprite);
		tempTileLocation = std::pair<float, float>(m_map.getMap()->data()[i].m_sprite->GetTransformComp().GetPosition().x, 
			m_map.getMap()->data()[i].m_sprite->GetTransformComp().GetPosition().y);
		
		if (tempTileLocation == UIWind.tilePos)
		{
				coord = m_map.getMap()->data()[i].m_tileCoordinate;
		}
			
		for (int j = 0; j < m_entities.size(); j++)
		{
			if (m_entities[j].second == coord)
			{
				entityPositionInVector = j;
			}
		}
			
		if (m_map.moveEntity(std::pair<int, int>(m_entities[entityPositionInVector].second), coord))
		{
			m_entities[entityPositionInVector].second = coord;
		}

		if (m_map.getMap()->data()[i].m_entityOnTile != nullptr)
		{
			HAPISPACE::VectorF tempVec{ m_map.getMap()->data()[i].m_sprite->GetTransformComp().GetPosition().x + 30, m_map.getMap()->data()[i].m_sprite->GetTransformComp().GetPosition().y + 40 };
			m_map.getMap()->data()[i].m_entityOnTile->getSprite().GetTransformComp().SetPosition(tempVec);
			m_map.getMap()->data()[i].m_entityOnTile->render();
		}
			
	}
	
}

void BattleSystem::run()
{
	while (HAPI_Sprites.Update()) //Why are there two while loops nested! (Here and one in update)
	{
		update();
	}
}
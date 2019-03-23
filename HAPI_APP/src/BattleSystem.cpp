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
	m_entities(),
	m_map(MapParser::parseMap(Utilities::getDataDirectory() + "Level1.tmx")),
	UIWind(),
	coord(std::pair<int, int>(0, 0))
{
	m_entities.emplace_back(std::pair<Entity, 
		std::pair<int, int>>((Utilities::getDataDirectory() + "mouseCrossHair.xml"), std::pair<int, int>(4, 4)));
	m_entities.emplace_back(std::pair<Entity, 
		std::pair<int, int>>((Utilities::getDataDirectory() + "thingy.xml"), std::pair<int, int>(5, 5)));
	
	for (auto& i : m_entities)
	{
		m_map.insertEntity(i.first, i.second);
	}
}

void BattleSystem::run()
{
	while (HAPI_Sprites.Update()) 
	{
		update();
		render();
	}
}

void BattleSystem::render()
{
	SCREEN_SURFACE->Clear();

	m_map.drawMap();
	UIWind.Update();
}

void BattleSystem::update()
{
	std::pair<float,float> tempTileLocation;
	int entityPositionInVector = 0;

	
	for (int y = 0; y < m_map.getDimensions().second; y++) //Iterating through the whole map 
	{
		for (int x = 0; x < m_map.getDimensions().first; x++)
		{
			std::pair<int, int> currentTile = std::pair<int, int>(x, y);
			UIWind.HandleCollision(UIWind.storage[0], m_map.getTile(currentTile)->m_sprite);
			tempTileLocation = std::pair<float, float>(m_map.getTile(currentTile)->m_sprite->GetTransformComp().GetPosition().x,
				m_map.getTile(currentTile)->m_sprite->GetTransformComp().GetPosition().y);

			if (tempTileLocation == UIWind.tilePos)
			{
				coord = m_map.getTile(currentTile)->m_tileCoordinate;
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

			if (m_map.getTile(currentTile)->m_entityOnTile) //This is a horrible way to do this, use m_map.getTilePos() to iterate through the array instead. Also it should've been commented. - Tristan
			{
				HAPISPACE::VectorF tempVec{ m_map.getTile(currentTile)->m_sprite->GetTransformComp().GetPosition().x + 30, m_map.getTile(currentTile)->m_sprite->GetTransformComp().GetPosition().y + 40 };
				m_map.getTile(currentTile)->m_entityOnTile->m_sprite->GetTransformComp().SetPosition(tempVec);
				m_map.getTile(currentTile)->m_entityOnTile->m_sprite->Render(SCREEN_SURFACE);
			}
		}

	}
}
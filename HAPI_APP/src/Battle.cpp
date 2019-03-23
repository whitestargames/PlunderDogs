#include "Battle.h"
#include "Utilities/MapParser.h"
#include "Utilities/Utilities.h"
#include "entity.h"

using namespace HAPISPACE;

Battle::Battle() : 
	m_entities(),
	m_map(MapParser::parseMap(Utilities::getDataDirectory() + "Level1.tmx")),
	m_battleUI(),
	coord(std::pair<int, int>(0, 0)),
	m_entityPositionInVector(0)
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

void Battle::run()
{
	while (HAPI_Sprites.Update()) 
	{
		render();
		update();
	}
}

void Battle::render()
{
	SCREEN_SURFACE->Clear();

	m_map.drawMap();
	m_battleUI.render();
}

void Battle::update()
{
	m_battleUI.Update();
	std::pair<float,float> tempTileLocation;

	for (int y = 0; y < m_map.getDimensions().second; y++) //Iterating through the whole map 
	{
		for (int x = 0; x < m_map.getDimensions().first; x++)
		{
			std::pair<int, int> currentTile = std::pair<int, int>(x, y);
			m_battleUI.HandleCollision(m_battleUI.m_mouseCursor, m_map.getTile(currentTile)->m_sprite);
			tempTileLocation = std::pair<float, float>(m_map.getTile(currentTile)->m_sprite->GetTransformComp().GetPosition().x,
				m_map.getTile(currentTile)->m_sprite->GetTransformComp().GetPosition().y);

			if (tempTileLocation == m_battleUI.m_tilePos)
			{
				coord = m_map.getTile(currentTile)->m_tileCoordinate;
			}

			for (int j = 0; j < m_entities.size(); j++)
			{
				if (m_entities[j].second == coord)
				{
					m_entityPositionInVector = j;
				}
			}

			if (m_map.moveEntity(std::pair<int, int>(m_entities[m_entityPositionInVector].second), coord))
			{
				m_entities[m_entityPositionInVector].second = coord;
			}

			if (m_map.getTile(currentTile)->m_entityOnTile) 
			{
				HAPISPACE::VectorF tempVec{ m_map.getTile(currentTile)->m_sprite->GetTransformComp().GetPosition().x + 30, m_map.getTile(currentTile)->m_sprite->GetTransformComp().GetPosition().y + 40 };
				m_map.getTile(currentTile)->m_entityOnTile->m_sprite->GetTransformComp().SetPosition(tempVec);
				m_map.getTile(currentTile)->m_entityOnTile->m_sprite->Render(SCREEN_SURFACE);
			}
		}
	}
}
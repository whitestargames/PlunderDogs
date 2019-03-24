#include "Battle.h"
#include "Utilities/MapParser.h"
#include "Utilities/Utilities.h"
#include "entity.h"

using namespace HAPISPACE;

Battle::Battle() :
	m_entities(),
	m_map(MapParser::parseMap(Utilities::getDataDirectory() + "Level1.tmx")),
	m_entityOnPoint(),
	m_entitySelected(false),
	m_mouseCursor(HAPI_Sprites.LoadSprite(Utilities::getDataDirectory() + "mouseCrossHair.xml"))
{
	m_mouseCursor->GetColliderComp().EnablePixelPerfectCollisions(true);
	addEntity("mouseCrossHair.xml", { 4, 4 });
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
	//Draw entities
	for (const auto& entity : m_entities)
	{
		entity.first->m_sprite->Render(SCREEN_SURFACE);
	}
}

void Battle::addEntity(const std::string & fileName, std::pair<int, int> point)
{
	m_entities.emplace_back(std::pair<std::unique_ptr<Entity>, 
		std::pair<int, int>>((std::make_unique<Entity>(Utilities::getDataDirectory() + fileName)), point));
	std::pair<int, int> tileScreenPoint = m_map.getTileScreenPos(point);
	m_entities.back().first->m_sprite->GetTransformComp().SetPosition({ (float)(tileScreenPoint.first + 30), (float)(tileScreenPoint.second + 40)});

	m_map.insertEntity(*m_entities.back().first.get(), point);
}

void Battle::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData & mouseData)
{
	if (mouseEvent == EMouseEvent::eLeftButtonDown && !m_entitySelected)
	{
		m_mouseCursor->GetTransformComp().SetPosition({ (float)mouseData.x - 5,(float)mouseData.x - 5 });
		storeEntity();
		int i = 0;
	}
	else if (mouseEvent == EMouseEvent::eLeftButtonDown && m_entitySelected)
	{
		m_mouseCursor->GetTransformComp().SetPosition({ (float)mouseData.x - 5,(float)mouseData.x - 5 });
		moveEntity();
		m_entitySelected = false;
		m_entityOnPoint = {};
		int i = 0;
	}
}

bool Battle::handleTileCollision(std::unique_ptr<Sprite>& tileSprite)
{
	return m_mouseCursor->CheckCollision(*tileSprite);
}

void Battle::storeEntity()
{
	for (int y = 0; y < m_map.getDimensions().second; y++) 
	{
		for (int x = 0; x < m_map.getDimensions().first; x++)
		{
			Tile* currentTile = m_map.getTile({ x, y });
			assert(currentTile);

			if (!handleTileCollision(currentTile->m_sprite))
			{
				continue;
			}
			
			for (auto& entity : m_entities)
			{
				if (entity.second == currentTile->m_tileCoordinate)
				{
					m_entityOnPoint = entity.second;
					m_entitySelected = true;
					return;
				}
			}		
		}
	}
}

void Battle::moveEntity()
{
	for (int y = 0; y < m_map.getDimensions().second; y++)
	{
		for (int x = 0; x < m_map.getDimensions().first; x++)
		{
			Tile* currentTile = m_map.getTile({ x, y });
			assert(currentTile);

			// No Collision
			if (!handleTileCollision(currentTile->m_sprite))
			{
				continue;
			}

			//No entity in new position
			if (currentTile->m_entityOnTile)
			{
				return;
			}

			auto& tileTransform = currentTile->m_sprite->GetTransformComp();
			for (auto& entity : m_entities)
			{
				if (entity.second == m_entityOnPoint)
				{
					std::pair<int, int> screenPosition = m_map.getTileScreenPos(currentTile->m_tileCoordinate);
					entity.first->m_sprite->GetTransformComp().SetPosition({ (float)(tileTransform.GetPosition().x + 30), 
						(float)(tileTransform.GetPosition().y + 40) });
					entity.second = currentTile->m_tileCoordinate;
					return;
				}
			}

			//	if (!currentTile->m_entityOnTile) 
			//	{
			///*		HAPISPACE::VectorF tempVec{ m_map.getTile({x, y})->m_sprite->GetTransformComp().GetPosition().x + 30, m_map.getTile({x, y})->m_sprite->GetTransformComp().GetPosition().y + 40 };
			//		m_map.getTile({ x, y })->m_entityOnTile->m_sprite->GetTransformComp().SetPosition(tempVec);*/
			//		//HAPISPACE::VectorF tempVec{ tileTransform.GetPosition().x + 30, tileTransform.GetPosition().y + 40 };
			//		//tileTransform.SetPosition(tempVec);
			//		//currentTile->m_entityOnTile->m_sprite->Render(SCREEN_SURFACE);
			//	}
		}
	}
}

void Battle::update()
{
	//if (m_entitySelected)
	//{


	//	if (m_map.moveEntity(std::pair<int, int>(entity.second), currentTile->m_tileCoordinate))
	//	{
	//		m_entities[m_entityPositionInVector].second = currentTile->m_tileCoordinate;
	//	}
	//	return;
	//}

}
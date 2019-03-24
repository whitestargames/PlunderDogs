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
	addEntity("thingy.xml", { 5, 5 });
}

void Battle::render() const
{
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
	if (mouseEvent == EMouseEvent::eLeftButtonDown)
	{
		m_mouseCursor->GetTransformComp().SetPosition({ (float)mouseData.x,(float)mouseData.y });
		handleEntityMovement();
	}
}

bool Battle::collision(std::unique_ptr<Sprite>& tileSprite) const
{
	return m_mouseCursor->CheckCollision(*tileSprite);
}

void Battle::handleEntityMovement()
{
	for (int y = 0; y < m_map.getDimensions().second; y++) 
	{
		for (int x = 0; x < m_map.getDimensions().first; x++)
		{
			Tile* currentTile = m_map.getTile({ x, y });
			assert(currentTile);

			//Check Collision
			if (!collision(currentTile->m_sprite))
			{
				continue;
			}

			//Move Selected Entity
			if (m_entitySelected)
			{
				moveEntity(*currentTile);
				return;
			}
			//Select new entity for movement
			else
			{
				selectEntity(*currentTile);
				return;
			}
		}
	}
}

void Battle::moveEntity(const Tile& tile)
{
	//Already existing entity in requested new position
	if (tile.m_entityOnTile && (tile.m_type != eTileType::eOcean && tile.m_type != eTileType::eSea))
	{
		m_entitySelected = false;
		m_entityOnPoint = {};
		return;
	}

	auto& tileTransform = tile.m_sprite->GetTransformComp();
	for (auto& entity : m_entities)
	{
		//Find entity to move
		if (entity.second == m_entityOnPoint)
		{
			m_map.getTile({ entity.second })->m_entityOnTile = nullptr;

			//Move Entity
			entity.first->m_sprite->GetTransformComp().SetPosition({ (float)(tileTransform.GetPosition().x + 30),
				(float)(tileTransform.GetPosition().y + 40) });
			entity.second = tile.m_tileCoordinate;
			m_map.getTile({ entity.second })->m_entityOnTile = entity.first.get();
			
			m_entitySelected = false;
			m_entityOnPoint = {};
			break;
		}
	}
}

void Battle::selectEntity(const Tile& tile)
{
	for (const auto& entity : m_entities)
	{
		//Find entity that matches requested tile
		if (entity.second == tile.m_tileCoordinate)
		{
			m_entityOnPoint = entity.second;
			m_entitySelected = true;
			break;
		}
	}
}
#include "Battle.h"
#include "Utilities/MapParser.h"
#include "Utilities/Utilities.h"
#include "entity.h"
#include "Pathfinding.h"

using namespace HAPISPACE;

Battle::Battle() :
	m_entities(),
	m_map(MapParser::parseMap(Utilities::getDataDirectory() + "Level1.tmx")),
	m_selectedEntityPoint(),
	m_isEntitySelected(false),
	m_mouseCursor(HAPI_Sprites.LoadSprite(Utilities::getDataDirectory() + "mouseCrossHair.xml")),
	m_movementPathSprites()
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

	for (const auto& i : m_movementPathSprites)
	{
		i->Render(SCREEN_SURFACE);
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

void Battle::OnMouseMove(const HAPI_TMouseData & mouseData)
{
	if (!m_isEntitySelected)
	{
		return;
	}

	m_mouseCursor->GetTransformComp().SetPosition({ (float)mouseData.x,(float)mouseData.y });
	m_movementPathSprites.clear();
	for (int y = 0; y < m_map.getDimensions().second; y++)
	{
		for (int x = 0; x < m_map.getDimensions().first; x++)
		{
			Tile* currentTile = m_map.getTile({ x, y });
			assert(currentTile);

			if (!collision(currentTile->m_sprite))
			{
				continue;
			}

			//Create movement trail to where mouse cursor is 
			auto pathToTile = PathFinding::getPathToTile(m_map, m_selectedEntityPoint, currentTile->m_tileCoordinate);
			for (int i = 0; i < pathToTile.size(); i++)
			{
				auto tileScreenPosition = m_map.getTileScreenPos(pathToTile[i]);
				m_movementPathSprites.emplace_back(HAPI_Sprites.LoadSprite(Utilities::getDataDirectory() + "mouseCrossHair.xml"));
				m_movementPathSprites.back()->GetTransformComp().SetPosition({ (float)tileScreenPosition.first + 30, (float)tileScreenPosition.second + 40});
			}
		}
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
			if (m_isEntitySelected)
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
	if (tile.m_entityOnTile || (tile.m_type != eTileType::eOcean && tile.m_type != eTileType::eSea))
	{
		m_isEntitySelected = false;
		m_selectedEntityPoint = {};
		return;
	}

	for (auto& entity : m_entities)
	{
		//Find entity to move
		if (entity.second == m_selectedEntityPoint)
		{
			//Make sure new point is within movement bounds of the entity
			std::vector<std::pair<int, int>> pathToTile = PathFinding::getPathToTile(m_map, m_selectedEntityPoint, tile.m_tileCoordinate);
			if (pathToTile.size() > entity.first->m_movementPoints)
			{
				m_isEntitySelected = false;
				m_selectedEntityPoint = {};
				break;
			}

			m_map.getTile({ entity.second })->m_entityOnTile = nullptr;
		
			//Move Entity
			const auto& tileTransform = tile.m_sprite->GetTransformComp();
			entity.first->m_sprite->GetTransformComp().SetPosition({ (float)(tileTransform.GetPosition().x + 30),
				(float)(tileTransform.GetPosition().y + 40) });
			entity.second = tile.m_tileCoordinate;
			m_map.getTile({ entity.second })->m_entityOnTile = entity.first.get();
			
			m_isEntitySelected = false;
			m_selectedEntityPoint = {};
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
			m_selectedEntityPoint = entity.second;
			m_isEntitySelected = true;
			break;
		}
	}
}
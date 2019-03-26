#include "Battle.h"
#include "Utilities/MapParser.h"
#include "Utilities/Utilities.h"
#include "entity.h"
#include "Pathfinding.h"

using namespace HAPISPACE;

Battle::Battle() :
	m_entity(),
	m_map(MapParser::parseMap(Utilities::getDataDirectory() + "Level1.tmx")),
	m_isEntitySelected(false),
	m_mouseCursor(HAPI_Sprites.LoadSprite(Utilities::getDataDirectory() + "mouseCrossHair.xml")),
	m_movementPath(),
	m_previousMousePoint()
{
	m_mouseCursor->GetColliderComp().EnablePixelPerfectCollisions(true);
	initializeEntity("thingy.xml", { 5, 5 });
}

void Battle::render() const
{
	m_map.drawMap();
	
	m_entity.first->m_sprite->Render(SCREEN_SURFACE);

	for (const auto& i : m_movementPath)
	{
		i->Render(SCREEN_SURFACE);
	}
}

void Battle::initializeEntity(const std::string & fileName, std::pair<int, int> point)
{
	m_entity.first = std::make_unique<Entity>(Utilities::getDataDirectory() + fileName);
	m_entity.second = point;

	//Assign entity sprite position
	std::pair<int, int> tileScreenPoint = m_map.getTileScreenPos(point);
	m_entity.first->m_sprite->GetTransformComp().SetPosition({ (float)(tileScreenPoint.first + 30), (float)(tileScreenPoint.second + 40)});

	//Insert entity into map
	m_map.insertEntity(*m_entity.first.get(), point);
}

void Battle::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData & mouseData)
{
	if (mouseEvent == EMouseEvent::eLeftButtonDown)
	{
		m_mouseCursor->GetTransformComp().SetPosition({ (float)mouseData.x,(float)mouseData.y });
		handleEntityMovement();

		//Initial point for the movement graph
		//Starts at point where entity has been selected
		if (m_isEntitySelected)
		{
			m_previousMousePoint = m_entity.second;
		}

		//If movement path is being displayed and the entity has moved
		//No further need of movement graph
		if (!m_movementPath.empty())
		{
			m_movementPath.clear();
		}
	}
}

void Battle::OnMouseMove(const HAPI_TMouseData & mouseData)
{
	if (!m_isEntitySelected)
	{
		return;
	}

	m_mouseCursor->GetTransformComp().SetPosition({ (float)mouseData.x,(float)mouseData.y });
	
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

			//If mouse hovering over tile that has been handled for the movement path
			//No need to redo the same opreations
			if (m_previousMousePoint == currentTile->m_tileCoordinate)
			{
				return;
			}

			auto pathToTile = PathFinding::getPathToTile(m_map, m_entity.second, currentTile->m_tileCoordinate);
			if (pathToTile.empty())
			{
				return;
			}

			//Create movement trail to where mouse cursor is 
			m_movementPath.clear();
			for (int i = 0; i < pathToTile.size() - 1; i++)
			{
				auto tileScreenPosition = m_map.getTileScreenPos(pathToTile[i]);
				m_movementPath.emplace_back(HAPI_Sprites.MakeSprite(m_mouseCursor->GetSpritesheet()));
				m_movementPath.back()->GetTransformComp().SetPosition({ (float)tileScreenPosition.first + 30, (float)tileScreenPosition.second + 40 });
			}

			//Assign last position for the end of the movement graph
			m_previousMousePoint = currentTile->m_tileCoordinate;
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
	if (tile.m_type != eTileType::eOcean && tile.m_type != eTileType::eSea)
	{
		m_isEntitySelected = false;
		return;
	}

	//Make sure new point is within movement bounds of the entity
	std::vector<std::pair<int, int>> pathToTile = PathFinding::getPathToTile(m_map, m_entity.second, tile.m_tileCoordinate);
	if (pathToTile.size() > m_entity.first->m_movementPoints)
	{
		m_isEntitySelected = false;
	}

	//Move Entity
	const auto& tileTransform = tile.m_sprite->GetTransformComp();
	m_entity.first->m_sprite->GetTransformComp().SetPosition({ (float)(tileTransform.GetPosition().x + 30),
		(float)(tileTransform.GetPosition().y + 40) });

	m_entity.second = tile.m_tileCoordinate;

	m_map.getTile({ m_entity.second })->m_entityOnTile = nullptr;
	m_map.getTile({ m_entity.second })->m_entityOnTile = m_entity.first.get();

	m_isEntitySelected = false;
}

void Battle::selectEntity(const Tile& tile)
{
	if (m_entity.second == tile.m_tileCoordinate)
	{
		m_isEntitySelected = true;
	}
}
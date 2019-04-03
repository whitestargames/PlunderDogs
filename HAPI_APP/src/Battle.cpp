#include "Battle.h"
#include "Utilities/MapParser.h"
#include "Utilities/Utilities.h"
#include "entity.h"
#include "Pathfinding.h"
#include "OverWorld.h"
#include "HAPIWrapper.h"
#include "Timer.h"

using namespace HAPISPACE;
constexpr float DRAW_OFFSET_X{ 12 };
constexpr float DRAW_OFFSET_Y{ 28 };
constexpr size_t MOVEMENT_PATH_SIZE{ 32 };

std::deque<std::pair<int, int>> getPathToTile(std::pair<int, int> src, std::pair<int, int> dest, Map& map);

std::deque<std::pair<int, int>> getPathToTile(std::pair<int, int> src, std::pair<int, int> dest, Map & map)
{
	auto pathToTile = PathFinding::getPathToTile(map, src, dest);
	if (pathToTile.empty())
	{
		return std::deque<std::pair<int, int>>();
	}

	std::deque<std::pair<int, int>> path;
	for (int i = pathToTile.size() - 1; i >= 0; i--)
	{
		path.push_back(pathToTile[i]);
	}

	return path;
}

Battle::Battle() :
	m_entity(),
	m_map(MapParser::parseMap("Level1.tmx")),
	m_entitySelected(false),
	m_movementAllowed(true),
	m_mouseCursor(HAPI_Wrapper::loadSprite("mouseCrossHair.xml")),
	m_movementPath(),
	m_previousMousePoint()
{
	m_mouseCursor->GetColliderComp().EnablePixelPerfectCollisions(true);
	
	//Initialize Entity
	m_entity.first = std::make_unique<Entity>(Utilities::getDataDirectory() + "thingy.xml");
	m_entity.second.m_currentPosition = std::make_pair<int, int>(5, 5);
	//Assign entity sprite position
	std::pair<int, int> tileScreenPoint = m_map.getTileScreenPos(m_entity.second.m_currentPosition);
	m_entity.first->m_sprite->GetTransformComp().SetPosition({
		(float)(tileScreenPoint.first + DRAW_OFFSET_X * m_map.getDrawScale()),
		(float)(tileScreenPoint.second + DRAW_OFFSET_Y * m_map.getDrawScale()) });
	//Insert entity into map
	m_map.insertEntity(*m_entity.first.get(), m_entity.second.m_currentPosition);
	
	//Initialize Movement Path
	m_movementPath.reserve(size_t(MOVEMENT_PATH_SIZE));
	for (int i = 0; i < MOVEMENT_PATH_SIZE; i++)
	{
		std::pair<std::unique_ptr<Sprite>, bool> sprite;
		sprite.first = HAPI_Sprites.MakeSprite(m_mouseCursor->GetSpritesheet());
		sprite.second = false;
		m_movementPath.push_back(std::move(sprite));
	}
}

void Battle::render() const
{
	m_map.drawMap();

	//Move entity sprite
	const std::pair<int, int> tileTransform = m_map.getTileScreenPos(m_entity.second.m_currentPosition);
	m_entity.first->m_sprite->GetTransformComp().SetPosition({
		static_cast<float>(tileTransform.first + DRAW_OFFSET_X * m_map.getDrawScale()),
		static_cast<float>(tileTransform.second + DRAW_OFFSET_Y * m_map.getDrawScale()) });
	//Render entity
	m_entity.first->render();
	//Draw Movement Graph
	for (auto& i : m_movementPath)
	{
		if (!i.second)
		{
			break;
		}

		i.first->Render(SCREEN_SURFACE);
	}
}

void Battle::update(float deltaTime)
{
	/*m_map.moveEntity(m_entity.second.m_currentPosition, tile.m_tileCoordinate);
	m_entity.second.m_currentPosition = tile.m_tileCoordinate;*/

	if (!m_movementAllowed || m_entity.second.m_pathToTile.empty())
	{
		return;
	}
	auto& movementTimer = m_entity.first->m_movementTimer;
	movementTimer.update(deltaTime);
	if (movementTimer.isExpired())
	{
		movementTimer.reset();
		
		m_entity.second.m_oldPosition = m_entity.second.m_currentPosition;
		m_entity.second.m_currentPosition = m_entity.second.m_pathToTile.front();
		m_map.moveEntity(m_entity.second.m_oldPosition, m_entity.second.m_currentPosition);

		m_entity.second.m_pathToTile.pop_front();
	}

	if (!m_movementAllowed || m_entity.second.m_pathToTile.empty())
	{
		int i = 0;
	}
}

void Battle::resetMovementPath()
{
	for (auto& i : m_movementPath)
	{
		i.second = false;
	}
}

void Battle::setMovementGraphPositions()
{
	if (m_entity.second.m_pathToTile.size() > m_entity.first->m_movementPoints + 1)
	{
		//Don't interact with path from source.
		for (int i = 1; i < m_entity.first->m_movementPoints + 1; ++i)
		{
			auto tileScreenPosition = m_map.getTileScreenPos(m_entity.second.m_pathToTile[i]);
			m_movementPath[i - 1].first->GetTransformComp().SetPosition({
				static_cast<float>(tileScreenPosition.first + DRAW_OFFSET_X * m_map.getDrawScale()),
				static_cast<float>(tileScreenPosition.second + DRAW_OFFSET_Y * m_map.getDrawScale()) });
			m_movementPath[i - 1].second = true;
		}
	}
	else
	{
		//Don't interact with path from source.
		for (int i = 1; i < m_entity.second.m_pathToTile.size(); ++i)
		{
			auto tileScreenPosition = m_map.getTileScreenPos(m_entity.second.m_pathToTile[i]);
			m_movementPath[i - 1].first->GetTransformComp().SetPosition({
				static_cast<float>(tileScreenPosition.first + DRAW_OFFSET_X * m_map.getDrawScale()),
				static_cast<float>(tileScreenPosition.second + DRAW_OFFSET_Y * m_map.getDrawScale()) });
			m_movementPath[i - 1].second = true;
		}
	}
}

void Battle::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData & mouseData)
{
	if (OverWorld::CURRENT_WINDOW != OverWorldWindow::Battle)
	{
		return;
	}

	if (mouseEvent == EMouseEvent::eLeftButtonDown)
	{
		Tile* currentTile = m_map.getTile(m_map.getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
		if (!currentTile)
		{
			return;
		}

		//Select new entity for movement
		if (!m_entitySelected)
		{
			selectEntity(*currentTile);
		}
		
		////Move Selected Entity
		//else if (m_movementAllowed)
		//{
		//	moveEntity(*currentTile);
		//	resetMovementPath();
		//}
	}
	else if (mouseEvent == EMouseEvent::eRightButtonDown)
	{
		//Cancel selected entity
		m_entitySelected = false;
		resetMovementPath();
	}
}

void Battle::OnMouseMove(const HAPI_TMouseData & mouseData)
{
	if (m_entitySelected && OverWorld::CURRENT_WINDOW == OverWorldWindow::Battle)
	{
		//Tile at mouse location
		const Tile* currentTile = m_map.getTile(m_map.getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
		if (!currentTile || m_previousMousePoint == currentTile->m_tileCoordinate)
		{
			return;
		}

		//Assign mouse point to new tile
		m_previousMousePoint = currentTile->m_tileCoordinate;

		m_entity.second.m_pathToTile = getPathToTile(m_entity.second.m_currentPosition, currentTile->m_tileCoordinate, m_map);
		if (m_entity.second.m_pathToTile.empty())
		{
			return;
		}

		m_movementAllowed = (m_entity.second.m_pathToTile.size() <= m_entity.first->m_movementPoints + 1);
		resetMovementPath();
		setMovementGraphPositions();
	}
}

void Battle::moveEntity(const Tile& tile)
{
	if (tile.m_type == eTileType::eOcean || tile.m_type == eTileType::eSea)
	{
		m_map.moveEntity(m_entity.second.m_currentPosition, tile.m_tileCoordinate);
		m_entity.second.m_currentPosition = tile.m_tileCoordinate;
	}

	m_entitySelected = false;
}

void Battle::selectEntity(const Tile& tile)
{
	if (m_entity.second.m_currentPosition == tile.m_tileCoordinate)
	{
		m_entitySelected = true;
		m_previousMousePoint = m_entity.second.m_currentPosition;
		m_movementAllowed = true;
	}	
	else
	{
		m_entitySelected = false;
		resetMovementPath();
	}
}
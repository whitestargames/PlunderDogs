#include "Battle.h"
#include "Utilities/MapParser.h"
#include "Utilities/Utilities.h"
#include "entity.h"
#include "Pathfinding.h"
#include "OverWorld.h"
#include "HAPIWrapper.h"

using namespace HAPISPACE;
constexpr float DRAW_OFFSET_X{ 12 };
constexpr float DRAW_OFFSET_Y{ 28 };
constexpr size_t MOVEMENT_PATH_SIZE{ 32 };

Battle::Battle() :
	m_entity(),
	m_map(MapParser::parseMap("Level1.tmx")),
	m_isEntitySelected(false),
	m_mouseCursor(HAPI_Wrapper::loadSprite("mouseCrossHair.xml")),
	m_movementPath(),
	m_previousMousePoint()
{
	m_mouseCursor->GetColliderComp().EnablePixelPerfectCollisions(true);
	initializeEntity("thingy.xml", { 5, 5 });
	
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
	const std::pair<int, int> tileTransform = m_map.getTileScreenPos(m_entity.second);
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

//float dx = target->x - x;
//float dy = target->y - y;
//float dist = std::sqrt(dx*dx + dy * dy);
//if (dist > speed * delta) {
//	dx /= dist;
//	dy /= dist;
//	x += dx * speed * delta;
//	y += dy * speed * delta;
//}
//else {
//	x = target->x;
//	y = target->y;
//}

void Battle::update(float deltaTime)
{

}

void Battle::initializeEntity(const std::string & fileName, std::pair<int, int> point)
{
	m_entity.first = std::make_unique<Entity>(Utilities::getDataDirectory() + fileName);
	m_entity.second = point;

	//Assign entity sprite position
	std::pair<int, int> tileScreenPoint = m_map.getTileScreenPos(point);
	m_entity.first->m_sprite->GetTransformComp().SetPosition({ 
		(float)(tileScreenPoint.first + DRAW_OFFSET_X * m_map.getDrawScale()), 
		(float)(tileScreenPoint.second + DRAW_OFFSET_Y * m_map.getDrawScale())});

	//Insert entity into map
	m_map.insertEntity(*m_entity.first.get(), point);
}

void Battle::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData & mouseData)
{
	if (OverWorld::CURRENT_WINDOW != OverWorldWindow::Battle)
	{
		return;
	}

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

		//Once entity has moved
		//Clear movement trail
		for (auto& i : m_movementPath)
		{
			i.second = false;
		}
	}
	else if (mouseEvent == EMouseEvent::eRightButtonDown)
	{
		m_isEntitySelected = false;

		for (auto& i : m_movementPath)
		{
			i.second = false;
		}
	}
}

void Battle::OnMouseMove(const HAPI_TMouseData & mouseData)
{
	if (OverWorld::CURRENT_WINDOW != OverWorldWindow::Battle)
	{
		return;
	}

	if (!m_isEntitySelected)
	{
		return;
	}
	//Make the cursor image follow the cursor
	m_mouseCursor->GetTransformComp().SetPosition({ (float)mouseData.x,(float)mouseData.y });
	handleMovementPath();
}

void Battle::handleEntityMovement()
{
	Tile* currentTile = m_map.getTile(m_map.getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
	if (!currentTile)
	{
		return;
	}

	//Move Selected Entity
	if (m_isEntitySelected)
	{
		moveEntity(*currentTile);
	}
	//Select new entity for movement
	else
	{
		selectEntity(*currentTile);//Note: this means that "selectEntity" is actually more like "selectTile"
	}
}

void Battle::handleMovementPath()
{
	Tile* currentTile = m_map.getTile(m_map.getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
	if (!currentTile)
	{
		return;
	}
	
	//If mouse hovering over tile that has been handled for the movement path
	//No need to redo the same opreations
	if (m_previousMousePoint == currentTile->m_tileCoordinate)
	{
		return;
	}
	
	auto pathToTile = getPathToTile(currentTile->m_tileCoordinate);
	if(pathToTile.empty() || pathToTile.size() > m_entity.first->m_movementPoints + 1)
	{
		return;
	}

	for (auto& i : m_movementPath)
	{
		i.second = false;
	}

	//Don't interact with path from source.
	for (int i = 1; i < pathToTile.size(); ++i)
	{
		auto tileScreenPosition = m_map.getTileScreenPos(pathToTile[i]);
		m_movementPath[i - 1].first->GetTransformComp().SetPosition({
			static_cast<float>(tileScreenPosition.first + DRAW_OFFSET_X * m_map.getDrawScale()),
			static_cast<float>(tileScreenPosition.second + DRAW_OFFSET_Y * m_map.getDrawScale()) });
		m_movementPath[i - 1].second = true;
	}

	//Assign last position for the end of the movement graph
	m_previousMousePoint = currentTile->m_tileCoordinate;
}

void Battle::moveEntity(const Tile& tile)
{
	//Already existing entity in requested new position
	if (tile.m_entityOnTile)
	{
		m_isEntitySelected = tile.m_entityOnTile;
		return;
	}
	//Not a travellable tile
	if (tile.m_type != eTileType::eOcean && tile.m_type != eTileType::eSea)
	{
		m_isEntitySelected = false;
		return;
	}

	//Change map's coordinate for entity
	m_map.moveEntity(m_entity.second, tile.m_tileCoordinate);
	//Change entity's coordinate
	m_entity.second = tile.m_tileCoordinate;
	//Deselect entity
	m_isEntitySelected = false;
}

void Battle::selectEntity(const Tile& tile)
{
	if (m_entity.second == tile.m_tileCoordinate)
	{
		m_isEntitySelected = true;
	}	
}

std::vector<std::pair<int, int>> Battle::getPathToTile(std::pair<int, int> dest)
{
	auto pathToTile = PathFinding::getPathToTile(m_map, m_entity.second, dest);
	if (pathToTile.empty())
	{
		return std::vector<std::pair<int, int>>();
	}

	std::vector<std::pair<int, int>> path;
	path.reserve(pathToTile.size());
	for (int i = pathToTile.size() - 1; i >= 0; i--)
	{
		path.push_back(pathToTile[i]);
	}

	return path;
}
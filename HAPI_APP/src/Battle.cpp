#include "Battle.h"
#include "Utilities/MapParser.h"
#include "Utilities/Utilities.h"
#include "entity.h"
#include "Pathfinding.h"

using namespace HAPISPACE;
constexpr float DRAW_OFFSET_X{ 30.0 };
constexpr float DRAW_OFFSET_Y{ 40.0 };

Battle::Battle() :
	m_entities(),
	m_map(MapParser::parseMap(Utilities::getDataDirectory() + "Level1.tmx")),
	m_selectedEntity(0),
	m_isEntitySelected(false),
	m_mouseCursor(HAPI_Sprites.LoadSprite(Utilities::getDataDirectory() + "mouseCrossHair.xml")),
	m_movementPath(),
	m_previousMousePoint()
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
		//Move entity sprite
		const std::pair<int, int> tileTransform = m_map.getTileScreenPos(entity.second);
		m_entities[m_selectedEntity].first->m_sprite->GetTransformComp().SetPosition({
			static_cast<float>(tileTransform.first + DRAW_OFFSET_X * m_map.getDrawScale()),
			static_cast<float>(tileTransform.second + DRAW_OFFSET_Y * m_map.getDrawScale()) });
		//Render entity
		entity.first->m_sprite->Render(SCREEN_SURFACE);
	}
	//Draw Movement Graph
	for (const auto& i : m_movementPath)
	{
		i->Render(SCREEN_SURFACE);
	}
}

void Battle::addEntity(const std::string & fileName, std::pair<int, int> point)
{
	m_entities.emplace_back(std::pair<std::unique_ptr<Entity>, 
		std::pair<int, int>>((std::make_unique<Entity>(Utilities::getDataDirectory() + fileName)), point));

	//Assign entity sprite position
	std::pair<int, int> tileScreenPoint = m_map.getTileScreenPos(point);
	m_entities.back().first->m_sprite->GetTransformComp().SetPosition({ (float)(tileScreenPoint.first + 30), (float)(tileScreenPoint.second + 40)});

	//Insert entity into map
	m_map.insertEntity(*m_entities.back().first.get(), point);
}

void Battle::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData & mouseData)
{
	if (mouseEvent == EMouseEvent::eLeftButtonDown)
	{
		m_mouseCursor->GetTransformComp().SetPosition({ (float)mouseData.x,(float)mouseData.y });
		handleEntityMovement();

		//Initial point for the movement graph
		//Starts at point where entity has been selected
		m_previousMousePoint = m_entities[m_selectedEntity].second;
		
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
	//Make the cursor image follow the cursor
	m_mouseCursor->GetTransformComp().SetPosition({ (float)mouseData.x,(float)mouseData.y });
	
	const std::pair<int, int> mouseLocation(mouseData.x, mouseData.y);
	Tile* currentTile = m_map.getTile(m_map.getMouseClickCoord(mouseLocation));
	//Exit if there is no tile near the mouse cursor
	if (!currentTile)
		return;
	//If mouse hovering over tile that has been handled for the movement path
	//No need to redo the same operations
	if (m_previousMousePoint == currentTile->m_tileCoordinate)
	{
		return;
	}
	auto pathToTile = PathFinding::getPathToTile(
		m_map, m_entities[m_selectedEntity].second, currentTile->m_tileCoordinate);
	if (pathToTile.empty())
	{
		return;
	}
	//Create movement trail to where mouse cursor is 
	m_movementPath.clear();
	for (int i = 0; i < pathToTile.size(); i++)
	{
		auto tileScreenPosition = m_map.getTileScreenPos(pathToTile[i]);
		m_movementPath.emplace_back(HAPI_Sprites.MakeSprite(m_mouseCursor->GetSpritesheet()));
		m_movementPath.back()->GetTransformComp().SetPosition({
			(float)tileScreenPosition.first + DRAW_OFFSET_X * m_map.getDrawScale(), 
			(float)tileScreenPosition.second + DRAW_OFFSET_Y * m_map.getDrawScale() });
	}
	//Assign last position for the end of the movement graph
	m_previousMousePoint = currentTile->m_tileCoordinate;
}

bool Battle::collision(std::unique_ptr<Sprite>& tileSprite) const
{
	return m_mouseCursor->CheckCollision(*tileSprite);
}

void Battle::handleEntityMovement()
{
	const std::pair<int, int> mouseLocation(
		HAPI_Sprites.GetMouseData().x, HAPI_Sprites.GetMouseData().y);
	Tile* currentTile = m_map.getTile(m_map.getMouseClickCoord(mouseLocation));
	//Exit if there is no tile near the mouse cursor
	if (!currentTile)
		return;
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
	//Make sure new point is within movement bounds of the entity
	std::vector<std::pair<int, int>> pathToTile = PathFinding::getPathToTile(
		m_map, m_entities[m_selectedEntity].second, tile.m_tileCoordinate);

	if (pathToTile.size() > m_entities[m_selectedEntity].first->m_movementPoints)
	{
		m_isEntitySelected = false;
		return;
	}
	//Change map's coordinate for entity
	m_map.moveEntity(m_entities[m_selectedEntity].second, tile.m_tileCoordinate);
	//Change entity's coordinate
	m_entities[m_selectedEntity].second = tile.m_tileCoordinate;
	//Deselect entity
	m_isEntitySelected = false;
}

void Battle::selectEntity(const Tile& tile)
{
	for (int i = 0; i < m_entities.size(); i++)
	{
		//Find entity that matches requested tile
		if (m_entities[i].second == tile.m_tileCoordinate)
		{
			m_selectedEntity = i;
			m_isEntitySelected = true;
			break;
		}
	}
}
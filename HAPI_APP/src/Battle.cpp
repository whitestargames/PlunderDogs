#include "Battle.h"
#include "Utilities/MapParser.h"
#include "Utilities/Utilities.h"
#include "entity.h"
#include "Pathfinding.h"
#include "OverWorld.h"
#include "HAPIWrapper.h"
#include "Timer.h"

using namespace HAPISPACE;
constexpr size_t MOVEMENT_PATH_SIZE{ 32 };

//Battle::Battle() :
//	m_entity(),
//	m_map(MapParser::parseMap("Level1.tmx")),
//	m_entitySelected(false),
//	m_movementAllowed(true),
//	m_mouseCursor(HAPI_Wrapper::loadSprite("mouseCrossHair.xml")),
//	m_movementPath(),
//	m_previousMousePoint()
//{
//	m_mouseCursor->GetColliderComp().EnablePixelPerfectCollisions(true);
//	
//	//Initialize Entity
//	m_entity.first = std::make_unique<Entity>(Utilities::getDataDirectory() + "thingy.xml");
//	m_entity.second.m_currentPosition = std::make_pair<int, int>(5, 5);
//	//Assign entity sprite position
//	std::pair<int, int> tileScreenPoint = m_map.getTileScreenPos(m_entity.second.m_currentPosition);
//	m_entity.first->m_sprite->GetTransformComp().SetPosition({
//		(float)(tileScreenPoint.first + DRAW_OFFSET_X * m_map.getDrawScale()),
//		(float)(tileScreenPoint.second + DRAW_OFFSET_Y * m_map.getDrawScale()) });
//	//Insert entity into map
//	m_map.insertEntity(*m_entity.first.get(), m_entity.second.m_currentPosition);
//}

Battle::Battle() :
	m_entity(),
	m_map(MapParser::parseMap("Level1.tmx")),
	m_battleUI(*this)
{	
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
}

void Battle::render()
{
	m_map.drawMap();

	//Move entity sprite
	const std::pair<int, int> tileTransform = m_map.getTileScreenPos(m_entity.second.m_currentPosition);
	m_entity.first->m_sprite->GetTransformComp().SetPosition({
		static_cast<float>(tileTransform.first + DRAW_OFFSET_X * m_map.getDrawScale()),
		static_cast<float>(tileTransform.second + DRAW_OFFSET_Y * m_map.getDrawScale()) });
	//Render entity
	m_entity.first->m_sprite->Render(SCREEN_SURFACE);

	m_battleUI.render();
}

void Battle::update(float deltaTime)
{
	//m_map.moveEntity(m_entity.second.m_currentPosition, tile.m_tileCoordinate);
	//m_entity.second.m_currentPosition = tile.m_tileCoordinate;
	//if (!m_movementAllowed || m_entity.second.m_pathToTile.empty())
	//{
	//	return;
	//}
	//auto& movementTimer = m_entity.first->m_movementTimer;
	//movementTimer.update(deltaTime);
	//if (movementTimer.isExpired())
	//{
	//	movementTimer.reset();
	//	
	//	m_entity.second.m_oldPosition = m_entity.second.m_currentPosition;
	//	m_entity.second.m_currentPosition = m_entity.second.m_pathToTile.front();
	//	m_map.moveEntity(m_entity.second.m_oldPosition, m_entity.second.m_currentPosition);
	//	m_entity.second.m_pathToTile.pop_front();
	//}
	//if (!m_movementAllowed || m_entity.second.m_pathToTile.empty())
	//{
	//	int i = 0;
	//}
}

void Battle::entityMoveTo(Entity & entity, std::pair<int, int> newPosition)
{
	
}

Map & Battle::getMap()
{
	return m_map;
}

//void Battle::resetMovementPath()
//{
//	for (auto& i : m_movementPath)
//	{
//		i.second = false;
//	}
//}
//
//void Battle::setMovementGraphPositions()
//{
//	if (m_entity.second.m_pathToTile.size() > m_entity.first->m_movementPoints + 1)
//	{
//		//Don't interact with path from source.
//		for (int i = 1; i < m_entity.first->m_movementPoints + 1; ++i)
//		{
//			auto tileScreenPosition = m_map.getTileScreenPos(m_entity.second.m_pathToTile[i]);
//			m_movementPath[i - 1].first->GetTransformComp().SetPosition({
//				static_cast<float>(tileScreenPosition.first + DRAW_OFFSET_X * m_map.getDrawScale()),
//				static_cast<float>(tileScreenPosition.second + DRAW_OFFSET_Y * m_map.getDrawScale()) });
//			m_movementPath[i - 1].second = true;
//		}
//	}
//	else
//	{
//		//Don't interact with path from source.
//		for (int i = 1; i < m_entity.second.m_pathToTile.size(); ++i)
//		{
//			auto tileScreenPosition = m_map.getTileScreenPos(m_entity.second.m_pathToTile[i]);
//			m_movementPath[i - 1].first->GetTransformComp().SetPosition({
//				static_cast<float>(tileScreenPosition.first + DRAW_OFFSET_X * m_map.getDrawScale()),
//				static_cast<float>(tileScreenPosition.second + DRAW_OFFSET_Y * m_map.getDrawScale()) });
//			m_movementPath[i - 1].second = true;
//		}
//	}
//}
//
//void Battle::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData & mouseData)
//{
//	//if (OverWorld::CURRENT_WINDOW != OverWorldWindow::Battle)
//	//{
//	//	return;
//	//}
//
//	if (mouseEvent == EMouseEvent::eLeftButtonDown)
//	{
//		Tile* currentTile = m_map.getTile(m_map.getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
//		if (!currentTile)
//		{
//			return;
//		}
//
//		//Select new entity for movement
//		if (!m_entitySelected)
//		{
//			selectEntity(*currentTile);
//		}
//		
//		////Move Selected Entity
//		//else if (m_movementAllowed)
//		//{
//		//	moveEntity(*currentTile);
//		//	resetMovementPath();
//		//}
//	}
//	else if (mouseEvent == EMouseEvent::eRightButtonDown)
//	{
//		//Cancel selected entity
//		m_entitySelected = false;
//		resetMovementPath();
//	}
//}
//
//void Battle::OnMouseMove(const HAPI_TMouseData & mouseData)
//{
//	//if (m_entitySelected && OverWorld::CURRENT_WINDOW == OverWorldWindow::Battle)
//	if (m_entitySelected)
//	{
//		//Tile at mouse location
//		const Tile* currentTile = m_map.getTile(m_map.getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
//		if (!currentTile || m_previousMousePoint == currentTile->m_tileCoordinate)
//		{
//			return;
//		}
//
//		//Assign mouse point to new tile
//		m_previousMousePoint = currentTile->m_tileCoordinate;
//
//		m_entity.second.m_pathToTile = getPathToTile(m_entity.second.m_currentPosition, currentTile->m_tileCoordinate, m_map);
//		if (m_entity.second.m_pathToTile.empty())
//		{
//			return;
//		}
//
//		m_movementAllowed = (m_entity.second.m_pathToTile.size() <= m_entity.first->m_movementPoints + 1);
//		resetMovementPath();
//		setMovementGraphPositions();
//	}
//}
//
//
//void Battle::moveEntity(const Tile& tile)
//{
//	if (tile.m_type == eTileType::eOcean || tile.m_type == eTileType::eSea)
//	{
//		m_map.moveEntity(m_entity.second.m_currentPosition, tile.m_tileCoordinate);
//		m_entity.second.m_currentPosition = tile.m_tileCoordinate;
//	}
//
//	m_entitySelected = false;
//}
//
//void Battle::selectEntity(const Tile& tile)
//{
//	if (m_entity.second.m_currentPosition == tile.m_tileCoordinate)
//	{
//		m_entitySelected = true;
//		m_previousMousePoint = m_entity.second.m_currentPosition;
//		m_movementAllowed = true;
//	}	
//	else
//	{
//		m_entitySelected = false;
//		resetMovementPath();
//	}
//}
//
////Battle::MovementPath::MovementPath(size_t size)
////{
////
////void Battle::MovementPath::render() const
////{
////	for (const auto& i : m_movementPath)
////	{
////		if (!i.second)
////		{
////			break;
////		}
////
////		i.first->Render(SCREEN_SURFACE);
////	}
////}
////
////void Battle::MovementPath::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData & mouseData)
////{
////	if (OverWorld::CURRENT_WINDOW == OverWorldWindow::Battle)
////	{
////
////	}
////}
////
////void Battle::MovementPath::OnMouseMove(const HAPI_TMouseData & mouseData)
////{
////	if (OverWorld::CURRENT_WINDOW == OverWorldWindow::Battle)
////	{
////		m_mousePosition = HAPI_Wrapper::getMouseLocation();
////	}
////}
////
////void Battle::MovementPath::selectEntity(const Tile & tile)
////{
////
////}
////
////void Battle::MovementPath::resetMovementPath()
////{
////
////}
////
////void Battle::MovementPath::setMovementGraphPositions(const Entity & currentEntity)
////{
////	if (currentEntity.second.m_pathToTile.size() > m_entity.first->m_movementPoints + 1)
////	{
////		//Don't interact with path from source.
////		for (int i = 1; i < m_entity.first->m_movementPoints + 1; ++i)
////		{
////			auto tileScreenPosition = m_map.getTileScreenPos(m_entity.second.m_pathToTile[i]);
////			m_movementPath[i - 1].first->GetTransformComp().SetPosition({
////				static_cast<float>(tileScreenPosition.first + DRAW_OFFSET_X * m_map.getDrawScale()),
////				static_cast<float>(tileScreenPosition.second + DRAW_OFFSET_Y * m_map.getDrawScale()) });
////			m_movementPath[i - 1].second = true;
////		}
////	}
////	else
////	{
////		//Don't interact with path from source.
////		for (int i = 1; i < m_entity.second.m_pathToTile.size(); ++i)
////		{
////			auto tileScreenPosition = m_map.getTileScreenPos(m_entity.second.m_pathToTile[i]);
////			m_movementPath[i - 1].first->GetTransformComp().SetPosition({
////				static_cast<float>(tileScreenPosition.first + DRAW_OFFSET_X * m_map.getDrawScale()),
////				static_cast<float>(tileScreenPosition.second + DRAW_OFFSET_Y * m_map.getDrawScale()) });
////			m_movementPath[i - 1].second = true;
////		}
////	}
////}
//
//Map & Battle::getMap()
//{
//	return m_map;
//}
//
//BattleUI::BattleUI(Battle & battle)
//	: m_battle(battle)
//{
//}
//
//void BattleUI::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData & mouseData)
//{
//	if (mouseEvent == EMouseEvent::eLeftButtonDown)
//	{
//		if (m_currentTileSelected && m_currentTileSelected->m_entityOnTile)
//		{
//			//Tell battle where to move entity
//		}
//
//		//Tile* currentTile = m_map.getTile(m_map.getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
//		//if (!currentTile)
//		//{
//		//	return;
//		////}
//		//auto& map = m_battle.getMap();
//		//(m_currentTileSelected ? map.getTile(map.getMouseClickCoord(HAPI_Wrapper::getMouseLocation())) : nullptr);
//
//
//		
//	}
//}
//

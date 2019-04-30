#include "BattleUI.h"
#include "Battle.h"
#include "Pathfinding.h"
#include "OverWorld.h"
#include "Textures.h"
#include "MouseSelection.h"
#include "GameEventMessenger.h"
#include <assert.h>

using namespace HAPISPACE;
constexpr float DRAW_ENTITY_OFFSET_X{ 16 };
constexpr float DRAW_ENTITY_OFFSET_Y{ 32 };
constexpr int SHIP_PLACEMENT_SPAWN_RANGE{ 3 };

//
//InvalidPositionSprite
//
BattleUI::InvalidPosition::InvalidPosition()
	: m_sprite(std::make_unique<Sprite>(Textures::m_thing)),
	m_activate(false)
{
	m_sprite->GetTransformComp().SetOriginToCentreOfFrame();
}

void BattleUI::InvalidPosition::render(const Map& map) const
{
	if (m_activate)
	{
		m_sprite->GetTransformComp().SetPosition({
		static_cast<float>(m_position.first + DRAW_ENTITY_OFFSET_X * map.getDrawScale()),
		static_cast<float>(m_position.second + DRAW_ENTITY_OFFSET_Y * map.getDrawScale()) });

		m_sprite->Render(SCREEN_SURFACE);
	}
}

void BattleUI::InvalidPosition::setPosition(std::pair<int, int> screenPosition, const Map& map)
{
	m_sprite->GetTransformComp().SetPosition({
		(float)screenPosition.first + DRAW_OFFSET_X * map.getDrawScale(),
		(float)screenPosition.second + DRAW_OFFSET_Y * map.getDrawScale()});

	m_position = screenPosition;
}

//
//BattleUI
//
BattleUI::BattleUI(Battle & battle)
	: m_battle(battle),
	m_gui(),
	m_selectedTile(),
	m_invalidPosition(),
	m_leftMouseDownPosition({ 0, 0 }),
	m_isMovingEntity(false),
	m_mouseDownTile(nullptr),
	m_explosion(0.08, Textures::m_explosion),
	m_fire(0.02, Textures::m_fire)
{
	GameEventMessenger::getInstance().subscribe(std::bind(&BattleUI::onResetBattle, this), "BattleUI", GameEvent::eResetBattle);
	GameEventMessenger::getInstance().subscribe(std::bind(&BattleUI::onNewTurn, this), "BattleUI", GameEvent::eNewTurn);
}

BattleUI::~BattleUI()
{
	GameEventMessenger::getInstance().unsubscribe("BattleUI", GameEvent::eResetBattle);
	GameEventMessenger::getInstance().unsubscribe("BattleUI", GameEvent::eNewTurn);
}

std::pair<int, int> BattleUI::getCameraPositionOffset() const
{
	return m_gui.getCameraPositionOffset();
}

void BattleUI::renderUI() const
{
	switch (m_battle.getCurrentPhase())
	{
	case BattlePhase::ShipPlacement:
		//assert(!m_playerShipPlacement.empty());
		m_playerShipPlacement.front()->render(m_invalidPosition, m_battle.getMap());
		break;

	case BattlePhase::Movement:
		m_selectedTile.render(m_battle.getMap());
		break;
	
	case BattlePhase::Attack:
		m_selectedTile.render(m_battle.getMap());
		m_targetArea.render(m_battle.getMap());
		
		break;
	}

	m_invalidPosition.render(m_battle.getMap());
}

void BattleUI::renderParticles() const
{
	m_explosion.render();
	m_fire.render();
}

void BattleUI::renderGUI() const
{
	m_gui.render(m_battle.getCurrentPhase());

	if (m_selectedTile.m_tile  != nullptr && m_selectedTile.m_tile->m_entityOnTile != nullptr)
	{
		m_gui.renderStats(m_selectedTile.m_tile->m_entityOnTile->m_entityProperties);
	}
}


void BattleUI::loadGUI(std::pair<int, int> mapDimensions)
{
	m_gui.setMaxCameraOffset(mapDimensions);
}

void BattleUI::update(float deltaTime)
{
	m_gui.update(m_battle.getMap().getWindDirection());// added update for gui to receive wind direction so compass direction updates
	m_explosion.run(deltaTime, m_battle.getMap());
	m_fire.run(deltaTime, m_battle.getMap());
}

void BattleUI::FactionUpdateGUI(FactionName faction)
{
	m_gui.updateFactionToken(faction);
}

//void BattleUI::newPhase()
//{
//	
//}
//
//void BattleUI::newTurn(FactionName playersTurn)
//{
//
//}

void BattleUI::startShipPlacement(const std::vector<std::pair<FactionName, std::vector<EntityProperties*>>>& players, Map& map)
{
	assert(m_battle.getCurrentPhase() == BattlePhase::ShipPlacement);
	assert(m_playerShipPlacement.empty());

	//TODO: Change this at some point
	for (auto& player : players)
	{
		for (auto& entity : player.second)
		{
			switch (player.first)
			{
			case FactionName::eYellow:
				//entity->m_sprite->SetFrameNumber(eShipSpriteFrame::eMaxHealthYellow);
				entity->m_sprite->GetTransformComp().SetOriginToCentreOfFrame();
				entity->m_sprite->GetTransformComp().SetScaling({ 1, 1 });
				break;
			case FactionName::eBlue:
				//entity->m_sprite->SetFrameNumber(eShipSpriteFrame::eMaxHealthBlue);
				entity->m_sprite->GetTransformComp().SetOriginToCentreOfFrame();
				entity->m_sprite->GetTransformComp().SetScaling({ 1, 1 });
				break;
			case FactionName::eRed:
				//entity->m_sprite->SetFrameNumber(eShipSpriteFrame::eMaxHealthRed);
				entity->m_sprite->GetTransformComp().SetOriginToCentreOfFrame();
				entity->m_sprite->GetTransformComp().SetScaling({ 1, 1 });
				break;
			case FactionName::eGreen:
				//entity->m_sprite->SetFrameNumber(eShipSpriteFrame::eMaxHealthGreen);
				entity->m_sprite->GetTransformComp().SetOriginToCentreOfFrame();
				entity->m_sprite->GetTransformComp().SetScaling({ 1, 1 });
				break;
			}
		}
	}

	for (const auto& player : players)
	{
		m_playerShipPlacement.push_back(std::make_unique<ShipPlacementPhase>
			(player.second, map.getSpawnPosition(), SHIP_PLACEMENT_SPAWN_RANGE, m_battle.getMap(), player.first));
	}
}

void BattleUI::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData & mouseData)
{
	if (OverWorldGUI::CURRENT_WINDOW != OverWorldWindow::eBattle)
	{
		return;
	}
	if (mouseEvent == EMouseEvent::eLeftButtonDown)
	{
		m_gui.OnMouseLeftClick(mouseData, m_battle.getCurrentPhase());

		switch (m_battle.getCurrentPhase())
		{
		case BattlePhase::ShipPlacement :
		{
			m_isMovingEntity = true;
			m_leftMouseDownPosition = { mouseData.x, mouseData.y };
			break;
		}
		case BattlePhase::Movement:
		{
			m_leftMouseDownPosition = { mouseData.x, mouseData.y };
			onLeftClickMovementPhase();
			break;
		}
		case BattlePhase::Attack :
		{
			onLeftClickAttackPhase();
			break;
		}
		}
	}
	else if (mouseEvent == EMouseEvent::eRightButtonDown)
	{
		switch (m_battle.getCurrentPhase())
		{
		case BattlePhase::Movement :
		{
			onRightClickMovementPhase();
			break;
		}
		case BattlePhase::Attack :
		{
			onRightClickAttackPhase();
			break;
		}
		}
	}
	else if (mouseEvent == EMouseEvent::eLeftButtonUp)
	{
		if (m_isMovingEntity)
		{
			std::pair<double, eDirection> mouseMoveDirection{ MouseSelection::calculateDirection(m_leftMouseDownPosition,HAPI_Wrapper::getMouseLocation()) };
			switch (m_battle.getCurrentPhase())
			{
			case BattlePhase::ShipPlacement :
			{
				assert(!m_playerShipPlacement.empty());
				if (!m_selectedTile.m_tile)
					break;
				if (mouseMoveDirection.first > 20)
				{
					//This function call will be used if the mouse moved a significant enough distance during inputing a move command to assume it was on purpose, it sends not only the
					//destination tile but the direction of the aformentioned movement.  In order for this to work the pathfinding must be capable of taking an eDirection as part of the
					//function used by the battle to move the entity.
					m_playerShipPlacement.front()->onLeftClick(m_invalidPosition, mouseMoveDirection.second, m_selectedTile.m_tile, m_battle);
					if (m_playerShipPlacement.front()->isCompleted())
					{
						m_playerShipPlacement.pop_front();
					}
					//m_battle.moveEntityToPosition(*m_selectedTile.m_tile->m_entityOnTile, *m_battle.getMap().getTile(m_leftMouseDownPosition), mouseMoveDirection.second);
				}
				else
				{
					//This function call is to be used if the movement of the mouse during the move command is small enough to be considered unintended,
					//in this case the ship should not rotate after reaching the destination.
					m_playerShipPlacement.front()->onLeftClick(m_invalidPosition, eNorth, m_selectedTile.m_tile, m_battle);
					if (m_playerShipPlacement.front()->isCompleted())
					{
						m_playerShipPlacement.pop_front();
					}
				}
				break;
			}
			case BattlePhase::Movement :
			{
				if (!m_mouseDownTile || !m_selectedTile.m_tile)
				{
					break;
				}
				if (mouseMoveDirection.first > 20)
				{
					m_battle.moveEntityToPosition(*m_selectedTile.m_tile->m_entityOnTile, *m_mouseDownTile, mouseMoveDirection.second);
				}
				else
				{
					m_battle.moveEntityToPosition(*m_selectedTile.m_tile->m_entityOnTile, *m_mouseDownTile);
				}
				break;
			}
			}
			//Resetting the variables used as triggers
			m_mouseDownTile = nullptr;
			//TODO: Drop info box
			m_selectedTile.m_tile = nullptr;
			m_isMovingEntity = false;
		}
	}
}

void BattleUI::OnMouseMove(const HAPI_TMouseData & mouseData)
{
	if (OverWorldGUI::CURRENT_WINDOW != OverWorldWindow::eBattle)
	{
		return;
	}

	m_gui.OnMouseMove(mouseData, m_battle.getCurrentPhase());

	switch (m_battle.getCurrentPhase())
	{
	case BattlePhase::ShipPlacement:
	{
		assert(!m_playerShipPlacement.empty());
		if (!m_isMovingEntity)
		{
			m_selectedTile.m_tile = m_playerShipPlacement.front()->getTileOnMouse(
				m_invalidPosition, m_selectedTile.m_tile, m_battle.getMap());
			//TODO: Raise info box
		}
		break;
	}
	case BattlePhase::Movement:
	{
		onMouseMoveMovementPhase();
		break;
	}
	case BattlePhase::Attack :
	{
		onMouseMoveAttackPhase();
		break;
	}
	}
}

void BattleUI::setCurrentFaction(FactionName faction)
{
	FactionUpdateGUI(faction);
}

void BattleUI::onMouseMoveMovementPhase()
{
	assert(m_battle.getCurrentPhase() == BattlePhase::Movement);

	//Current tile selected does not match the current player in play
	if (m_selectedTile.m_tile && m_selectedTile.m_tile->m_entityOnTile 
		&& m_selectedTile.m_tile->m_entityOnTile->m_factionName != m_battle.getCurentFaction())
	{
		return;
	}

	//!isMovingToDestination && !reachedDestination
	if (m_selectedTile.m_tile && m_selectedTile.m_tile->m_entityOnTile &&  
		!m_selectedTile.m_tile->m_entityOnTile->m_battleProperties.isMovingToDestination() &&
		!m_selectedTile.m_tile->m_entityOnTile->m_battleProperties.isDestinationSet())
	{
		const Tile* tile = m_battle.getMap().getTile(m_battle.getMap().getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
		if (!tile)
		{
			return;
		}

		if (m_selectedTile.m_tile->m_tileCoordinate != tile->m_tileCoordinate && !m_mouseDownTile)
		{
			if (tile->m_type != eTileType::eSea && tile->m_type != eTileType::eOcean)
			{
				m_invalidPosition.setPosition(m_battle.getMap().getTileScreenPos(tile->m_tileCoordinate), m_battle.getMap());
				m_invalidPosition.m_activate = true;
				m_selectedTile.m_tile->m_entityOnTile->m_battleProperties.clearMovementPath();
			}
			else
			{
				m_selectedTile.m_tile->m_entityOnTile->m_battleProperties.generateMovementGraph(m_battle.getMap(), *m_selectedTile.m_tile, *tile);
				m_invalidPosition.m_activate = false;
			}
		}
	}
}

void BattleUI::onLeftClickMovementPhase()
{
	assert(m_battle.getCurrentPhase() == BattlePhase::Movement);

	const Tile* tileOnMouse = m_battle.getMap().getTile(m_battle.getMap().getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
	if (!tileOnMouse)
	{
		return;
	}

	if (!m_selectedTile.m_tile && tileOnMouse->m_entityOnTile)
	{
		m_selectedTile.m_tile = tileOnMouse;
		return;
	}

	if (!m_selectedTile.m_tile && tileOnMouse->m_entityOnTile &&
		tileOnMouse->m_entityOnTile->m_factionName != m_battle.getCurentFaction())
	{
		m_selectedTile.m_tile = tileOnMouse;
		return;
	}

	//Invalid Location - Collidable tile
	if (tileOnMouse->m_type != eTileType::eSea && tileOnMouse->m_type != eTileType::eOcean)
	{
		if (m_selectedTile.m_tile && m_selectedTile.m_tile->m_entityOnTile)
		{
			m_selectedTile.m_tile->m_entityOnTile->m_battleProperties.clearMovementPath();
		}
		return;
	}

	//Do not select killed entity
	if (tileOnMouse->m_entityOnTile && tileOnMouse->m_entityOnTile->m_battleProperties.isDead())
	{
		//TODO: Drop info box
		m_selectedTile.m_tile = nullptr;
		return;
	}

	//Clicking to where entity is moving to
	if (tileOnMouse->m_entityOnTile && tileOnMouse->m_entityOnTile->m_battleProperties.isDestinationSet())
	{
		//TODO: Drop info box
		m_selectedTile.m_tile = nullptr;
		return;
	}

	if (m_selectedTile.m_tile)
	{
		//Cancel movement if clicked on same entity
		if (m_selectedTile.m_tile->m_tileCoordinate == tileOnMouse->m_tileCoordinate)
		{
			m_selectedTile.m_tile->m_entityOnTile->m_battleProperties.clearMovementPath();
			//TODO: Drop info box
			m_selectedTile.m_tile = nullptr;
		}

		//Disallow movement to tile occupied by other player
		else if (tileOnMouse->m_entityOnTile && tileOnMouse->m_entityOnTile->m_factionName != m_battle.getCurentFaction())
		{
			m_selectedTile.m_tile->m_entityOnTile->m_battleProperties.clearMovementPath();
			m_selectedTile.m_tile = tileOnMouse;
		}
		else if (tileOnMouse->m_entityOnTile && tileOnMouse->m_entityOnTile->m_factionName == m_battle.getCurentFaction())
		{
			m_selectedTile.m_tile->m_entityOnTile->m_battleProperties.clearMovementPath();
			m_selectedTile.m_tile = tileOnMouse;
		}

		//Store data so Entity can move to new location
		else if (m_selectedTile.m_tile->m_entityOnTile && (m_selectedTile.m_tile->m_tileCoordinate != tileOnMouse->m_tileCoordinate)
			&& m_selectedTile.m_tile->m_entityOnTile->m_factionName == m_battle.getCurentFaction())
		{
			assert(m_selectedTile.m_tile->m_entityOnTile->m_factionName == m_battle.getCurentFaction());
			m_mouseDownTile = tileOnMouse;
			m_isMovingEntity = true;
			//m_battle.moveEntityToPosition(*m_selectedTile.m_tile->m_entityOnTile, *tileOnMouse);
			//m_selectedTile.m_tile = nullptr;
		}
	}
	else
	{
		if (tileOnMouse->m_entityOnTile && tileOnMouse->m_entityOnTile->m_battleProperties.isDead())
		{
			//TODO: Drop info box
			m_selectedTile.m_tile = nullptr;
		}
		//Do not select tile that contains wrong players entity
		if (tileOnMouse->m_entityOnTile)
		{
			if (tileOnMouse->m_entityOnTile->m_factionName != m_battle.getCurentFaction())
			{
				//TODO: Drop info box
				m_selectedTile.m_tile = nullptr;
			}
			else
			{
				//TODO: Raise info box
				m_selectedTile.m_tile = tileOnMouse;
			}
		}
	}
}

void BattleUI::onRightClickMovementPhase()
{
	assert(m_battle.getCurrentPhase() == BattlePhase::Movement);
	//Cancel selected Entity
	if (m_selectedTile.m_tile && m_selectedTile.m_tile->m_entityOnTile)
	{
		m_selectedTile.m_tile->m_entityOnTile->m_battleProperties.clearMovementPath();
		m_invalidPosition.m_activate = false;
	}
	//TODO: Drop info box
	m_selectedTile.m_tile = nullptr;
}

void BattleUI::onLeftClickAttackPhase()
{
	//assert(m_battle.getCurrentPhase() == BattlePhase::Attack);
	//const Tile* tileOnMouse = m_battle.getMap().getTile(m_battle.getMap().getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
	//if (!tileOnMouse || !tileOnMouse->m_entityOnTile)
	//{
	//	if (m_selectedTile.m_tile)
	//	{
	//		m_targetArea.clearTargetArea();
	//	}
	//	m_selectedTile.m_tile = nullptr;
	//	m_invalidPosition.m_activate = false;
	//	return;
	//}

	assert(m_battle.getCurrentPhase() == BattlePhase::Attack);
	const Tile* tileOnMouse = m_battle.getMap().getTile(m_battle.getMap().getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
	if (!tileOnMouse)
	{
		return;
	}

	//Select new entity that is on same team
	if (m_selectedTile.m_tile && m_selectedTile.m_tile->m_entityOnTile && tileOnMouse->m_entityOnTile &&
		(tileOnMouse->m_entityOnTile->m_factionName == m_battle.getCurentFaction()))
	{
		if (!tileOnMouse->m_entityOnTile->m_battleProperties.isWeaponFired())
		{
			m_targetArea.clearTargetArea();
			m_targetArea.generateTargetArea(m_battle.getMap(), *tileOnMouse);
			m_selectedTile.m_tile = tileOnMouse;
			return;
		}
	}

	if (!m_selectedTile.m_tile && tileOnMouse->m_entityOnTile && tileOnMouse->m_entityOnTile->m_factionName != m_battle.getCurentFaction())
	{
		m_selectedTile.m_tile = tileOnMouse;
		return;
	}

	//Do not fire on destroyed ship
	if (tileOnMouse->m_entityOnTile && tileOnMouse->m_entityOnTile->m_battleProperties.isDead())
	{
		m_targetArea.clearTargetArea();
		m_invalidPosition.m_activate = false;
		//TODO: Drop info box
		m_selectedTile.m_tile = nullptr;
		return;
	}

	//Clicking on the same entity what has been previously selected
	if (m_selectedTile.m_tile && m_selectedTile.m_tile->m_tileCoordinate == tileOnMouse->m_tileCoordinate)
	{
		m_targetArea.clearTargetArea();
		m_invalidPosition.m_activate = false;
		//TODO: Drop info box
		m_selectedTile.m_tile = nullptr;
		return;
	}

	//Entity already selected Fire weapon at position
	if (m_selectedTile.m_tile && m_selectedTile.m_tile->m_entityOnTile && !m_selectedTile.m_tile->m_entityOnTile->m_battleProperties.isWeaponFired())
	{
		if ((tileOnMouse->m_entityOnTile != nullptr) && tileOnMouse->m_entityOnTile->m_factionName != m_selectedTile.m_tile->m_entityOnTile->m_factionName)
		{
			if (m_selectedTile.m_tile->m_entityOnTile->m_entityProperties.m_weaponType == eFlamethrower)
			{
				m_fire.orient(m_selectedTile.m_tile->m_entityOnTile->m_battleProperties.getCurrentDirection());
				m_fire.setPosition(m_targetArea.m_targetArea[0]->m_tileCoordinate);
				m_fire.m_isEmitting = true;
			}
			else
			{
				m_explosion.setPosition(tileOnMouse->m_entityOnTile->m_battleProperties.getCurrentPosition());
				m_explosion.m_isEmitting = true;
			}

		}

		m_battle.fireEntityWeaponAtPosition(*m_selectedTile.m_tile->m_entityOnTile, *tileOnMouse, m_targetArea.m_targetArea);
		m_targetArea.clearTargetArea();
		m_selectedTile.m_tile = nullptr;
		m_invalidPosition.m_activate = false;
		return;
	}

	//Entity Already Selected whilst showing where to fire
	//Change to different Entity before firing
	if (tileOnMouse->m_entityOnTile && tileOnMouse->m_entityOnTile->m_factionName == m_battle.getCurentFaction()
		&& m_selectedTile.m_tile && m_selectedTile.m_tile && m_targetArea.m_targetArea.size() > 0)
	{
		m_targetArea.clearTargetArea();
		m_targetArea.generateTargetArea(m_battle.getMap(), *tileOnMouse);
		//TODO: Raise info box
		m_selectedTile.m_tile = tileOnMouse;
		return;
	}

	//Click on same
	//Select new Entity to fire at something
	if (tileOnMouse->m_entityOnTile && tileOnMouse->m_entityOnTile->m_factionName != m_battle.getCurentFaction())
	{
		m_selectedTile.m_tile = tileOnMouse;
		return;
	}

	if (!m_selectedTile.m_tile && tileOnMouse->m_entityOnTile && tileOnMouse->m_entityOnTile->m_factionName == m_battle.getCurentFaction())
	{
		m_selectedTile.m_tile = tileOnMouse;
	}

	if (m_selectedTile.m_tile && m_selectedTile.m_tile->m_entityOnTile && !m_selectedTile.m_tile->m_entityOnTile->m_battleProperties.isWeaponFired() &&
		m_selectedTile.m_tile->m_entityOnTile->m_factionName == m_battle.getCurentFaction())
	{
		m_selectedTile.m_tile = tileOnMouse;
		m_targetArea.generateTargetArea(m_battle.getMap(), *tileOnMouse);
	}
}

void BattleUI::onRightClickAttackPhase()
{
	//TODO: Drop info box
	m_selectedTile.m_tile = nullptr;
	m_invalidPosition.m_activate = false;
	m_targetArea.clearTargetArea();
}

void BattleUI::onMouseMoveAttackPhase()
{
	assert(m_battle.getCurrentPhase() == BattlePhase::Attack);
	const Tile* tileOnMouse = m_battle.getMap().getTile(m_battle.getMap().getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
	if (!tileOnMouse)
	{
		return;
	}

	auto tileCoordinate = tileOnMouse->m_tileCoordinate;
	//tileOnMouse in new position
	if (m_selectedTile.m_tile && m_selectedTile.m_tile->m_tileCoordinate != tileOnMouse->m_tileCoordinate)
	{
		//TODO: this does not work if some of the tiles are nullptr!
		auto cIter = std::find_if(m_targetArea.m_targetArea.cbegin(), m_targetArea.m_targetArea.cend(),
			[tileCoordinate](const auto& tile) { 
				if (tile != nullptr)
				{
					return tileCoordinate == tile->m_tileCoordinate;
				}
			});
		//tileOnMouse within weapon range
		if (cIter != m_targetArea.m_targetArea.cend())
		{
			m_invalidPosition.setPosition(m_battle.getMap().getTileScreenPos(tileOnMouse->m_tileCoordinate), m_battle.getMap());
			m_invalidPosition.m_activate = true;
		}
		//outside weapon range
		else
		{
			m_invalidPosition.m_activate = false;
		}
	}
}

void BattleUI::onResetBattle()
{
	//TODO: reset other things
	m_playerShipPlacement.clear();
	m_targetArea.clearTargetArea();
	m_selectedTile.m_tile = nullptr;
	m_invalidPosition.m_activate = false;
}

void BattleUI::onNewTurn()
{
	//TODO: Drop info box
	m_selectedTile.m_tile = nullptr;
}

//Weapon Graph
BattleUI::TargetArea::TargetArea()
{
	m_targetAreaSprites.reserve(size_t(300));
	for (int i = 0; i < 300; ++i)
	{
		m_targetAreaSprites.push_back({});
	}
}

void BattleUI::TargetArea::render(const Map& map) const
{
	for (const auto& i : m_targetAreaSprites)
	{
		if (i.activate)
		{
			const std::pair<int, int> tileTransform = map.getTileScreenPos(i.position);

			i.sprite->GetTransformComp().SetPosition({
			static_cast<float>(tileTransform.first + DRAW_ENTITY_OFFSET_X * map.getDrawScale()),
			static_cast<float>(tileTransform.second + DRAW_ENTITY_OFFSET_Y * map.getDrawScale()) });

			i.sprite->Render(SCREEN_SURFACE);
		}
	}
}

void BattleUI::TargetArea::generateTargetArea(const Map & map, const Tile & source)
{
	
	if (source.m_entityOnTile->m_entityProperties.m_weaponType == eSideCannons)
	{
		m_targetArea = map.cGetTileCone(source.m_tileCoordinate,
			source.m_entityOnTile->m_entityProperties.m_range, 
			source.m_entityOnTile->m_battleProperties.getCurrentDirection());
	
	}

	else if (source.m_entityOnTile->m_entityProperties.m_weaponType == eStraightShot)
	{
		m_targetArea = map.cGetTileLine(source.m_tileCoordinate, 
			source.m_entityOnTile->m_entityProperties.m_range, 
			source.m_entityOnTile->m_battleProperties.getCurrentDirection());

	}

	else if (source.m_entityOnTile->m_entityProperties.m_weaponType == eShotgun)
	{
		// make so where ever the place presses get radius called talk adrais about size of that
		m_targetArea = map.cGetTileRadius(source.m_tileCoordinate, 
			source.m_entityOnTile->m_entityProperties.m_range);
	}

	else if (source.m_entityOnTile->m_entityProperties.m_weaponType == eFlamethrower)
	{
		eDirection directionOfFire;
		switch (source.m_entityOnTile->m_battleProperties.getCurrentDirection() )
		{
		case eNorth:
			directionOfFire = eSouth;
			break;
		case eNorthEast:
			directionOfFire = eSouthWest;
			break;
		case eSouthEast:
			directionOfFire = eNorthWest;
			break;
		case eSouth:
			directionOfFire = eNorth;
			break;
		case eSouthWest:
			directionOfFire = eNorthEast;
			break;
		case eNorthWest:
			directionOfFire = eSouthEast;
			break;
		}
		m_targetArea = map.cGetTileLine(source.m_tileCoordinate,
			source.m_entityOnTile->m_entityProperties.m_range, directionOfFire);
	}
	
	if (m_targetArea.empty())
	{
		return;
	}
	clearTargetArea();
	assert(!m_targetAreaSprites.empty());
	//using same convention as movement // from source should be able to get position
	for (int i = 0; i < m_targetArea.size(); i++)
	{
		if (!m_targetArea[i])//Check that i is not nullptr
			continue;

		std::pair<int, int>tilePos = map.getTileScreenPos(m_targetArea[i]->m_tileCoordinate);
		m_targetAreaSprites[i].sprite->GetTransformComp().SetPosition(
			{
				 tilePos.first + 12 * map.getDrawScale(),
				 tilePos.second + 28 * map.getDrawScale()
			});

		m_targetAreaSprites[i].activate = true;
		m_targetAreaSprites[i].position = m_targetArea[i]->m_tileCoordinate;
	}
}

void BattleUI::TargetArea::clearTargetArea()
{
	for (auto& i : m_targetAreaSprites)
	{
		i.activate = false;
	}
}

BattleUI::TargetArea::HighlightNode::HighlightNode()
	: sprite(std::make_unique<Sprite>(Textures::m_mouseCrossHair)),
	activate(false)
{
	sprite->GetTransformComp().SetOriginToCentreOfFrame();
}

BattleUI::ShipPlacementPhase::ShipPlacementPhase(std::vector<EntityProperties*> player, 
	std::pair<int, int> spawnPosition, int range, const Map& map, FactionName factionName)
	: m_factionName(factionName),
	m_player(player),
	m_currentSelectedEntity(),
	m_spawnArea(),
	m_spawnSprites()
{
	//Might change this - for now its two containers but looks confusing
	m_spawnArea = map.cGetTileRadius(spawnPosition, range, true);
	m_spawnSprites.reserve(m_spawnArea.size());
	for (int i = 0; i < m_spawnArea.size(); ++i)
	{
		std::unique_ptr<Sprite> sprite;
		switch (factionName)
		{
		case eYellow:
			sprite = HAPI_Sprites.MakeSprite(Textures::m_yellowSpawnHex);
			break;
		case eBlue:
			sprite = HAPI_Sprites.MakeSprite(Textures::m_blueSpawnHex);
			break;
		case eGreen:
			sprite = HAPI_Sprites.MakeSprite(Textures::m_greenSpawnHex);
			break;
		case eRed:
			sprite = HAPI_Sprites.MakeSprite(Textures::m_redSpawnHex);
			break;
		};

		auto screenPosition = map.getTileScreenPos(m_spawnArea[i]->m_tileCoordinate);
		sprite->GetTransformComp().SetPosition({
		(float)screenPosition.first + DRAW_ENTITY_OFFSET_X * map.getDrawScale(),
		(float)screenPosition.second + DRAW_ENTITY_OFFSET_Y * map.getDrawScale() });
		sprite->GetTransformComp().SetOriginToCentreOfFrame();
		sprite->GetTransformComp().SetScaling({ 2.f, 2.f });

		m_spawnSprites.push_back(std::move(sprite));
	}

	for (int i = 0; i < m_spawnArea.size(); ++i)
	{
		//const std::pair<int, int> tileTransform = map.getTileScreenPos(m_tile->m_tileCoordinate);

		//m_sprite->GetTransformComp().SetPosition({
		//static_cast<float>(tileTransform.first + DRAW_ENTITY_OFFSET_X * map.getDrawScale()),
		//static_cast<float>(tileTransform.second + DRAW_ENTITY_OFFSET_Y * map.getDrawScale()) });

		auto screenPosition = map.getTileScreenPos(m_spawnArea[i]->m_tileCoordinate);
		m_spawnSprites[i]->GetTransformComp().SetPosition({
		(float)screenPosition.first + DRAW_ENTITY_OFFSET_X * map.getDrawScale() ,
		(float)screenPosition.second + DRAW_ENTITY_OFFSET_Y * map.getDrawScale() });
		m_spawnSprites[i]->GetTransformComp().SetOriginToCentreOfFrame();
		m_spawnSprites[i]->GetTransformComp().SetScaling({ 2.f, 2.f });
	}

	m_currentSelectedEntity.m_currentSelectedEntity = m_player.back();
}

bool BattleUI::ShipPlacementPhase::isCompleted() const
{
	return m_player.empty();
}

void BattleUI::ShipPlacementPhase::render(const InvalidPosition& invalidPosition, const Map& map) const
{
	if (m_currentSelectedEntity.m_currentSelectedEntity && !invalidPosition.m_activate)
	{
		const std::pair<int, int> tileTransform = map.getTileScreenPos(m_currentSelectedEntity.m_position);

		m_currentSelectedEntity.m_currentSelectedEntity->m_sprite->GetTransformComp().SetPosition({
		static_cast<float>(tileTransform.first + DRAW_ENTITY_OFFSET_X * map.getDrawScale()),
		static_cast<float>(tileTransform.second + DRAW_ENTITY_OFFSET_Y * map.getDrawScale()) });

		m_currentSelectedEntity.m_currentSelectedEntity->m_sprite->Render(SCREEN_SURFACE);
	}

	for (int i = 0; i < m_spawnSprites.size(); ++i)
	{
		const std::pair<int, int> tileTransform = map.getTileScreenPos(m_spawnArea[i]->m_tileCoordinate);

		m_spawnSprites[i]->GetTransformComp().SetPosition({
		static_cast<float>(tileTransform.first + DRAW_ENTITY_OFFSET_X * map.getDrawScale()),
		static_cast<float>(tileTransform.second + DRAW_ENTITY_OFFSET_Y * map.getDrawScale()) });

		m_spawnSprites[i]->Render(SCREEN_SURFACE);
	}

	//for (const auto& spawnHex : m_spawnSprites)
	//{
	//	const std::pair<int, int> tileTransform = map.getTileScreenPos(m_currentSelectedEntity.m_position);

	//	m_currentSelectedEntity.m_currentSelectedEntity->m_sprite->GetTransformComp().SetPosition({
	//	static_cast<float>(tileTransform.first + DRAW_ENTITY_OFFSET_X * map.getDrawScale()),
	//	static_cast<float>(tileTransform.second + DRAW_ENTITY_OFFSET_Y * map.getDrawScale()) });

	//	spawnHex->Render(SCREEN_SURFACE);
	//}
}

const Tile* BattleUI::ShipPlacementPhase::getTileOnMouse(InvalidPosition& invalidPosition, const Tile* currentTileSelected, const Map& map)
{
	const Tile* tileOnMouse = map.getTile(map.getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
	if (!tileOnMouse)
	{
		return nullptr;
	}

	if (!currentTileSelected)
	{
		return tileOnMouse;
	}

	//If new tile is in new position than old tile
	if (currentTileSelected->m_tileCoordinate != tileOnMouse->m_tileCoordinate)
	{
		currentTileSelected = tileOnMouse;
		
		//Cannot place ship on existing ship
		if (tileOnMouse->m_entityOnTile)
		{
			invalidPosition.setPosition(map.getTileScreenPos(tileOnMouse->m_tileCoordinate), map);
			invalidPosition.m_activate = true;
			return tileOnMouse;
		}

		auto tileCoordinate = tileOnMouse->m_tileCoordinate;
		auto iter = std::find_if(m_spawnArea.begin(), m_spawnArea.end(), [tileCoordinate](const auto& tile) { return tileCoordinate == tile->m_tileCoordinate; });
		if (iter != m_spawnArea.cend())
		{
			const std::pair<int, int> tileTransform = map.getTileScreenPos(tileOnMouse->m_tileCoordinate);
			
			m_currentSelectedEntity.m_currentSelectedEntity->m_sprite->GetTransformComp().SetPosition({
				static_cast<float>(tileTransform.first + DRAW_ENTITY_OFFSET_X * map.getDrawScale()),
				static_cast<float>(tileTransform.second + DRAW_ENTITY_OFFSET_Y * map.getDrawScale()) });
			m_currentSelectedEntity.m_position = tileOnMouse->m_tileCoordinate;

			invalidPosition.m_activate = false;
		}
		else
		{
			invalidPosition.setPosition(map.getTileScreenPos(tileOnMouse->m_tileCoordinate), map);
			invalidPosition.m_activate = true;
		}
	}

	return tileOnMouse;
}

void BattleUI::ShipPlacementPhase::onLeftClick(const InvalidPosition& invalidPosition, eDirection startingDirection, const Tile* currentTileSelected, Battle& battle)
{
	if (!currentTileSelected)
	{
		return;
	}
	//Disallow spawning on land
	if (currentTileSelected->m_type != eTileType::eSea && currentTileSelected->m_type != eTileType::eOcean)
	{
		return;
	}
	if (!invalidPosition.m_activate && !currentTileSelected->m_entityOnTile)
	{
		battle.insertEntity(currentTileSelected->m_tileCoordinate, startingDirection, *m_currentSelectedEntity.m_currentSelectedEntity, m_factionName);
		//Change ordering around to pop front with different container
		m_player.pop_back();
		if (m_player.empty())
		{
			battle.nextTurn();
		}
		else
		{
			m_currentSelectedEntity.m_currentSelectedEntity = m_player.back();
		}
	}
}

//Current Selected Tile
BattleUI::CurrentSelectedTile::CurrentSelectedTile()
	: m_sprite(std::make_unique<Sprite>(Textures::m_selectedHex)),
	m_tile(nullptr),
	m_position()
{
	m_sprite->GetTransformComp().SetOriginToCentreOfFrame();
	m_sprite->GetTransformComp().SetScaling({ 1.9f, 1.9f });
}

void BattleUI::CurrentSelectedTile::render(const Map & map) const
{
	if (m_tile && (m_tile->m_type == eTileType::eSea || m_tile->m_type == eTileType::eOcean))
	{
		const std::pair<int, int> tileTransform = map.getTileScreenPos(m_tile->m_tileCoordinate);

		m_sprite->GetTransformComp().SetPosition({
		static_cast<float>(tileTransform.first + DRAW_ENTITY_OFFSET_X * map.getDrawScale()),
		static_cast<float>(tileTransform.second + DRAW_ENTITY_OFFSET_Y * map.getDrawScale()) });

		m_sprite->Render(SCREEN_SURFACE);
	}
}

BattleUI::ParticleSystem::ParticleSystem(float lifespan, std::shared_ptr<HAPISPACE::SpriteSheet> texture) :
	m_position(),
	m_lifeSpan(lifespan),
	m_particle(HAPI_Sprites.MakeSprite(texture)),
	m_frameNum(0),
	m_isEmitting(false)
{
	m_particle->SetFrameNumber(m_frameNum);
}

void BattleUI::ParticleSystem::setPosition(std::pair<int, int> position)
{
	m_position = position;
}

void BattleUI::ParticleSystem::run(float deltaTime, const Map& map) 
{

	if (m_isEmitting)
	{
		const std::pair<int, int> tileTransform = map.getTileScreenPos(m_position);
		m_particle->GetTransformComp().SetPosition({
			tileTransform.first + DRAW_ENTITY_OFFSET_X * map.getDrawScale(),
			tileTransform.second + DRAW_ENTITY_OFFSET_Y * map.getDrawScale() });

		m_lifeSpan.update(deltaTime);

		if (m_lifeSpan.isExpired())
		{
			m_particle->SetFrameNumber(m_frameNum);
			
			m_lifeSpan.reset();
			++m_frameNum;
		}

		if (m_frameNum >= m_particle->GetNumFrames())
		{
			m_isEmitting = false;
			m_frameNum = 0;
		}
	}

}

void BattleUI::ParticleSystem::render()const 
{
	if (m_isEmitting)
	{
		m_particle->GetTransformComp().SetOriginToCentreOfFrame();
		m_particle->GetTransformComp().SetScaling(1.5);
		m_particle->Render(SCREEN_SURFACE);
	}
}

void BattleUI::ParticleSystem::orient(eDirection entityDir)
{
	eDirection direction;
	switch (entityDir)
	{
	case eNorth:
		direction = eSouth;
		break;
	case eNorthEast:
		direction = eSouthWest;
		break;
	case eSouthEast:
		direction = eNorthWest;
		break;
	case eSouth:
		direction = eNorth;
		break;
	case eSouthWest:
		direction = eNorthEast;
		break;
	case eNorthWest:
		direction = eSouthEast;
		break;
	}
	m_particle->GetTransformComp().SetRotation(DEGREES_TO_RADIANS(static_cast<int>(direction) * 60 % 360));
}

std::pair<int, int> BattleUI::ShipPlacementPhase::getSpawnPosition() const
{
	return m_spawnPosition;
}
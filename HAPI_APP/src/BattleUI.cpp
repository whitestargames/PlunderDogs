#include "Global.h"
#include "BattleUI.h"
#include "HAPIWrapper.h"
#include "Battle.h"
#include "Pathfinding.h"
#include "OverWorld.h"
#include "OverWorldGUI.h"
#include "Utilities/Utilities.h"
#include "HAPIWrapper.h"
#include "Textures.h"
#include "MouseSelection.h"
#include <assert.h>

using namespace HAPISPACE;
constexpr float DRAW_ENTITY_OFFSET_X{ 16 };
constexpr float DRAW_ENTITY_OFFSET_Y{ 32 };

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
BattleUI::BattleUI(Battle & battle, std::vector<EntityProperties*>& player1, std::vector<EntityProperties*>& player2)
	: m_battle(battle),
	m_gui({ battle.getMap().getDimensions().first * 28 - 150, battle.getMap().getDimensions().second * 32 - 150}),
	m_currentTileSelected(nullptr),
	m_invalidPosition()
{

	assert(m_battle.getCurrentPhase() == BattlePhase::ShipPlacement);
	//Player Spawn Positions
	//TODO: Get them parsed with map instead of hardcoded
	std::pair<int, int> player1SpawnPos{ 4, 11 };
	m_playerShipPlacement.push_back(std::make_unique<ShipPlacementPhase>(player1, player1SpawnPos, 4, m_battle.getMap(), PlayerName::Player1));
	std::pair<int, int> player2SpawnPos{ 22, 2 };
	m_playerShipPlacement.push_back(std::make_unique<ShipPlacementPhase>(player2, player2SpawnPos, 4, m_battle.getMap(), PlayerName::Player2));

	//Hack to make sprites position correctly
	//TODO: Will change at some point
	for (auto& i : player1)
	{
		i->m_sprite->GetTransformComp().SetOriginToCentreOfFrame(); //.SetOrigin({ 13, 25 });
		i->m_sprite->GetTransformComp().SetScaling({ 1,1 });
	}
	for (auto& i : player2)
	{
		i->m_sprite->GetTransformComp().SetOriginToCentreOfFrame();//.SetOrigin({ 13, 25 });
		i->m_sprite->GetTransformComp().SetScaling({ 1,1 });
	}
}

std::pair<int, int> BattleUI::getCameraPositionOffset() const
{
	return m_gui.getCameraPositionOffset();
}

void BattleUI::render() const
{
	switch (m_battle.getCurrentPhase())
	{
	case BattlePhase::ShipPlacement:
	{
		assert(!m_playerShipPlacement.empty());
		m_playerShipPlacement.front()->render(m_invalidPosition, m_battle.getMap());
		break;
	}
	case BattlePhase::Attack:
	{
		m_targetArea.render(m_battle.getMap());
		break;
	}
	}

	m_invalidPosition.render(m_battle.getMap());
	m_gui.render();
}

void BattleUI::update()
{
	m_gui.update();
}

void BattleUI::newPhase()
{
	m_currentTileSelected = nullptr;
}

void BattleUI::newTurn(PlayerName playersTurn)
{
	if (m_battle.getCurrentPhase() == BattlePhase::ShipPlacement)
	{
		for (auto iter = m_playerShipPlacement.begin(); iter != m_playerShipPlacement.end(); ++iter)
		{
			if ((*iter)->isCompleted())
			{
				m_playerShipPlacement.erase(iter);
				break;
			}
		}
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
		m_gui.OnMouseLeftClick(mouseData);

		switch (m_battle.getCurrentPhase())
		{
		case BattlePhase::ShipPlacement :
		{
			assert(!m_playerShipPlacement.empty());
			m_playerShipPlacement.front()->onLeftClick(m_invalidPosition, m_currentTileSelected, m_battle);
			m_currentTileSelected = nullptr;
			break;
		}
		case BattlePhase::Movement:
		{
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
			std::pair<double, eDirection> inputInformation{ calculateDirection(m_leftMouseDownPosition,HAPI_Wrapper::getMouseLocation()) };
			if (inputInformation.first > 20)
			{
				/*
				This function call will be used if the mouse moved a significant enough distance during inputing a move command to assume it was on purpose, it sends not only the 
				destination tile but the direction of the aformentioned movement.  In order for this to work the pathfinding must be capable of taking an eDirection as part of the 
				function used by the battle to move the entity.
				*/
				//m_battle.moveEntityToPosition(*m_currentTileSelected->m_entityOnTile, *m_battle.getMap().getTile(m_leftMouseDownPosition), inputInformation.second);
			}
			/*
			This function call is to be used if the movement of the mouse during the move command is small enough to be considered unintended,
			in this case the ship should not rotate after reaching the destination.
			*/
			else m_battle.moveEntityToPosition(*m_currentTileSelected->m_entityOnTile, *m_battle.getMap().getTile(m_battle.getMap().getMouseClickCoord(m_leftMouseDownPosition)));
			m_currentTileSelected = nullptr;
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

	m_gui.OnMouseMove(mouseData);

	switch (m_battle.getCurrentPhase())
	{
	case BattlePhase::ShipPlacement:
	{
		assert(!m_playerShipPlacement.empty());
		m_currentTileSelected = m_playerShipPlacement.front()->getTileOnMouse(m_invalidPosition, m_currentTileSelected, m_battle.getMap());
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

void BattleUI::onMouseMoveMovementPhase()
{
	assert(m_battle.getCurrentPhase() == BattlePhase::Movement);

	//Current tile selected does not match the current player in play
	if (m_currentTileSelected && m_currentTileSelected->m_entityOnTile 
		&& m_currentTileSelected->m_entityOnTile->m_playerName != m_battle.getCurentPlayer())
	{
		return;
	}

	if (m_currentTileSelected && m_currentTileSelected->m_entityOnTile && !m_currentTileSelected->m_entityOnTile->m_battleProperties.isMovedToDestination())
	{
		const Tile* tile = m_battle.getMap().getTile(m_battle.getMap().getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
		if (!tile)
		{
			return;
		}

		if (m_currentTileSelected->m_tileCoordinate != tile->m_tileCoordinate)
		{
			if (tile->m_type != eTileType::eSea && tile->m_type != eTileType::eOcean)
			{
				m_invalidPosition.setPosition(m_battle.getMap().getTileScreenPos(tile->m_tileCoordinate), m_battle.getMap());
				m_invalidPosition.m_activate = true;
				m_currentTileSelected->m_entityOnTile->m_battleProperties.clearMovementPath();
			}
			else
			{
				m_currentTileSelected->m_entityOnTile->m_battleProperties.generateMovementGraph(m_battle.getMap(), *m_currentTileSelected, *tile);
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

	//Invalid Location - Collidable tile
	if (tileOnMouse->m_type != eTileType::eSea && tileOnMouse->m_type != eTileType::eOcean)
	{
		if (m_currentTileSelected && m_currentTileSelected->m_entityOnTile)
		{
			m_currentTileSelected->m_entityOnTile->m_battleProperties.clearMovementPath();
		}

		return;
	}

	//Clicking to where entity is moving to
	if (tileOnMouse->m_entityOnTile && tileOnMouse->m_entityOnTile->m_battleProperties.isMovedToDestination())
	{
		m_currentTileSelected = nullptr;
	}

	if (m_currentTileSelected)
	{
		//Cancel movement if clicked on same entity
		if (m_currentTileSelected->m_tileCoordinate == tileOnMouse->m_tileCoordinate)
		{
			m_currentTileSelected->m_entityOnTile->m_battleProperties.clearMovementPath();
			m_currentTileSelected = nullptr;
		}

		//Disallow movement to tile occupied by other player
		else if (tileOnMouse->m_entityOnTile && tileOnMouse->m_entityOnTile->m_playerName != m_battle.getCurentPlayer())
		{
			m_currentTileSelected->m_entityOnTile->m_battleProperties.clearMovementPath();
			m_currentTileSelected = nullptr;
		}

		//Instruct Entity to move to new location
		else if (m_currentTileSelected->m_entityOnTile && (m_currentTileSelected->m_tileCoordinate != tileOnMouse->m_tileCoordinate))
		{
			assert(m_currentTileSelected->m_entityOnTile->m_playerName == m_battle.getCurentPlayer());
			m_battle.moveEntityToPosition(*m_currentTileSelected->m_entityOnTile, *tileOnMouse);
			m_currentTileSelected = nullptr;
		}
	}
	else
	{
		//Do not select tile that contains wrong players entity
		if (tileOnMouse->m_entityOnTile)
		{
			if (tileOnMouse->m_entityOnTile->m_playerName != m_battle.getCurentPlayer())
			{
				m_currentTileSelected = nullptr;
			}
			else
			{
				m_currentTileSelected = tileOnMouse;
			}
		}
	}
}

void BattleUI::onRightClickMovementPhase()
{
	assert(m_battle.getCurrentPhase() == BattlePhase::Movement);
	//Cancel selected Entity
	if (m_currentTileSelected && m_currentTileSelected->m_entityOnTile)
	{
		m_currentTileSelected->m_entityOnTile->m_battleProperties.clearMovementPath();
		m_invalidPosition.m_activate = false;
	}

	m_currentTileSelected = nullptr;
}

void BattleUI::onLeftClickAttackPhase()
{
	assert(m_battle.getCurrentPhase() == BattlePhase::Attack);
	const Tile* tileOnMouse = m_battle.getMap().getTile(m_battle.getMap().getMouseClickCoord(HAPI_Wrapper::getMouseLocation()));
	if (!tileOnMouse)
	{
		return;
	}

	//Entity already selected
	//Fire weapon at position
	if (m_currentTileSelected && m_currentTileSelected->m_entityOnTile && !m_currentTileSelected->m_entityOnTile->m_battleProperties.isWeaponFired())
	{
		m_battle.fireEntityWeaponAtPosition(*m_currentTileSelected->m_entityOnTile, *tileOnMouse, m_targetArea.m_targetArea);

		//TODO: Might change this
		m_targetArea.clearTargetArea();
		m_currentTileSelected = nullptr;
		m_invalidPosition.m_activate = false;
		return;
	}

	//Entity Already Selected whilst showing where to fire
	//Change to different Entity before firing
	if (tileOnMouse->m_entityOnTile && tileOnMouse->m_entityOnTile->m_playerName == m_battle.getCurentPlayer()
		&& m_currentTileSelected && m_currentTileSelected && m_targetArea.m_targetArea.size() > 0)
	{
		m_targetArea.clearTargetArea();
		m_targetArea.generateTargetArea(m_battle.getMap(), *tileOnMouse);
		m_currentTileSelected = tileOnMouse;
		return;
	}

	//Click on same
	//Select new Entity to fire at something
	if (tileOnMouse->m_entityOnTile && tileOnMouse->m_entityOnTile->m_playerName != m_battle.getCurentPlayer())
	{
		return;
	}

	m_currentTileSelected = tileOnMouse;
	if (m_currentTileSelected->m_entityOnTile && !m_currentTileSelected->m_entityOnTile->m_battleProperties.isWeaponFired())
	{
		m_targetArea.generateTargetArea(m_battle.getMap(), *tileOnMouse);
	}
}

void BattleUI::onRightClickAttackPhase()
{
	m_currentTileSelected = nullptr;
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
	if (m_currentTileSelected && m_currentTileSelected->m_tileCoordinate != tileOnMouse->m_tileCoordinate)
	{
		auto cIter = std::find_if(m_targetArea.m_targetArea.cbegin(), m_targetArea.m_targetArea.cend(),
			[tileCoordinate](const auto& tile) { return tileCoordinate == tile->m_tileCoordinate; });
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
	m_targetArea = map.getTileCone(source.m_tileCoordinate, source.m_entityOnTile->m_entityProperties.m_range, 
		source.m_entityOnTile->m_battleProperties.getCurrentDirection());
	if (m_targetArea.empty())
	{
		return;
	}
	clearTargetArea();
	assert(!m_targetAreaSprites.empty());
	//using same convention as movement // from source should be able to get position
	for (int i = 0; i < m_targetArea.size(); i++)
	{
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

BattleUI::ShipPlacementPhase::ShipPlacementPhase(std::vector<EntityProperties*>& player, 
	std::pair<int, int> spawnPosition, int range, const Map& map, PlayerName playerName)
	: m_playerName(playerName),
	m_player(player),
	m_currentSelectedEntity(),
	m_spawnArea(),
	m_spawnSprites()
{
	//Might change this - for now its two containers but looks confusing
	m_spawnArea = map.getTileRadius(spawnPosition, range);
	m_spawnSprites.reserve(m_spawnArea.size());
	for (int i = 0; i < m_spawnArea.size(); ++i)
	{
		m_spawnSprites.push_back(std::make_unique<Sprite>(Textures::m_spawnHex));
	}

	for (int i = 0; i < m_spawnArea.size(); ++i)
	{
		auto screenPosition = map.getTileScreenPos(m_spawnArea[i]->m_tileCoordinate);
		m_spawnSprites[i]->GetTransformComp().SetPosition({
		(float)screenPosition.first + DRAW_OFFSET_X * map.getDrawScale() ,
		(float)screenPosition.second + DRAW_OFFSET_Y * map.getDrawScale() });
	}

	m_currentSelectedEntity.m_currentSelectedEntity = m_player.back();
}

bool BattleUI::ShipPlacementPhase::isCompleted() const
{
	return m_player.empty();
}

void BattleUI::ShipPlacementPhase::render(const InvalidPosition& invalidPosition, const Map& map) const
{
	for (auto& i : m_spawnArea)
	{
		i->m_sprite->Render(SCREEN_SURFACE);
	}

	if (m_currentSelectedEntity.m_currentSelectedEntity && !invalidPosition.m_activate)
	{
		const std::pair<int, int> tileTransform = map.getTileScreenPos(m_currentSelectedEntity.m_position);

		m_currentSelectedEntity.m_currentSelectedEntity->m_sprite->GetTransformComp().SetPosition({
		static_cast<float>(tileTransform.first + DRAW_ENTITY_OFFSET_X * map.getDrawScale()),
		static_cast<float>(tileTransform.second + DRAW_ENTITY_OFFSET_Y * map.getDrawScale()) });

		m_currentSelectedEntity.m_currentSelectedEntity->m_sprite->Render(SCREEN_SURFACE);
	}
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

void BattleUI::ShipPlacementPhase::onLeftClick(const InvalidPosition& invalidPosition, const Tile* currentTileSelected, Battle& battle)
{
	if (!invalidPosition.m_activate && currentTileSelected && !currentTileSelected->m_entityOnTile)
	{
		battle.insertEntity(currentTileSelected->m_tileCoordinate, *m_currentSelectedEntity.m_currentSelectedEntity, m_playerName);

		//Change ordering around to pop front with different container
		m_player.pop_back();
		if (m_player.empty())
		{
			battle.nextTurn();
			return;
		}

		m_currentSelectedEntity.m_currentSelectedEntity = m_player.back();
	}
}
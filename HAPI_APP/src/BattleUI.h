#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include "PlayerName.h"
#include <vector>

struct EntityProperties;
struct Tile;
class Battle;
class Map;
class BattleUI : public IHapiSpritesInputListener
{
	struct TargetArea
	{
		struct HighlightNode
		{
			HighlightNode();
			std::unique_ptr<Sprite> sprite;
			bool activate;
		};

		TargetArea();
		void render() const;
		void generateTargetArea(const Map& map, const Tile& source);
		void clearTargetArea();

		std::vector<HighlightNode> m_targetAreaSprites;
		std::vector<const Tile*> m_targetArea;
	};

	struct InvalidPosition
	{
		InvalidPosition();

		void render() const;
		void setPosition(std::pair<int, int> screenPosition, float mapDrawScale);

		std::unique_ptr<Sprite> m_sprite;
		bool m_activate;
	};

	class PlayerShipPlacement
	{
	public:
		PlayerShipPlacement(std::vector<EntityProperties*>& player,
			std::pair<int, int> spawnPosition, int range, Battle& battle, PlayerName playerName);

		bool isCompleted() const;
		void render(const InvalidPosition& invalidPosition) const;

		void onMouseMove(InvalidPosition& invalidPosition, const Tile* currentTileSelected);
		void onLeftClick(const InvalidPosition& invalidPosition, const Tile* currectTileSelected);

	private:
		Battle& m_battle;
		PlayerName m_playerName;
		std::vector<EntityProperties*>& m_player;
		EntityProperties* m_currentSelectedEntity;
		std::vector<const Tile*> m_spawnArea;
		std::vector<std::unique_ptr<Sprite>> m_spawnSprites;
	};

public:
	BattleUI(Battle& battle, std::vector<EntityProperties*>& player1, std::vector<EntityProperties*>& player2);

	void render() const;

	void newPhase();
	void newTurn(PlayerName playersTurn);

	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final {}
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;

private:
	Battle& m_battle;
	const Tile* m_currentTileSelected;
	InvalidPosition m_invalidPosition;
	std::vector<std::unique_ptr<PlayerShipPlacement>> m_playerShipPlacement;
	
	////ShipPlacement Phase
	//std::deque<EntityProperties*>* m_selectedEntities;
	//std::deque<EntityProperties*>* m_player1;
	//std::deque<EntityProperties*>* m_player2;
	//EntityProperties* m_currentSelectedEntity;
	//std::vector<const Tile*> m_spawnTiles;
	//std::vector<std::unique_ptr<Sprite>> m_spawnSprites;

	//Movement Phase
	void onMouseMoveMovementPhase();
	void onLeftClickMovementPhase();
	void onRightClickMovementPhase();


	//Attack Phase
	void onLeftClickAttackPhase();
	TargetArea m_targetArea;
};
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

	class ShipPlacementPhase
	{
	public:
		ShipPlacementPhase(std::vector<EntityProperties*>& player,
			std::pair<int, int> spawnPosition, int range, const Map& map, PlayerName playerName);

		bool isCompleted() const;
		void render(const InvalidPosition& invalidPosition) const;

		const Tile* getTileOnMouse(InvalidPosition& invalidPosition, const Tile* currentTileSelected, const Map& map);
		void onLeftClick(const InvalidPosition& invalidPosition, const Tile* currectTileSelected, Battle& battle);

	private:
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
	std::vector<std::unique_ptr<ShipPlacementPhase>> m_playerShipPlacement;
	
	//Movement Phase
	void onMouseMoveMovementPhase();
	void onLeftClickMovementPhase();
	void onRightClickMovementPhase();


	//Attack Phase
	void onLeftClickAttackPhase();
	void onRightClickAttackPhase();
	void onMouseMoveAttackPhase();
	TargetArea m_targetArea;
};
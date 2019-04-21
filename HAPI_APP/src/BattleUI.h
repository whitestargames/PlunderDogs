#pragma once

#include "BattleGUI.h"
#include "entity.h"
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
			std::pair<int, int> position;
		};

		TargetArea();
		void render(const Map& map) const;
		void generateTargetArea(const Map& map, const Tile& source);
		void clearTargetArea();

		std::vector<HighlightNode> m_targetAreaSprites;
		std::vector<const Tile*> m_targetArea;
	};

	struct InvalidPosition
	{
		InvalidPosition();

		void render(const Map& map) const;
		void setPosition(std::pair<int, int> screenPosition, const Map& map);

		std::unique_ptr<Sprite> m_sprite;
		bool m_activate;
		std::pair<int, int> m_position;
	};

	class ShipPlacementPhase
	{
		struct CurrentSelectedEntity
		{
			CurrentSelectedEntity()
				: m_currentSelectedEntity(nullptr),
				m_position()
			{}

			EntityProperties* m_currentSelectedEntity;
			std::pair<int, int> m_position;
		};
	public:
		ShipPlacementPhase(std::vector<EntityProperties*> player,
			std::pair<int, int> spawnPosition, int range, const Map& map, FactionName factionName);

		bool isCompleted() const;
		void render(const InvalidPosition& invalidPosition, const Map& map) const;

		const Tile* getTileOnMouse(InvalidPosition& invalidPosition, const Tile* currentTileSelected, const Map& map);
		void onLeftClick(const InvalidPosition& invalidPosition, eDirection startingDirection, const Tile* currectTileSelected, Battle& battle);

	private:
		FactionName m_factionName;
		std::vector<EntityProperties*> m_player;
		CurrentSelectedEntity m_currentSelectedEntity;
		std::vector<const Tile*> m_spawnArea;
		std::vector<std::unique_ptr<Sprite>> m_spawnSprites;
	};

	struct CurrentSelectedTile
	{
		CurrentSelectedTile();

		void render(const Map& map) const;

		std::unique_ptr<Sprite> m_sprite;
		const Tile* m_tile;
		std::pair<int, int> m_position;
	};

public:
	BattleUI(Battle& battles);

	std::pair<int, int> getCameraPositionOffset() const;

	void renderUI() const;
	void renderGUI() const;
	void update();
	void newPhase();
	void newTurn(FactionName playersTurn);
	void startShipPlacement(std::vector<std::pair<FactionName, std::vector<EntityProperties*>>>& players);

	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final {}
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;

private:
	Battle& m_battle;
	CurrentSelectedTile m_selectedTile;
	//This stores the position at which the mouse event "eLeftMouseButtonDown" last occured while giving an entity a move command, 
	//it's used to calculate what direction the mouse moved during that input.
	std::pair<int, int> m_leftMouseDownPosition;
	//This is used to determine if an entity is currently being given a move command, it gets set to true in the "handleOnLeftClickMovementPhase()" and false after "eLeftMouseButtonUp" is detected.
	bool m_isMovingEntity;
	//Used to store the tile selected for movement when the lmb is depressed, so that it can be used for moveEntity input on mouse up
	const Tile* m_mouseDownTile;
	BattleGUI m_gui;
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
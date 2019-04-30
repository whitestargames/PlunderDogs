#pragma once

#include "BattleGUI.h"
#include "entity.h"
#include <vector>
#include <deque>

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

	struct ParticleSystem
	{
		std::pair<float, float> m_position;
		Timer m_lifeSpan;
		std::unique_ptr<HAPISPACE::Sprite> m_particle;
		int m_frameNum = 0;
		bool m_isEmitting;

		ParticleSystem(float lifespan, std::shared_ptr<HAPISPACE::SpriteSheet> texture);
		void setPosition(std::pair<int, int> position);
		void run(float deltaTime, const Map& map);
		void render() const;
		void orient(eDirection direction);
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

		std::pair<int, int> getSpawnPosition() const;

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
		std::pair<int, int> m_spawnPosition;
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
	~BattleUI();

	std::pair<int, int> getCameraPositionOffset() const;

	void renderUI() const;
	void renderParticles() const;
	void renderGUI() const;
	void loadGUI(std::pair<int, int> mapDimensions);

	void update(float deltaTime);

	void FactionUpdateGUI(FactionName faction);//tempName

	//void newPhase();
	//void newTurn(FactionName playersTurn);

	void startShipPlacement(const std::vector<std::pair<FactionName, std::vector<EntityProperties*>>>& players, Map& map);

	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final {}
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;
	void setCurrentFaction(FactionName faction);
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
	std::deque<std::unique_ptr<ShipPlacementPhase>> m_playerShipPlacement;

	//Movement Phase
	void onMouseMoveMovementPhase();
	void onLeftClickMovementPhase();
	void onRightClickMovementPhase();

	//Attack Phase
	void onLeftClickAttackPhase();
	void onRightClickAttackPhase();
	void onMouseMoveAttackPhase();
	TargetArea m_targetArea;

	void onResetBattle();
	void onNewTurn();

	ParticleSystem m_explosion;
	ParticleSystem m_fire;
};
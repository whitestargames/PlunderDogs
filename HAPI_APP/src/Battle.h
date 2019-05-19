#pragma once

#include "Global.h"
#include "Map.h"
#include "BattleUI.h"
#include "entity.h"

enum class BattlePhase
{
	Deployment = 0,
	Movement,
	Attack
};

class DeploymentPhase
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
	DeploymentPhase(FactionName factionName);

	std::pair<int, int> getSpawnPosition() const;

	bool isCompleted() const;
	void render(const InvalidPosition& invalidPosition, const Map& map) const;
	void onReset();

	const Tile* getTileOnMouse(InvalidPosition& invalidPosition, const Tile* currentTileSelected, const Map& map);
	void onLeftClick(InvalidPosition& invalidPosition, eDirection startingDirection, const Tile* currectTileSelected, Battle& battle);

private:
	FactionName m_factionName;
	std::vector<EntityProperties*> m_player;
	CurrentSelectedEntity m_currentSelectedEntity;
	std::vector<const Tile*> m_spawnArea;
	std::vector<std::unique_ptr<Sprite>> m_spawnSprites;
	std::pair<int, int> m_spawnPosition;
};

class InvalidPosition;
struct BattlePlayer
{
	BattlePlayer(FactionName name, std::pair<int, int> spawnPosition, ePlayerType playerType, const Map& map);

	std::vector<std::shared_ptr<BattleEntity>> m_entities; 
	const FactionName m_factionName;
	const ePlayerType m_playerType;
	const std::pair<int, int> m_spawnPosition;
	bool m_eliminated;
	//Deployment
	bool m_deployed;
};

class Player;
class Battle
{
	struct LightIntensity
	{
		LightIntensity();

		void update(float deltaTime);

		Timer m_timer;
		eLightIntensity m_lightIntensity;
	};

	struct Particle
	{
		std::pair<float, float> m_position;
		Timer m_lifeSpan;
		std::unique_ptr<HAPISPACE::Sprite> m_particle;
		int m_frameNum = 0;
		bool m_isEmitting;
		const float m_scale;

		Particle(float lifespan, std::shared_ptr<HAPISPACE::SpriteSheet> texture, float scale);
		void setPosition(std::pair<int, int> position);
		void run(float deltaTime, const Map& map);
		void render() const;
		void orient(eDirection direction);
	};

	class FactionWinHandler
	{
	public:
		FactionWinHandler();
		~FactionWinHandler();
	
		bool isGameOver() const;

		void update(float deltaTime);

		void onYellowShipDestroyed(std::vector<BattlePlayer>& players);
		void onBlueShipDestroyed(std::vector<BattlePlayer>& players);
		void onGreenShipDestroyed(std::vector<BattlePlayer>& players);
		void onRedShipDestroyed(std::vector<BattlePlayer>& players);

	private:
		int m_yellowShipsDestroyed;
		int m_blueShipsDestroyed;
		int m_greenShipsDestroyed;
		int m_redShipsDestroyed;
		void onReset();

		void checkGameStatus(const std::vector<BattlePlayer>& players);
		Timer m_winTimer;
		FactionName m_winningFaction;
		bool m_gameOver;
	};

public:
	Battle();
	~Battle();

	const Map& getMap() const;
	BattlePhase getCurrentPhase() const;

	FactionName getCurrentFaction() const;
	const BattlePlayer& getPlayer(FactionName name) const;
	const std::vector<BattlePlayer>& getAllPlayers() const;
	bool isAIPlaying() const;

	void start(const std::string& newMapName, const std::vector<Player>& newPlayers);
	void render() const;
	void update(float deltaTime);
	void moveEntityToPosition(BattleEntity& entity, const Tile& destination);
	void moveEntityToPosition(BattleEntity& entity, const Tile& destination, eDirection endDirection);

	bool fireEntityWeaponAtPosition(const Tile& tileOnPlayer, const Tile& tileOnAttackPosition, const std::vector<const Tile*>& targetArea);
	void insertEntity(std::pair<int, int> startingPosition, eDirection startingDirection, const EntityProperties& entityProperties, FactionName factionName);
	void nextTurn();

	std::vector<FactionName> getAllFactions() const;
	//const BattlePlayer& getPlayer(FactionName faction) const;
	//std::vector<std::shared_ptr<BattleEntity>>& getFactionShips(FactionName faction);
	//const std::vector<std::shared_ptr<BattleEntity>>& getFactionShips(FactionName faction) const;
	void playFireAnimation(BattleEntity& entity, std::pair<int, int> position);
	void playExplosionAnimation(BattleEntity& entity);
private:
	std::vector<BattlePlayer> m_players;
	int m_currentPlayerTurn;
	Map m_map;
	BattlePhase m_currentPhase;
	BattleUI m_battleUI;
	FactionWinHandler m_factionWinHandler;
	std::vector<Particle> m_explosionParticles;
	std::vector<Particle> m_fireParticles;
	Timer m_timeUntilAITurn;
	Timer m_timeBetweenAIUnits;
	
	bool m_AITurn;
	LightIntensity m_lightIntensity;
	
	void updateDeploymentPhase();
	void updateMovementPhase(float deltaTime);
	void updateAttackPhase();

	bool allEntitiesAttacked(std::vector<std::shared_ptr<BattleEntity>>& playerEntities) const;
	BattlePlayer& getPlayer(FactionName factionName);


	void incrementPlayerTurn();
	void updateWindDirection();

	void handleAIMovementPhaseTimer(float deltaTime);
	void handleAIAttackPhaseTimer(float deltaTime);
	//void resetAITimers();

	void onResetBattle();
	void onYellowShipDestroyed();
	void onBlueShipDestroyed();
	void onGreenShipDestroyed();
	void onRedShipDestroyed();
	void onEndMovementPhaseEarly();
	void onEndAttackPhaseEarly();
	std::vector<DeploymentPhase> m_playersToDeploy;
};
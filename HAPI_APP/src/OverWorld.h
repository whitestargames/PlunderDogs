#pragma once

#include <string>
#include "Global.h"
#include "Battle.h"
#include "FactionName.h"
#include "OverWorldGUI.h"
#include <deque>

using namespace HAPI_UI_SPACE;
using namespace HAPISPACE;

struct Player
{
	Player(FactionName name);

	std::vector<EntityProperties> m_entities;
	std::vector<EntityProperties*> m_selectedEntities;
	const FactionName m_factionName;
};

class OverWorld : public IHapiSpritesInputListener
{
public:
	OverWorld();

	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;
	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final {}
	bool battleState = false;

	void render();
	void update(float deltaTime);
	void startBattle();

private:
	const int m_totalPlayers;
	int m_currentPlayer;
	bool m_selectNextPlayer;
	std::vector<Player> m_players;
	//Player m_player1;
	//Player m_player2;
	OverWorldGUI m_GUI;
	std::unique_ptr<Battle> m_battle;

	//Temporary - will change functionality in next few days
	//14/04/19
	bool m_startBattle;
	bool m_selectedNextPlayer;

	FactionName m_currentFactionSelected;
};
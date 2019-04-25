#pragma once

#include "Global.h"
#include "Battle.h"
#include "OverWorldGUI.h"

using namespace HAPI_UI_SPACE;
using namespace HAPISPACE;

class OverWorld : public IHapiSpritesInputListener
{
public:
	OverWorld();
	~OverWorld();

	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;
	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final {}

	void render();
	void update(float deltaTime);
	void startBattle();
	
private:
	
	int m_currentPlayer;
	bool m_selectNextPlayer;
	std::vector<Player> m_players;
	OverWorldGUI m_GUI;
	Battle m_battle;
	bool m_startBattle;

	void onReset();
};
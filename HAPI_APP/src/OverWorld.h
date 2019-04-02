#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include <string>
#include "Battle.h"

using namespace HAPI_UI_SPACE;
using namespace HAPISPACE;

enum class OverWorldWindow
{
	Battle = 0,
	PreBattle,
	LevelSelection
};

class OverWorld : public IHapiSpritesInputListener
{
public:
	OverWorld();

	void render();
	void update(float deltaTime);
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;
	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final {}
	
	static OverWorldWindow CURRENT_WINDOW;

private:
	std::unique_ptr<Sprite> m_battleMapBackground;
	std::unique_ptr<Sprite> m_enemyTerritoryHexSheet;
	std::unique_ptr<Sprite> m_prebattleUIBackground;
	std::unique_ptr<Sprite> m_playButton;
	std::unique_ptr<Sprite> m_backButton;
	Battle m_battle;
};
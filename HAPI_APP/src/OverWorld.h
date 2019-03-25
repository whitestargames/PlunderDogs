#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include <string>
#include "Battle.h"

using namespace HAPI_UI_SPACE;
using namespace HAPISPACE;

class OverWorld : public IHapiSpritesInputListener
{
	enum class OverWorldWindow
	{
		Battle = 0,
		PreBattle,              
		LevelSelection
	};
public:
	OverWorld();

	void render();
	void update() {}
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;
	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final {}

private:
	std::unique_ptr<Sprite> m_battleMapBackground;
	std::unique_ptr<Sprite> m_enemyTerritoryHexSheet;
	std::unique_ptr<Sprite> m_prebattleUIBackground;
	std::unique_ptr<Sprite> m_playButton;
	std::unique_ptr<Sprite> m_backButton;
	std::vector<Entity> m_entityVector;
	std::vector<Entity> m_selectedEntities;
	std::shared_ptr<Entity> m_currentlySelected {nullptr};
	HAPISPACE::SliderLayout sliderLayout;
	OverWorldWindow m_currentWindow;
	Battle m_battle;
};
#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include <string>

using namespace HAPI_UI_SPACE;
using namespace HAPISPACE;

class OverWorld : public IHapiSpritesInputListener
{
	enum class OverWorldWindow
	{
		Battle = 0,
		PreBattle,              
		LevelSelection,
		Upgrade
	};
public:
	OverWorld();

	void render();
	void update() {}
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;
	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final {}
	bool battleState = false;

private:
	std::unique_ptr<Sprite> m_battleMapBackground;
	std::unique_ptr<Sprite> m_enemyTerritoryHexSheet;
	std::unique_ptr<Sprite> m_prebattleUIBackground;
	std::unique_ptr<Sprite> m_playButton;
	std::unique_ptr<Sprite> m_backButton;
	std::unique_ptr<Sprite> m_upgradesButton;
	OverWorldWindow m_currentWindow;
};
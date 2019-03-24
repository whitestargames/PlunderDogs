#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include <string>

using namespace HAPI_UI_SPACE;
using namespace HAPISPACE;

class OverWorld : public IHapiSpritesInputListener
{
public:
	OverWorld();

	void Update();
	void Run();
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;
	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final {}

private:
	std::unique_ptr<Sprite> m_battleMapBackground;
	std::unique_ptr<Sprite> m_enemyTerritoryHexSheet;
	std::unique_ptr<Sprite> m_prebattleUIBackground;
	std::unique_ptr<Sprite> m_playButton;
	std::unique_ptr<Sprite> m_backButton;
	bool m_testBattleMapWindow;
	bool m_testPrebattleWindow;
	bool m_battleSystem;
};
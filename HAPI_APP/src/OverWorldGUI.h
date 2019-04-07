#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include "GUIBase.h"
#include <memory>

class OverWorldGUI : public GUIBase
{
public:
	OverWorldGUI();

	void render() override final;

	void onMouseMove(const HAPI_TMouseData& mouseData) override final;
	void onLeftClick(const HAPI_TMouseData& mouseData) override final;
	void onRightClick() override final {}

private:
	std::unique_ptr<Sprite> m_battleMapBackground;
	std::unique_ptr<Sprite> m_enemyTerritoryHexSheet;
	std::unique_ptr<Sprite> m_prebattleUIBackground;
	std::unique_ptr<Sprite> m_playButton;
	std::unique_ptr<Sprite> m_backButton;

};
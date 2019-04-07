#include "OverWorldGUI.h"
#include "Textures.h"
#include "OverWorld.h"
#include "Utilities/Utilities.h"

constexpr int WINDOW_OBJECTWIDTH = 75;
constexpr int WINDOW_OBJECTHEIGHT = 150;
constexpr int WINDOW_WIDTH = 830;
constexpr int WINDOW_HEIGHT = 200;

OverWorldGUI::OverWorldGUI()
	: m_battleMapBackground(std::make_unique<Sprite>(Textures::m_battleMapBackground)),
	m_enemyTerritoryHexSheet(std::make_unique<Sprite>(Textures::m_enemyTerritoryHexSheet)),
	m_prebattleUIBackground(std::make_unique<Sprite>(Textures::m_prebattleUIBackground)),
	m_playButton(std::make_unique<Sprite>(Textures::m_preBattleUIPlayButton)),
	m_backButton(std::make_unique<Sprite>(Textures::m_preBattleUIBackButton)),
	fleetWindowSkinName(UI.LoadSkin(Utilities::getDataDirectory() + "fleetWindowSkin.xml")),
	fleetWindowSliderSkinName(UI.LoadSkin(Utilities::getDataDirectory() + "fleetWindowSliderSkin.xml"))
{
	HAPI_Wrapper::setPosition(m_enemyTerritoryHexSheet, { 100, 600 });
	HAPI_Wrapper::setPosition(m_playButton, { 1150, 722 });
	HAPI_Wrapper::setPosition(m_backButton, { 185, 747 });
	//adding the windows and sliders, also populates the fleet window with all current entities
	UI.AddWindow(FLEET_WINDOW, HAPISPACE::RectangleI(220, 1050, 510, 710), fleetWindowSkinName);
	/*for (int i = 0; i < Overworld.m_entityVector.size(); i++)
	{
		UI.GetWindow(FLEET_WINDOW)->AddCanvas(ENTITY + std::to_string(i), calculateObjectWindowPosition(i), m_entityVector[i].m_sprite);
	}*/
	UI.GetWindow(FLEET_WINDOW)->AddSlider(FLEET_SLIDER, HAPISPACE::RectangleI(0, 830, 160, 210), sliderLayout);
	UI.AddWindow(BATTLE_FLEET_WINDOW, HAPISPACE::RectangleI(220, 1050, 220, 420), fleetWindowSkinName);
	UI.GetWindow(BATTLE_FLEET_WINDOW)->AddSlider(BATTLE_FLEET_SLIDER, HAPISPACE::RectangleI(0, 830, 160, 210), sliderLayout);
}

void OverWorldGUI::render()
{
	switch (OverWorld::CURRENT_WINDOW)
	{
	case OverWorldWindow::PreBattle:
	{
		HAPI_Wrapper::render(m_prebattleUIBackground);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 270), HAPISPACE::Colour255::BLACK, "45/55", 50);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 355), HAPISPACE::Colour255::BLACK, "3", 50);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 445), HAPISPACE::Colour255::BLACK, "4", 50);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 535), HAPISPACE::Colour255::BLACK, "5", 50);
		HAPI_Wrapper::render(m_playButton);
		HAPI_Wrapper::render(m_backButton);
		break;
	}
	case OverWorldWindow::LevelSelection:
	{
		HAPI_Wrapper::render(m_battleMapBackground);
		HAPI_Wrapper::render(m_enemyTerritoryHexSheet);
		break;
	}
	}
}
void OverWorldGUI::onMouseMove(const HAPI_TMouseData& mouseData)
{
	switch (OverWorld::CURRENT_WINDOW)
	{
	case OverWorldWindow::LevelSelection:
	{
		if (HAPI_Wrapper::isTranslated(m_enemyTerritoryHexSheet, mouseData, 0))
		{
			m_enemyTerritoryHexSheet->SetFrameNumber(1);//changes the buttons sprite to hover sprite
		}
		else if (m_enemyTerritoryHexSheet->GetFrameNumber() != 0)//if mouse is not over the button and the button has the hover sprite
		{
			m_enemyTerritoryHexSheet->SetFrameNumber(0);// sets it to the default sprite
		}
		break;
	}
	case OverWorldWindow::PreBattle:
	{
		//Play Button
		if (HAPI_Wrapper::isTranslated(m_playButton, mouseData, 0))
		{
			m_playButton->SetFrameNumber(1);
		}
		else if (m_playButton->GetFrameNumber() != 0)
		{
			m_playButton->SetFrameNumber(0);
		}
		//Back Button
		if (HAPI_Wrapper::isTranslated(m_backButton, mouseData, 0))
		{
			m_backButton->SetFrameNumber(1);
		}
		else
		{
			m_backButton->SetFrameNumber(0);
		}
		break;
	}
	}
}

void OverWorldGUI::onLeftClick(const HAPI_TMouseData& mouseData)
{
	switch (OverWorld::CURRENT_WINDOW)
	{
	case OverWorldWindow::LevelSelection:
	{
		if (HAPI_Wrapper::isTranslated(m_enemyTerritoryHexSheet, mouseData, 0))
		{
			OverWorld::CURRENT_WINDOW = OverWorldWindow::PreBattle;
		}
		break;
	}
	case OverWorldWindow::PreBattle:
	{
		if (HAPI_Wrapper::isTranslated(m_playButton, mouseData, 0))
		{
			OverWorld::CURRENT_WINDOW = OverWorldWindow::Battle;
		}
		else if (HAPI_Wrapper::isTranslated(m_backButton, mouseData, 0))
		{
			OverWorld::CURRENT_WINDOW = OverWorldWindow::LevelSelection;
		}
		break;
	}
	}
}
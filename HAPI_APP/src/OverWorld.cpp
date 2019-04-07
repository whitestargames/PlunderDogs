#include  "Overworld.h"
#include "Battle.h"
#include "Utilities/Utilities.h"
#include "HAPIWrapper.h"
#include "Textures.h"

OverWorldWindow OverWorld::CURRENT_WINDOW = OverWorldWindow::LevelSelection;

OverWorld::OverWorld()
	: m_battleMapBackground(std::make_unique<Sprite>(Textures::m_battleMapBackground)),
	m_enemyTerritoryHexSheet(std::make_unique<Sprite>(Textures::m_enemyTerritoryHexSheet)),
	m_prebattleUIBackground(std::make_unique<Sprite>(Textures::m_prebattleUIBackground)),
	m_playButton(std::make_unique<Sprite>(Textures::m_preBattleUIPlayButton)), 
	m_backButton(std::make_unique<Sprite>(Textures::m_preBattleUIBackButton))
{
	HAPI_Wrapper::setPosition(m_enemyTerritoryHexSheet, { 100, 600 });
	HAPI_Wrapper::setPosition(m_playButton, { 1150, 722 });
	HAPI_Wrapper::setPosition(m_backButton, { 185, 747 });
}

void OverWorld::render()
{
	switch (CURRENT_WINDOW)
	{
	case OverWorldWindow::PreBattle :
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
	case OverWorldWindow::Battle :
	{
		m_battle.render();
		break;
	}
	case OverWorldWindow::LevelSelection :
	{
		HAPI_Wrapper::render(m_battleMapBackground);
		HAPI_Wrapper::render(m_enemyTerritoryHexSheet);
		break;
	}
	}
}

void OverWorld::update(float deltaTime)
{
	switch (CURRENT_WINDOW)
	{
	case OverWorldWindow::Battle :
	{
		m_battle.update(deltaTime);
		break;
	}
	}
}

void OverWorld::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData)
{
	if (mouseEvent == EMouseEvent::eLeftButtonDown)
	{
		switch (CURRENT_WINDOW)
		{
		case OverWorldWindow::LevelSelection:
		{
			if (HAPI_Wrapper::isTranslated(m_enemyTerritoryHexSheet, mouseData, 0))
			{
				CURRENT_WINDOW = OverWorldWindow::PreBattle;
			}
			break;
		}
		case OverWorldWindow::PreBattle:
		{
			if (HAPI_Wrapper::isTranslated(m_playButton, mouseData, 0))
			{
				CURRENT_WINDOW = OverWorldWindow::Battle;
			}
			else if (HAPI_Wrapper::isTranslated(m_backButton, mouseData, 0))
			{
				CURRENT_WINDOW = OverWorldWindow::LevelSelection;
			}
			break;
		}
		}
	}
}

void OverWorld::OnMouseMove(const HAPI_TMouseData& mouseData)
{
	switch (CURRENT_WINDOW)
	{
	case OverWorldWindow::LevelSelection :
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
	case OverWorldWindow::PreBattle :
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
#include  "Overworld.h"
#include "Battle.h"
#include "Utilities/Utilities.h"
#include "HAPIWrapper.h"

OverWorldWindow OverWorld::CURRENT_WINDOW = OverWorldWindow::LevelSelection;

OverWorld::OverWorld()
	: m_battleMapBackground(HAPI_Wrapper::makeSprite("TempBattleMapBackground.png")),
	m_enemyTerritoryHexSheet(HAPI_Wrapper::makeSprite("EnemyTerritoryHexSheet.png", 2)),
	m_prebattleUIBackground(HAPI_Wrapper::makeSprite("PrebattleUI.png")),
	m_playButton(HAPI_Wrapper::makeSprite("PrebattleUIPlayButton.png", 2)),
	m_backButton(HAPI_Wrapper::makeSprite("PrebattleUIBackButton.png", 2))
{
	m_enemyTerritoryHexSheet->GetTransformComp().SetPosition({ 100, 600 });
	m_playButton->GetTransformComp().SetPosition({ 1150, 722 });
	m_backButton->GetTransformComp().SetPosition({ 185, 747 });
}

void OverWorld::render()
{
	SCREEN_SURFACE->Clear();

	switch (CURRENT_WINDOW)
	{
	case OverWorldWindow::PreBattle :
	{
		m_prebattleUIBackground->Render(SCREEN_SURFACE);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 270), HAPISPACE::Colour255::BLACK, "45/55", 50);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 355), HAPISPACE::Colour255::BLACK, "3", 50);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 445), HAPISPACE::Colour255::BLACK, "4", 50);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 535), HAPISPACE::Colour255::BLACK, "5", 50);
		m_playButton->Render(SCREEN_SURFACE);
		m_backButton->Render(SCREEN_SURFACE);
		break;
	}
	case OverWorldWindow::Battle :
	{
		m_battle.render();
		break;
	}
	case OverWorldWindow::LevelSelection :
	{
		m_battleMapBackground->Render(SCREEN_SURFACE);
		m_enemyTerritoryHexSheet->Render(SCREEN_SURFACE);
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
			if (HAPI_Wrapper::isSpriteTranslated(m_enemyTerritoryHexSheet, mouseData, 0))
			{
				CURRENT_WINDOW = OverWorldWindow::PreBattle;
			}
			break;
		}
		case OverWorldWindow::PreBattle:
		{
			if (HAPI_Wrapper::isSpriteTranslated(m_playButton, mouseData, 0))
			{
				CURRENT_WINDOW = OverWorldWindow::Battle;
			}
			else if (HAPI_Wrapper::isSpriteTranslated(m_backButton, mouseData, 0))
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
		if (HAPI_Wrapper::isSpriteTranslated(m_enemyTerritoryHexSheet, mouseData, 0))
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
		if (HAPI_Wrapper::isSpriteTranslated(m_playButton, mouseData, 0))
		{
			m_playButton->SetFrameNumber(1);
		}
		else if (m_playButton->GetFrameNumber() != 0)
		{
			m_playButton->SetFrameNumber(0);
		}
		//Back Button
		if (HAPI_Wrapper::isSpriteTranslated(m_backButton, mouseData, 0))
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
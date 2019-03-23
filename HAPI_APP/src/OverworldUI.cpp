#include  "OverworldUI.h"
#include "BattleSystem.h"
#include "Utilities/Utilities.h"

OverworldUIWIndowTest::OverworldUIWIndowTest()
	: m_battleMapBackground(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "TempBattleMapBackground.png")),
	m_enemyTerritoryHexSheet(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "EnemyTerritoryHexSheet.png", 2)),
	m_prebattleUIBackground(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "PrebattleUI.png")),
	m_playButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "PrebattleUIPlayButton.png", 2)),
	m_backButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "PrebattleUIBackButton.png", 2)),
	m_testBattleMapWindow(false),
	m_testPrebattleWindow(false)
{
	m_testBattleMapWindow = true;

	m_enemyTerritoryHexSheet->GetTransformComp().SetPosition({ 100, 600 });
	m_playButton->GetTransformComp().SetPosition({ 1150, 722 });
	m_backButton->GetTransformComp().SetPosition({ 185, 747 });
}

void OverworldUIWIndowTest::Update()
{
	SCREEN_SURFACE->Clear();

	m_battleMapBackground->Render(SCREEN_SURFACE);
	m_enemyTerritoryHexSheet->Render(SCREEN_SURFACE);

	if (m_testPrebattleWindow)
	{
		m_prebattleUIBackground->Render(SCREEN_SURFACE);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 270), HAPISPACE::Colour255::BLACK, "45/55", 50);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 355), HAPISPACE::Colour255::BLACK, "3", 50);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 445), HAPISPACE::Colour255::BLACK, "4", 50);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 535), HAPISPACE::Colour255::BLACK, "5", 50);
		m_playButton->Render(SCREEN_SURFACE);
		m_backButton->Render(SCREEN_SURFACE);
	}
}

void OverworldUIWIndowTest::Run()
{
	while (HAPI_Sprites.Update())
	{
		Update();
	}
}

void OverworldUIWIndowTest::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData)
{
	if (mouseEvent == EMouseEvent::eLeftButtonDown)
	{
		if (m_testBattleMapWindow && !m_testPrebattleWindow)
		{
			if (m_enemyTerritoryHexSheet->GetSpritesheet()->GetFrameRect(0).Translated(m_enemyTerritoryHexSheet->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				m_testPrebattleWindow = true;
				UI.OpenWindow("testWindow");
			}
		}
		else if (m_testPrebattleWindow)
		{
			if (m_playButton->GetSpritesheet()->GetFrameRect(0).Translated(m_playButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				BattleSystem world;
				UI.CloseWindow("testWindow");
				world.run();
			}
			else if (m_backButton->GetSpritesheet()->GetFrameRect(0).Translated(m_backButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				m_testPrebattleWindow = false;
				UI.CloseWindow("testWindow");
			}
		}
	}
}

void OverworldUIWIndowTest::OnMouseMove(const HAPI_TMouseData& mouseData)
{
	if (m_testBattleMapWindow && !m_testPrebattleWindow)//the battle map is active in the background of the prebattle so we need both checks
	{
		if (m_enemyTerritoryHexSheet->GetSpritesheet()->GetFrameRect(0).Translated(m_enemyTerritoryHexSheet->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))//checks if mouse is over button
		{
			m_enemyTerritoryHexSheet->SetFrameNumber(1);//changes the buttons sprite to hover sprite
		}
		else if (m_enemyTerritoryHexSheet->GetFrameNumber() != 0)//if mouse is not over the button and the button has the hover sprite
		{
			m_enemyTerritoryHexSheet->SetFrameNumber(0);// sets it to the default sprite
		}
	}
	else if (m_testPrebattleWindow)
	{
		if (m_playButton->GetSpritesheet()->GetFrameRect(0).Translated(m_playButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
		{
			m_playButton->SetFrameNumber(1);
		}
		else if (m_playButton->GetFrameNumber() != 0)
		{
			m_playButton->SetFrameNumber(0);
		}

		if (m_backButton->GetSpritesheet()->GetFrameRect(0).Translated(m_backButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
		{
			m_backButton->SetFrameNumber(1);
		}
		else if (m_backButton->GetFrameNumber() != 0)
		{
			m_backButton->SetFrameNumber(0);
		}
	}
}
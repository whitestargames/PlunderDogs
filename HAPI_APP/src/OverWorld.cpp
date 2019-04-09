#include  "Overworld.h"
#include "Utilities/Utilities.h"

OverWorld::OverWorld()
	: m_battleMapBackground(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "TempBattleMapBackground.png")),
	m_enemyTerritoryHexSheet(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "EnemyTerritoryHexSheet.png", 2)),
	m_prebattleUIBackground(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "PrebattleUI.png")),
	m_playButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "PrebattleUIPlayButton.png", 2)),
	m_backButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "PrebattleUIBackButton.png", 2)),
	m_upgradesButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "upgradesButton.png", 2)),
	//upgrade background
	//upgrade buttons
	m_upgradesScreenBackground(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "upgradesScreenBackground.png")),
	m_upgradeBackButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "PrebattleUIBackButton.png", 2)),
	m_removeHealthButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "removeButton.png", 2)),
	m_removeMovementButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "removeButton.png", 2)),
	m_removeDamageButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "removeButton.png", 2)),
	m_removeRangeButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "removeButton.png", 2)),
	m_addHealthButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "addButton.png", 2)),
	m_addMovementButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "addButton.png", 2)),
	m_addDamageButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "addButton.png", 2)),
	m_addRangeButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "addButton.png", 2)),

	m_currentWindow(OverWorldWindow::LevelSelection)
{
	m_enemyTerritoryHexSheet->GetTransformComp().SetPosition({ 100, 600 });
	m_upgradesButton->GetTransformComp().SetPosition({ 1300, 25 });

	m_playButton->GetTransformComp().SetPosition({ 1150, 722 });
	m_backButton->GetTransformComp().SetPosition({ 185, 747 });

	//upgrade buttons positions
	m_upgradesScreenBackground->GetTransformComp().SetPosition({ 185, 50 });
	m_upgradeBackButton->GetTransformComp().SetPosition({ 1190, 785 });
	m_removeHealthButton->GetTransformComp().SetPosition({ 625, 150 });//remove buttons
	m_removeMovementButton->GetTransformComp().SetPosition({ 625, 280 });
	m_removeDamageButton->GetTransformComp().SetPosition({ 625, 410 });
	m_removeRangeButton->GetTransformComp().SetPosition({ 625, 540 });
	m_addHealthButton->GetTransformComp().SetPosition({ 875, 150 });//add buttons
	m_addMovementButton->GetTransformComp().SetPosition({ 875, 280 });
	m_addDamageButton->GetTransformComp().SetPosition({ 875, 410 });
	m_addRangeButton->GetTransformComp().SetPosition({ 875, 540 });
}

void OverWorld::render()
{
	SCREEN_SURFACE->Clear();

	m_battleMapBackground->Render(SCREEN_SURFACE);
	m_enemyTerritoryHexSheet->Render(SCREEN_SURFACE);
	m_upgradesButton->Render(SCREEN_SURFACE);

	switch (m_currentWindow)
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
		battleState = true;
		break;
	}
	case OverWorldWindow::LevelSelection ://level selection always wants to be rendered behind the other smaller overworld windows so is outside the switch case
	{
		break;
	}
	case OverWorldWindow::Upgrade :
	{
		m_upgradesScreenBackground->Render(SCREEN_SURFACE);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1170, 150), HAPISPACE::Colour255::BLACK, "36", 50);//draw text gold
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1110, 270), HAPISPACE::Colour255::BLACK, "BUY", 50);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1110, 315), HAPISPACE::Colour255::BLACK, "SHIPS", 50);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(710, 150), HAPISPACE::Colour255::BLACK, "56", 50);//draw stat text*4
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(710, 280), HAPISPACE::Colour255::BLACK, "6", 50);//draw stat text*4
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(710, 410), HAPISPACE::Colour255::BLACK, "4", 50);//draw stat text*4
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(710, 540), HAPISPACE::Colour255::BLACK, "3", 50);//draw stat text*4
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(640, 670), HAPISPACE::Colour255::BLACK, "UPGRADES: " "0" "/" "2", 50);
		m_upgradeBackButton->Render(SCREEN_SURFACE);
		//if render "+" button*5
		//if render "-" button*5
		m_removeHealthButton->Render(SCREEN_SURFACE);
		m_removeMovementButton->Render(SCREEN_SURFACE);
		m_removeDamageButton->Render(SCREEN_SURFACE);
		m_removeRangeButton->Render(SCREEN_SURFACE);
		m_addHealthButton->Render(SCREEN_SURFACE);
		m_addMovementButton->Render(SCREEN_SURFACE);
		m_addDamageButton->Render(SCREEN_SURFACE);
		m_addRangeButton->Render(SCREEN_SURFACE);
		break;
	}
	}
}

void OverWorld::OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData)
{
	if (mouseEvent == EMouseEvent::eLeftButtonDown)
	{
		switch (m_currentWindow)
		{
		case OverWorldWindow::LevelSelection:
		{
			if (m_enemyTerritoryHexSheet->GetSpritesheet()->GetFrameRect(0).Translated(
				m_enemyTerritoryHexSheet->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				m_currentWindow = OverWorldWindow::PreBattle;
			}

			if (m_upgradesButton->GetSpritesheet()->GetFrameRect(0).Translated(
				m_upgradesButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				m_currentWindow = OverWorldWindow::Upgrade;
			}
			break;
		}
		case OverWorldWindow::PreBattle:
		{
			if (m_playButton->GetSpritesheet()->GetFrameRect(0).Translated(m_playButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				m_currentWindow = OverWorldWindow::Battle;
			}
			else if (m_backButton->GetSpritesheet()->GetFrameRect(0).Translated(m_backButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				m_currentWindow = OverWorldWindow::LevelSelection;
			}
			break;
		}
		case OverWorldWindow::Upgrade:
		{
			if (m_upgradeBackButton->GetSpritesheet()->GetFrameRect(0).Translated(m_upgradeBackButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				m_currentWindow = OverWorldWindow::LevelSelection;
			}
			else if (m_addHealthButton->GetSpritesheet()->GetFrameRect(0).Translated(m_addHealthButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				//health+
			}
			else if (m_addMovementButton->GetSpritesheet()->GetFrameRect(0).Translated(m_addMovementButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				//movement+
			}
			else if (m_addDamageButton->GetSpritesheet()->GetFrameRect(0).Translated(m_addDamageButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				//damage+
			}
			else if (m_addRangeButton->GetSpritesheet()->GetFrameRect(0).Translated(m_addRangeButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				//range+
			}
			else if (m_removeHealthButton->GetSpritesheet()->GetFrameRect(0).Translated(m_removeHealthButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				//health-
			}
			else if (m_removeMovementButton->GetSpritesheet()->GetFrameRect(0).Translated(m_removeMovementButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				//movement-
			}
			else if (m_removeDamageButton->GetSpritesheet()->GetFrameRect(0).Translated(m_removeDamageButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				//damage-
			}
			else if (m_removeRangeButton->GetSpritesheet()->GetFrameRect(0).Translated(m_removeRangeButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				//range-
			}
			break;
		}
		}
	}
}

void OverWorld::OnMouseMove(const HAPI_TMouseData& mouseData)
{
	switch (m_currentWindow)
	{
	case OverWorldWindow::LevelSelection :
	{
		if (m_enemyTerritoryHexSheet->GetSpritesheet()->GetFrameRect(0).Translated(m_enemyTerritoryHexSheet->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))//checks if mouse is over button
		{
			m_enemyTerritoryHexSheet->SetFrameNumber(1);//changes the buttons sprite to hover sprite
		}
		else if (m_enemyTerritoryHexSheet->GetFrameNumber() != 0)//if mouse is not over the button and the button has the hover sprite
		{
			m_enemyTerritoryHexSheet->SetFrameNumber(0);// sets it to the default sprite
		}

		if (m_upgradesButton->GetSpritesheet()->GetFrameRect(0).Translated(m_upgradesButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
		{
			m_upgradesButton->SetFrameNumber(1);
		}
		else if (m_upgradesButton->GetFrameNumber() != 0)
		{
			m_upgradesButton->SetFrameNumber(0);
		}
		break;
	}
	case OverWorldWindow::PreBattle :
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
		break;
	}

	case OverWorldWindow::Upgrade :
	{
		if (m_upgradeBackButton->GetSpritesheet()->GetFrameRect(0).Translated(m_upgradeBackButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
		{
			m_upgradeBackButton->SetFrameNumber(1);
		}
		else if (m_upgradeBackButton->GetFrameNumber() != 0)
		{
			m_upgradeBackButton->SetFrameNumber(0);
		}

		if (m_addHealthButton->GetSpritesheet()->GetFrameRect(0).Translated(m_addHealthButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
		{
			m_addHealthButton->SetFrameNumber(1);//health+
		}
		else if (m_addHealthButton->GetFrameNumber() != 0)
		{
			m_addHealthButton->SetFrameNumber(0);
		}
		if (m_addMovementButton->GetSpritesheet()->GetFrameRect(0).Translated(m_addMovementButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
		{
			m_addMovementButton->SetFrameNumber(1);//movement+
		}
		else if (m_addMovementButton->GetFrameNumber() != 0)
		{
			m_addMovementButton->SetFrameNumber(0);
		}
		if (m_addDamageButton->GetSpritesheet()->GetFrameRect(0).Translated(m_addDamageButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
		{
			m_addDamageButton->SetFrameNumber(1);//damage+
		}
		else if (m_addDamageButton->GetFrameNumber() != 0)
		{
			m_addDamageButton->SetFrameNumber(0);
		}
		if (m_addRangeButton->GetSpritesheet()->GetFrameRect(0).Translated(m_addRangeButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
		{
			m_addRangeButton->SetFrameNumber(1);//range+
		}
		else if (m_addRangeButton->GetFrameNumber() != 0)
		{
			m_addRangeButton->SetFrameNumber(0);
		}
		if (m_removeHealthButton->GetSpritesheet()->GetFrameRect(0).Translated(m_removeHealthButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
		{
			m_removeHealthButton->SetFrameNumber(1);//health-
		}
		else if (m_removeHealthButton->GetFrameNumber() != 0)
		{
			m_removeHealthButton->SetFrameNumber(0);
		}
		if (m_removeMovementButton->GetSpritesheet()->GetFrameRect(0).Translated(m_removeMovementButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
		{
			m_removeMovementButton->SetFrameNumber(1);//movement-
		}
		else if (m_removeMovementButton->GetFrameNumber() != 0)
		{
			m_removeMovementButton->SetFrameNumber(0);
		}
		if (m_removeDamageButton->GetSpritesheet()->GetFrameRect(0).Translated(m_removeDamageButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
		{
			m_removeDamageButton->SetFrameNumber(1);//damage-
		}
		else if (m_removeDamageButton->GetFrameNumber() != 0)
		{
			m_removeDamageButton->SetFrameNumber(0);
		}
		if (m_removeRangeButton->GetSpritesheet()->GetFrameRect(0).Translated(m_removeRangeButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
		{
			m_removeRangeButton->SetFrameNumber(1);//range-
		}
		else if (m_removeRangeButton->GetFrameNumber() != 0)
		{
			m_removeRangeButton->SetFrameNumber(0);
		}

		break;
	}
	}
}
#include  "OverworldUI.h"


bool OverworldUIWIndowTest::Initialise()
{
	testBattleMapWindow = true;
	
	EnemyTerritoryHexSheet->GetTransformComp().SetPosition({ 100, 600 });
	PlayButton->GetTransformComp().SetPosition({ 1150, 722 });
	BackButton->GetTransformComp().SetPosition({ 185, 747 });

	//UI.AddWindow("testWindow", HAPISPACE::RectangleI(220, 1050, 510, 710));
	//for (int i = 0; i < m_entityVector.size(); i++)
	//{
	//	UI.GetWindow("testWindow")->AddCanvas("entity" + std::to_string(i), HAPISPACE::RectangleI(50 * i, (50 * i) + 50, 0, 100), m_entityVector[i].getSpritePtr());
	//}

	return true;
}

void OverworldUIWIndowTest::Update()
{
	SCREEN_SURFACE->Clear();

	BattleMapBackground->Render(SCREEN_SURFACE);
	EnemyTerritoryHexSheet->Render(SCREEN_SURFACE);

	if (EnemyTerritoryHexSheet->GetFrameNumber() == 0)//only shows the difficulty number of the hex if the mouse isn't hovered over it
	{
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(EnemyTerritoryHexSheet->GetTransformComp().GetPosition().x + EnemyTerritoryHexSheet->GetCurrentFrame().rect.right / 2.5, EnemyTerritoryHexSheet->GetTransformComp().GetPosition().y + EnemyTerritoryHexSheet->GetCurrentFrame().rect.bottom / 4), difficultyColour, std::to_string(testHexDifficulty), 90);
	}

	if (testPrebattleWindow)
	{
		PrebattleUIBackground->Render(SCREEN_SURFACE);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 270), HAPISPACE::Colour255::BLACK, "45/55", 50);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 355), HAPISPACE::Colour255::BLACK, "3", 50);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 445), HAPISPACE::Colour255::BLACK, "4", 50);
		SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 535), HAPISPACE::Colour255::BLACK, "5", 50);
		PlayButton->Render(SCREEN_SURFACE);
		BackButton->Render(SCREEN_SURFACE);
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
		if (testBattleMapWindow && !testPrebattleWindow)
		{
			if (EnemyTerritoryHexSheet->GetSpritesheet()->GetFrameRect(0).Translated(EnemyTerritoryHexSheet->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				testPrebattleWindow = true;
				UI.OpenWindow("testWindow");
			}
		}
		else if (testPrebattleWindow)
		{
			if (PlayButton->GetSpritesheet()->GetFrameRect(0).Translated(PlayButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				BattleSystem world;
				UI.CloseWindow("testWindow");
				world.run();
			}
			else if (BackButton->GetSpritesheet()->GetFrameRect(0).Translated(BackButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				testPrebattleWindow = false;
				UI.CloseWindow("testWindow");
			}
		}
	}
}

void OverworldUIWIndowTest::OnMouseMove(const HAPI_TMouseData& mouseData)
{
	if (testBattleMapWindow && !testPrebattleWindow)//the battle map is active in the background of the prebattle so we need both checks
	{
		if (EnemyTerritoryHexSheet->GetSpritesheet()->GetFrameRect(0).Translated(EnemyTerritoryHexSheet->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))//checks if mouse is over button
		{
			EnemyTerritoryHexSheet->SetFrameNumber(1);//changes the buttons sprite to hover sprite
		}
		else if (EnemyTerritoryHexSheet->GetFrameNumber() != 0)//if mouse is not over the button and the button has the hover sprite
		{
			EnemyTerritoryHexSheet->SetFrameNumber(0);// sets it to the default sprite
		}
	}
	else if (testPrebattleWindow)
	{
		if (PlayButton->GetSpritesheet()->GetFrameRect(0).Translated(PlayButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
		{
			PlayButton->SetFrameNumber(1);
		}
		else if (PlayButton->GetFrameNumber() != 0)
		{
			PlayButton->SetFrameNumber(0);
		}

		if (BackButton->GetSpritesheet()->GetFrameRect(0).Translated(BackButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
		{
			BackButton->SetFrameNumber(1);
		}
		else if (BackButton->GetFrameNumber() != 0)
		{
			BackButton->SetFrameNumber(0);
		}
	}
}
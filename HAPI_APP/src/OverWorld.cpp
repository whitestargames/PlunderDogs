#include  "Overworld.h"
#include "Battle.h"
#include "Utilities/Utilities.h"

OverWorld::OverWorld()
	: m_battleMapBackground(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "TempBattleMapBackground.png")),
	m_enemyTerritoryHexSheet(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "EnemyTerritoryHexSheet.png", 2)),
	m_prebattleUIBackground(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "PrebattleUI.png")),
	m_playButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "PrebattleUIPlayButton.png", 2)),
	m_backButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "PrebattleUIBackButton.png", 2)),
	m_currentWindow(OverWorldWindow::LevelSelection)
{
	m_enemyTerritoryHexSheet->GetTransformComp().SetPosition({ 100, 600 });
	m_playButton->GetTransformComp().SetPosition({ 1150, 722 });
	m_backButton->GetTransformComp().SetPosition({ 185, 747 });
	for (int i = 0; i < 20; i++)
	{
		Entity newEntity(Utilities::getDataDirectory() + "thingy.xml");
		newEntity.m_healthPoints = i + 1;
		newEntity.m_damage = i + 2;
		newEntity.m_range = i + 3;
		m_entityVector.push_back(newEntity);
	}

	//adding the windows and sliders, also populates the fleet window with all current entities
	UI.AddWindow("fleetWindow", HAPISPACE::RectangleI(220, 1050, 510, 710));
	for (int i = 0; i < m_entityVector.size(); i++)
	{
		UI.GetWindow("fleetWindow")->AddCanvas("entity" + std::to_string(i), HAPISPACE::RectangleI(50 * i, (50 * i) + 50, 0, 100), m_entityVector[i].m_sprite);
	}
	UI.GetWindow("fleetWindow")->AddSlider("fleetSlider", HAPISPACE::RectangleI(0, 830, 110, 210), sliderLayout);
	UI.AddWindow("battleFleetWindow", HAPISPACE::RectangleI(220, 1050, 220, 420));
	UI.GetWindow("battleFleetWindow")->AddSlider("battleFleetSlider", HAPISPACE::RectangleI(0, 830, 110, 210), sliderLayout);
}

void OverWorld::render()
{
	SCREEN_SURFACE->Clear();

	switch (m_currentWindow)
	{
	case OverWorldWindow::PreBattle :
	{
		m_prebattleUIBackground->Render(SCREEN_SURFACE);
		m_playButton->Render(SCREEN_SURFACE);
		m_backButton->Render(SCREEN_SURFACE);
		if (m_currentlySelected != nullptr)
		{
			SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 270), HAPISPACE::Colour255::BLACK, std::to_string(m_currentlySelected->m_healthPoints), 50);
			SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 355), HAPISPACE::Colour255::BLACK, std::to_string(m_currentlySelected->m_damage), 50);
			SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 445), HAPISPACE::Colour255::BLACK, std::to_string(m_currentlySelected->m_movementPoints), 50);
			SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 535), HAPISPACE::Colour255::BLACK, std::to_string(m_currentlySelected->m_range), 50);
		}
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
		switch (m_currentWindow)
		{
		case OverWorldWindow::LevelSelection:
		{
			if (m_enemyTerritoryHexSheet->GetSpritesheet()->GetFrameRect(0).Translated(
				m_enemyTerritoryHexSheet->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				m_currentWindow = OverWorldWindow::PreBattle;
				UI.OpenWindow("fleetWindow");
				UI.OpenWindow("battleFleetWindow");
			}
			break;
		}
		case OverWorldWindow::PreBattle:
		{
			if (m_playButton->GetSpritesheet()->GetFrameRect(0).Translated(m_playButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				UI.CloseWindow("fleetWindow");
				UI.CloseWindow("battleFleetWindow");
				m_currentWindow = OverWorldWindow::Battle;
			}
			else if (m_backButton->GetSpritesheet()->GetFrameRect(0).Translated(m_backButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				m_currentWindow = OverWorldWindow::LevelSelection;
				UI.CloseWindow("fleetWindow");
				UI.CloseWindow("battleFleetWindow");
			}
			bool selection = false;

			//loops through the entity vector to make sure the object is positioned correctly and tests to see if the mouse is on one of the objects.
			//selects a ship to display the stats of
			if (UI.GetWindow("fleetWindow")->GetScreenRect().Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
			{
				for (int i = 0; i < m_entityVector.size(); i++)
				{
					UI.GetWindow("fleetWindow")->PositionObject(
						"entity" + std::to_string(i),
						HAPISPACE::VectorI((50 * i) - ((50 * m_entityVector.size()) * UI.GetWindow("fleetWindow")->GetObject("fleetSlider")->GetValue()), 0));
					if (UI.GetWindow("fleetWindow")->GetObject("entity" + std::to_string(i))->GetBoundingRectangleScreenSpace(HAPISPACE::VectorI(220, 510)).Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
					{
						m_currentlySelected = &m_entityVector[i];
						selection = true;
					}
				}
			}
			//same for the battlefleet window
			if (UI.GetWindow("battleFleetWindow")->GetScreenRect().Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
			{
				for (int i = 0; i < m_selectedEntities.size(); i++)
				{
					UI.GetWindow("battleFleetWindow")->PositionObject("entity" + std::to_string(i), HAPISPACE::VectorI((50 * i) - ((50 * m_entityVector.size()) * UI.GetWindow("battleFleetWindow")->GetObject("battleFleetSlider")->GetValue()), 0));
					if (UI.GetWindow("battleFleetWindow")->GetObject("entity" + std::to_string(i))->GetBoundingRectangleScreenSpace(HAPISPACE::VectorI(220, 220)).Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
					{
						m_currentlySelected = *&m_selectedEntities[i];
						selection = true;
					}
				}
			}
			if (selection == false)
			{
				m_currentlySelected = nullptr;
			}
			break;
		}
		}
	}
	if (mouseEvent == EMouseEvent::eRightButtonDown)
	{
		switch (m_currentWindow)
		{
		case OverWorldWindow::PreBattle :
		{
			//loops through the entity vector to make sure the object is positioned correctly and tests to see if the mouse is on one of the objects.
			//selects a ship to go into battle
			if (UI.GetWindow("fleetWindow")->GetScreenRect().Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
			{
				for (int i = 0; i < m_entityVector.size(); i++)
				{
					UI.GetWindow("fleetWindow")->PositionObject("entity" + std::to_string(i), HAPISPACE::VectorI((50 * i) - ((50 * m_entityVector.size()) * UI.GetWindow("fleetWindow")->GetObject("fleetSlider")->GetValue()), 0));
					if (UI.GetWindow("fleetWindow")->GetObject("entity" + std::to_string(i))->GetBoundingRectangleScreenSpace(HAPISPACE::VectorI(220, 510)).Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
					{
						m_selectedEntities.push_back(&m_entityVector[i]);
						for (int i = 0; i < m_selectedEntities.size(); i++)
						{
							if (UI.GetWindow("battleFleetWindow")->GetObject("entity") == nullptr)
							{
								UI.GetWindow("battleFleetWindow")->AddCanvas("entity" + std::to_string(i), HAPISPACE::RectangleI(50 * i, (50 * i) + 50, 0, 100), m_selectedEntities[i]->m_sprite);
							}
						}
					}
				}
			}
			//same for the battlefleet window but deselects ships
			if (UI.GetWindow("battleFleetWindow")->GetScreenRect().Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
			{
				for (int i = 0; i < m_selectedEntities.size(); i++)
				{
					UI.GetWindow("battleFleetWindow")->PositionObject("entity" + std::to_string(i), HAPISPACE::VectorI((50 * i) - ((50 * m_entityVector.size()) * UI.GetWindow("battleFleetWindow")->GetObject("battleFleetSlider")->GetValue()), 0));
					if (UI.GetWindow("battleFleetWindow")->GetObject("entity" + std::to_string(i))->GetBoundingRectangleScreenSpace(HAPISPACE::VectorI(220, 220)).Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
					{
						for (int i = 0; i < m_selectedEntities.size(); i++)
						{
							UI.GetWindow("battleFleetWindow")->DeleteObject("entity" + std::to_string(i));
						}
						m_selectedEntities.erase(m_selectedEntities.begin() + i);
						for (int i = 0; i < m_selectedEntities.size(); i++)
						{
							if (UI.GetWindow("battleFleetWindow")->GetObject("entity") == nullptr)
							{
								UI.GetWindow("battleFleetWindow")->AddCanvas("entity" + std::to_string(i), HAPISPACE::RectangleI(50 * i, (50 * i) + 50, 0, 100), m_selectedEntities[i]->m_sprite);
							}
						}
					}
				}
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
		//code for using the slider, slider varies from 0 to 1. this code block makes sure that when the slider value changes the window objects get positioned accordingly
		if (mouseData.leftButtonDown)
		{
			if (UI.GetWindow("fleetWindow")->GetScreenRect().Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
			{
				for (int i = 0; i < m_entityVector.size(); i++)
				{
					UI.GetWindow("fleetWindow")->PositionObject("entity" + std::to_string(i), HAPISPACE::VectorI((50 * i) - ((50 * m_entityVector.size()) * UI.GetWindow("fleetWindow")->GetObject("fleetSlider")->GetValue()), 0));
				}
			}
			if (UI.GetWindow("fleetWindow")->GetScreenRect().Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
			{
				for (int i = 0; i < m_selectedEntities.size(); i++)
				{
					UI.GetWindow("battleFleetWindow")->PositionObject("entity" + std::to_string(i), HAPISPACE::VectorI((50 * i) - ((50 * m_selectedEntities.size()) * UI.GetWindow("battleFleetWindow")->GetObject("battleFleetSlider")->GetValue()), 0));
				}
			}
		}
		break;
	}
	}
}
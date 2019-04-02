#include  "Overworld.h"
#include "Battle.h"
#include "Utilities/Utilities.h"

constexpr int WINDOW_OBJECTWIDTH = 75;
constexpr int WINDOW_OBJECTHEIGHT = 150;
constexpr int WINDOW_WIDTH = 830;
constexpr int WINDOW_HEIGHT = 200;

OverWorld::OverWorld()
	: m_battleMapBackground(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "TempBattleMapBackground.png")),
	m_enemyTerritoryHexSheet(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "EnemyTerritoryHexSheet.png", 2)),
	m_prebattleUIBackground(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "PrebattleUI.png")),
	m_playButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "PrebattleUIPlayButton.png", 2)),
	m_backButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "PrebattleUIBackButton.png", 2)),
	m_currentWindow(OverWorldWindow::LevelSelection),
	fleetWindowSkinName(UI.LoadSkin(Utilities::getDataDirectory() + "fleetWindowSkin.xml")),
	fleetWindowSliderSkinName(UI.LoadSkin(Utilities::getDataDirectory() + "fleetWindowSliderSkin.xml"))
{
	m_enemyTerritoryHexSheet->GetTransformComp().SetPosition({ 100, 600 });
	m_playButton->GetTransformComp().SetPosition({ 1150, 722 });
	m_backButton->GetTransformComp().SetPosition({ 185, 747 });
	for (int i = 0; i < 20; i++)
	{
		Entity newEntity(Utilities::getDataDirectory() + "thingy.xml", 5, i + 1, i + 2, i + 3);
		m_entityVector.push_back(newEntity);
	}
	//adding the windows and sliders, also populates the fleet window with all current entities
	UI.AddWindow(FLEET_WINDOW, HAPISPACE::RectangleI(220, 1050, 510, 710), fleetWindowSkinName);
	for (int i = 0; i < m_entityVector.size(); i++)
	{
		UI.GetWindow(FLEET_WINDOW)->AddCanvas(ENTITY + std::to_string(i), calculateObjectWindowPosition(i), m_entityVector[i].m_sprite);
	}
	UI.GetWindow(FLEET_WINDOW)->AddSlider(FLEET_SLIDER, HAPISPACE::RectangleI(0, 830, 160, 210), sliderLayout);
	UI.AddWindow(BATTLE_FLEET_WINDOW, HAPISPACE::RectangleI(220, 1050, 220, 420), fleetWindowSkinName);
	UI.GetWindow(BATTLE_FLEET_WINDOW)->AddSlider(BATTLE_FLEET_SLIDER, HAPISPACE::RectangleI(0, 830, 160, 210), sliderLayout);
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
				UI.OpenWindow(FLEET_WINDOW);
				UI.OpenWindow(BATTLE_FLEET_WINDOW);
			}
			break;
		}
		case OverWorldWindow::PreBattle:
		{
			if (m_playButton->GetSpritesheet()->GetFrameRect(0).Translated(m_playButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				UI.CloseWindow(FLEET_WINDOW);
				UI.CloseWindow(BATTLE_FLEET_WINDOW);
				m_currentWindow = OverWorldWindow::Battle;
			}
			else if (m_backButton->GetSpritesheet()->GetFrameRect(0).Translated(m_backButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				m_currentWindow = OverWorldWindow::LevelSelection;
				UI.CloseWindow(FLEET_WINDOW);
				UI.CloseWindow(BATTLE_FLEET_WINDOW);
			}
			bool selection = false;

			//loops through the entity vector to make sure the object is positioned correctly and tests to see if the mouse is on one of the objects.
			//selects a ship to display the stats of
			if (windowScreenRect(FLEET_WINDOW).Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
			{
				for (int i = 0; i < m_entityVector.size(); i++)
				{
					positionEntity(FLEET_WINDOW, FLEET_SLIDER, ENTITY + std::to_string(i), i, m_entityVector.size());
					if (entityContainsMouse(FLEET_WINDOW, ENTITY + std::to_string(i), m_fleetWindowTopLeft, HAPISPACE::VectorI(mouseData.x, mouseData.y)))
					{
						m_currentlySelected = &m_entityVector[i];
						selection = true;
					}
				}
			}
			//same for the battlefleet window
			if (windowScreenRect(BATTLE_FLEET_WINDOW).Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
			{
				for (int i = 0; i < m_selectedEntities.size(); i++)
				{
					positionEntity(BATTLE_FLEET_WINDOW, BATTLE_FLEET_SLIDER, ENTITY + std::to_string(i), i, m_selectedEntities.size());
					if (entityContainsMouse(BATTLE_FLEET_WINDOW, ENTITY + std::to_string(i), m_battleFleetWindowTopLeft, HAPISPACE::VectorI(mouseData.x, mouseData.y)))
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
			if (windowScreenRect(FLEET_WINDOW).Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
			{
				for (int i = 0; i < m_entityVector.size(); i++)
				{
					positionEntity(FLEET_WINDOW, FLEET_SLIDER, ENTITY + std::to_string(i), i, m_entityVector.size());
					if (entityContainsMouse(FLEET_WINDOW, ENTITY + std::to_string(i), m_fleetWindowTopLeft, HAPISPACE::VectorI(mouseData.x, mouseData.y)))
					{
						bool isSelected{ false };
						for (int it = 0; it < m_selectedEntities.size(); it++)
						{
							if (m_selectedEntities[it] == &m_entityVector[i])
							{
								isSelected = true;
							}
						}
						if (!isSelected)
						{
							m_selectedEntities.push_back(&m_entityVector[i]);
							for (int i = 0; i < m_selectedEntities.size(); i++)
							{
								if (!windowObjectExists(BATTLE_FLEET_WINDOW, ENTITY + std::to_string(i)))
								{
									UI.GetWindow(BATTLE_FLEET_WINDOW)->AddCanvas(ENTITY + std::to_string(i), calculateObjectWindowPosition(i), m_selectedEntities[i]->m_sprite);
								}
							}
						}
					}
				}
			}

			//same for the battlefleet window but deselects ships
			if (windowScreenRect(BATTLE_FLEET_WINDOW).Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
			{
				for (int i = 0; i < m_selectedEntities.size(); i++)
				{
					positionEntity(BATTLE_FLEET_WINDOW, BATTLE_FLEET_SLIDER, ENTITY + std::to_string(i), i, m_selectedEntities.size());
					if (entityContainsMouse(BATTLE_FLEET_WINDOW, ENTITY + std::to_string(i), m_battleFleetWindowTopLeft, HAPISPACE::VectorI(mouseData.x, mouseData.y)))
					{
						for (int i = 0; i < m_selectedEntities.size(); i++)
						{
							UI.GetWindow(BATTLE_FLEET_WINDOW)->DeleteObject(ENTITY + std::to_string(i));
						}
						m_selectedEntities.erase(m_selectedEntities.begin() + i);
						for (int i = 0; i < m_selectedEntities.size(); i++)
						{
	
							if (!windowObjectExists(BATTLE_FLEET_WINDOW, ENTITY + std::to_string(i)))
							{
								UI.GetWindow(BATTLE_FLEET_WINDOW)->AddCanvas(ENTITY + std::to_string(i), calculateObjectWindowPosition(i), m_selectedEntities[i]->m_sprite);
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

		//varies the position of objects based on the slder value
		if (mouseData.leftButtonDown)
		{
			if (windowScreenRect(FLEET_WINDOW).Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
			{
				for (int i = 0; i < m_entityVector.size(); i++)
				{
					positionEntity(FLEET_WINDOW, FLEET_SLIDER, "entity" + std::to_string(i), i, m_entityVector.size());
				}
			}
			if (windowScreenRect(BATTLE_FLEET_WINDOW).Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
			{
				for (int i = 0; i < m_selectedEntities.size(); i++)
				{
					positionEntity(BATTLE_FLEET_WINDOW, BATTLE_FLEET_SLIDER, "entity" + std::to_string(i), i, m_selectedEntities.size());
				}
			}
		}
		break;
	}
	}
}

void OverWorld::positionEntity(const std::string & windowName, const std::string& windowSliderName, const std::string& windowObjectName, int objectNumber, size_t vectorSize)
{
	UI.GetWindow(windowName)->PositionObject(windowObjectName, calculateObjectScrolledPosition(windowName, windowSliderName, objectNumber, vectorSize));
}

float OverWorld::getWindowSliderValue(const std::string & windowName, const std::string & windowSliderName) const
{
	return UI.GetWindow(windowName)->GetObject(windowSliderName)->GetValue();
}

HAPISPACE::RectangleI OverWorld::calculateObjectWindowPosition(int objectNumber) const
{
	return HAPISPACE::RectangleI(WINDOW_OBJECTWIDTH * objectNumber, (WINDOW_OBJECTWIDTH * objectNumber) + WINDOW_OBJECTWIDTH, 0, WINDOW_OBJECTHEIGHT);
}

HAPISPACE::RectangleI OverWorld::windowScreenRect(const std::string & windowName) const
{
	return UI.GetWindow(windowName)->GetScreenRect();
}

HAPISPACE::VectorI OverWorld::calculateObjectScrolledPosition(const std::string & windowName, const std::string & windowSliderName, int objectNumber, size_t vectorSize)
{
	return HAPISPACE::VectorI((WINDOW_OBJECTWIDTH * objectNumber) - ((WINDOW_OBJECTWIDTH * vectorSize) * getWindowSliderValue(windowName, windowSliderName)), 0);
}


bool OverWorld::entityContainsMouse(const std::string & windowName, const std::string& windowObjectName, HAPISPACE::VectorI windowTopLeft, HAPISPACE::VectorI mousePosition) const
{
	if (UI.GetWindow(windowName)->GetObject(windowObjectName)->GetBoundingRectangleScreenSpace(windowTopLeft).Contains(mousePosition))
	{
		return true;
	}
	return false;
}

bool OverWorld::windowObjectExists(const std::string & windowName, const std::string& windowObjectName) const
{
	if (UI.GetWindow(windowName)->GetObject(windowObjectName) != nullptr)
	{
		return true;
	}
	return false;
}

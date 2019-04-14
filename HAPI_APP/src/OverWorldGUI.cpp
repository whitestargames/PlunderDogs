#include "OverWorldGUI.h"
#include "Textures.h"
#include "OverWorld.h"
#include "Utilities/Utilities.h"

OverWorldWindow OverWorldGUI::CURRENT_WINDOW = OverWorldWindow::eLevelSelection;

constexpr int WINDOW_OBJECTWIDTH = 75;
constexpr int WINDOW_OBJECTHEIGHT = 150;
constexpr int WINDOW_WIDTH = 830;
constexpr int WINDOW_HEIGHT = 200;

OverWorldGUI::OverWorldGUI(std::vector<EntityProperties>& entities)
	: m_battleMapBackground(std::make_unique<Sprite>(Textures::m_battleMapBackground)),
	m_enemyTerritoryHexSheet(std::make_unique<Sprite>(Textures::m_enemyTerritoryHexSheet)),
	m_prebattleUIBackground(std::make_unique<Sprite>(Textures::m_prebattleUIBackground)),
	m_playButton(std::make_unique<Sprite>(Textures::m_preBattleUIPlayButton)),
	m_backButton(std::make_unique<Sprite>(Textures::m_preBattleUIBackButton)),
	fleetWindowSkinName(UI.LoadSkin(Utilities::getDataDirectory() + "fleetWindowSkin.xml")),
	fleetWindowSliderSkinName(UI.LoadSkin(Utilities::getDataDirectory() + "fleetWindowSliderSkin.xml")),
	m_currentlySelected(nullptr),
	m_enitiesAdded(false)
{
	HAPI_Wrapper::setPosition(m_enemyTerritoryHexSheet, { 100, 600 });
	HAPI_Wrapper::setPosition(m_playButton, { 1150, 722 });
	HAPI_Wrapper::setPosition(m_backButton, { 185, 747 });
	//adding the windows and sliders, also populates the fleet window with all current entities
	UI.AddWindow(FLEET_WINDOW, HAPISPACE::RectangleI(220, 1050, 510, 710), fleetWindowSkinName);
	for (int i = 0; i < entities.size(); i++) TODO:
	{
		UI.GetWindow(FLEET_WINDOW)->AddCanvas(ENTITY + std::to_string(i), calculateObjectWindowPosition(i), entities[i].m_sprite);
	}
	UI.GetWindow(FLEET_WINDOW)->AddSlider(FLEET_SLIDER, HAPISPACE::RectangleI(0, 830, 160, 210), sliderLayout);
	UI.AddWindow(BATTLE_FLEET_WINDOW, HAPISPACE::RectangleI(220, 1050, 220, 420), fleetWindowSkinName);
	UI.GetWindow(BATTLE_FLEET_WINDOW)->AddSlider(BATTLE_FLEET_SLIDER, HAPISPACE::RectangleI(0, 830, 160, 210), sliderLayout);
}

void OverWorldGUI::render(std::unique_ptr<Battle>& battle)
{
	SCREEN_SURFACE->Clear();

	switch (CURRENT_WINDOW)
	{
		case OverWorldWindow::ePreBattle:
		{
			m_prebattleUIBackground->Render(SCREEN_SURFACE);
			m_playButton->Render(SCREEN_SURFACE);
			m_backButton->Render(SCREEN_SURFACE);
			if (m_currentlySelected != nullptr)
			{
				SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 270), HAPISPACE::Colour255::BLACK, std::to_string(m_currentlySelected->m_currentHealth), 50);
				SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 355), HAPISPACE::Colour255::BLACK, std::to_string(m_currentlySelected->m_damage), 50);
				SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 445), HAPISPACE::Colour255::BLACK, std::to_string(m_currentlySelected->m_movementPoints), 50);
				SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1440, 535), HAPISPACE::Colour255::BLACK, std::to_string(m_currentlySelected->m_range), 50);
			}
			
			break;
		}
		case OverWorldWindow::eBattle:
		{
			assert(battle.get());
			
			battle->render();
			break;
		}
		case OverWorldWindow::eLevelSelection:
		{
			HAPI_Wrapper::render(m_battleMapBackground);
			HAPI_Wrapper::render(m_enemyTerritoryHexSheet);
			break;
		}
	}
}

void OverWorldGUI::onLeftClick(const HAPI_TMouseData& mouseData, std::vector<EntityProperties>& entities,
	std::vector<EntityProperties*>& selectedEntities, bool& startBattle)
{
	switch (CURRENT_WINDOW)
	{
		case OverWorldWindow::eLevelSelection:
		{
			if (HAPI_Wrapper::isTranslated(m_enemyTerritoryHexSheet, mouseData, 0))
			{
				CURRENT_WINDOW = OverWorldWindow::ePreBattle;
				UI.OpenWindow(FLEET_WINDOW);
				UI.OpenWindow(BATTLE_FLEET_WINDOW);
			}
			break;
		}
		case OverWorldWindow::ePreBattle:
		{
			if (HAPI_Wrapper::isTranslated(m_playButton, mouseData, 0))
			{
				UI.CloseWindow(FLEET_WINDOW);
				UI.CloseWindow(BATTLE_FLEET_WINDOW);
				
				CURRENT_WINDOW = OverWorldWindow::eBattle;
				startBattle = true;
			}
			else if (HAPI_Wrapper::isTranslated(m_backButton, mouseData, 0))
			{
				CURRENT_WINDOW = OverWorldWindow::eLevelSelection;
				UI.CloseWindow(FLEET_WINDOW);
				UI.CloseWindow(BATTLE_FLEET_WINDOW);
			}
			bool selection = false;

			//loops through the entity vector to make sure the object is positioned correctly and tests to see if the mouse is on one of the objects.
			//selects a ship to display the stats of
			if (windowScreenRect(FLEET_WINDOW).Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
			{
				for (int i = 0; i < entities.size(); i++)
				{
					positionEntity(FLEET_WINDOW, FLEET_SLIDER, ENTITY + std::to_string(i), i, entities.size());
					if (entityContainsMouse(FLEET_WINDOW, ENTITY + std::to_string(i), m_fleetWindowTopLeft, HAPISPACE::VectorI(mouseData.x, mouseData.y)))
					{
						m_currentlySelected = &entities[i];
						selection = true;
					}
				}
			}
			//same for the battlefleet window
			if (windowScreenRect(BATTLE_FLEET_WINDOW).Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
			{
				for (int i = 0; i < selectedEntities.size(); i++)
				{
					positionEntity(BATTLE_FLEET_WINDOW, BATTLE_FLEET_SLIDER, ENTITY + std::to_string(i), i, selectedEntities.size());
					if (entityContainsMouse(BATTLE_FLEET_WINDOW, ENTITY + std::to_string(i), m_battleFleetWindowTopLeft, HAPISPACE::VectorI(mouseData.x, mouseData.y)))
					{
						m_currentlySelected = selectedEntities[i];
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

void OverWorldGUI::onRightClick(const HAPI_TMouseData& mouseData, std::vector<EntityProperties>& entities,
	std::vector<EntityProperties*>& selectedEntities)
{
	switch (CURRENT_WINDOW)
	{
	case OverWorldWindow::ePreBattle:
	{
		//loops through the entity vector to make sure the object is positioned correctly and tests to see if the mouse is on one of the objects.
		//selects a ship to go into battle
		if (windowScreenRect(FLEET_WINDOW).Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
		{
			for (int i = 0; i < entities.size(); i++)
			{
				positionEntity(FLEET_WINDOW, FLEET_SLIDER, (ENTITY + std::to_string(i)), i, entities.size());
				if (entityContainsMouse(FLEET_WINDOW, ENTITY + std::to_string(i), m_fleetWindowTopLeft, HAPISPACE::VectorI(mouseData.x, mouseData.y)))
				{
					bool isSelected{ false };
					for (int it = 0; it < selectedEntities.size(); it++)
					{
						if (selectedEntities[it] == &entities[i])
						{
							isSelected = true;
						}
					}
					if (!isSelected)
					{
						selectedEntities.push_back(&entities[i]);
						for (int j = 0; j < selectedEntities.size(); j++)
						{
							if (!windowObjectExists(BATTLE_FLEET_WINDOW, ENTITY + std::to_string(j)))
							{
								UI.GetWindow(BATTLE_FLEET_WINDOW)->AddCanvas(ENTITY + std::to_string(j), calculateObjectWindowPosition(j), selectedEntities[j]->m_sprite);
							}
						}
					}
				}
			}
		}

		//same for the battlefleet window but deselects ships
		if (windowScreenRect(BATTLE_FLEET_WINDOW).Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
		{
			for (int i = 0; i < selectedEntities.size(); i++)
			{
				positionEntity(BATTLE_FLEET_WINDOW, BATTLE_FLEET_SLIDER, ENTITY + std::to_string(i), i, selectedEntities.size());
				if (entityContainsMouse(BATTLE_FLEET_WINDOW, ENTITY + std::to_string(i), m_battleFleetWindowTopLeft, HAPISPACE::VectorI(mouseData.x, mouseData.y)))
				{
					for (int j = 0; j < selectedEntities.size(); j++)
					{
						UI.GetWindow(BATTLE_FLEET_WINDOW)->DeleteObject(ENTITY + std::to_string(j));
					}
					selectedEntities.erase(selectedEntities.begin() + i);
					for (int j = 0; j < selectedEntities.size(); j++)
					{

						if (!windowObjectExists(BATTLE_FLEET_WINDOW, ENTITY + std::to_string(j)))
						{
							UI.GetWindow(BATTLE_FLEET_WINDOW)->AddCanvas(ENTITY + std::to_string(j), calculateObjectWindowPosition(j), selectedEntities[j]->m_sprite);
						}
					}
				}
			}
		}
		break;
	}
	}
}

void OverWorldGUI::onMouseMove(const HAPI_TMouseData& mouseData, std::vector<EntityProperties>& entities,
	std::vector<EntityProperties*>& selectedEntities)
{
	switch (CURRENT_WINDOW)
	{
	case OverWorldWindow::eLevelSelection:
	{
		if (HAPI_Wrapper::isTranslated(m_enemyTerritoryHexSheet, mouseData, 0))//checks if mouse is over button
		{
			m_enemyTerritoryHexSheet->SetFrameNumber(1);//changes the buttons sprite to hover sprite
		}
		else if (m_enemyTerritoryHexSheet->GetFrameNumber() != 0)//if mouse is not over the button and the button has the hover sprite
		{
			m_enemyTerritoryHexSheet->SetFrameNumber(0);// sets it to the default sprite
		}
		break;
	}
	case OverWorldWindow::ePreBattle:
	{
		if (HAPI_Wrapper::isTranslated(m_playButton, mouseData, 0))
		{
			m_playButton->SetFrameNumber(1);
		}
		else if (m_playButton->GetFrameNumber() != 0)
		{
			m_playButton->SetFrameNumber(0);
		}

		if (HAPI_Wrapper::isTranslated(m_backButton, mouseData, 0))
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
				for (int i = 0; i < entities.size(); i++)
				{
					positionEntity(FLEET_WINDOW, FLEET_SLIDER, "entity" + std::to_string(i), i, entities.size());
				}
			}
			if (windowScreenRect(BATTLE_FLEET_WINDOW).Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
			{
				for (int i = 0; i < selectedEntities.size(); i++)
				{
					positionEntity(BATTLE_FLEET_WINDOW, BATTLE_FLEET_SLIDER, "entity" + std::to_string(i), i, selectedEntities.size());
				}
			}
		}
		break;
	}
	}
}

void OverWorldGUI::positionEntity(const std::string & windowName, const std::string& windowSliderName, const std::string& windowObjectName, int objectNumber, size_t vectorSize)
{
	UI.GetWindow(windowName)->PositionObject(windowObjectName, calculateObjectScrolledPosition(windowName, windowSliderName, objectNumber, vectorSize));
}

float OverWorldGUI::getWindowSliderValue(const std::string & windowName, const std::string & windowSliderName) const
{
	return UI.GetWindow(windowName)->GetObject(windowSliderName)->GetValue();
}

HAPISPACE::RectangleI OverWorldGUI::calculateObjectWindowPosition(int objectNumber) const
{
	return HAPISPACE::RectangleI(WINDOW_OBJECTWIDTH * objectNumber, (WINDOW_OBJECTWIDTH * objectNumber) + WINDOW_OBJECTWIDTH, 0, WINDOW_OBJECTHEIGHT);
}

HAPISPACE::RectangleI OverWorldGUI::windowScreenRect(const std::string & windowName) const
{
	return UI.GetWindow(windowName)->GetScreenRect();
}

HAPISPACE::VectorI OverWorldGUI::calculateObjectScrolledPosition(const std::string & windowName, const std::string & windowSliderName, int objectNumber, size_t vectorSize)
{
	return HAPISPACE::VectorI((WINDOW_OBJECTWIDTH * objectNumber) - ((WINDOW_OBJECTWIDTH * vectorSize) * getWindowSliderValue(windowName, windowSliderName)), 0 );
}

bool OverWorldGUI::entityContainsMouse(const std::string & windowName, const std::string& windowObjectName, HAPISPACE::VectorI windowTopLeft, HAPISPACE::VectorI mousePosition) const
{
	if (UI.GetWindow(windowName)->GetObject(windowObjectName)->GetBoundingRectangleScreenSpace(windowTopLeft).Contains(mousePosition))
	{
		return true;
	}
	return false;
}

bool OverWorldGUI::windowObjectExists(const std::string & windowName, const std::string& windowObjectName) const
{
	if (UI.GetWindow(windowName)->GetObject(windowObjectName) != nullptr)
	{
		return true;
	}
	return false;
}
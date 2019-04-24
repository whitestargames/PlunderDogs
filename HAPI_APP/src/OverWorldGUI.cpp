#include "OverWorldGUI.h"
#include "Textures.h"
#include "OverWorld.h"
#include "Utilities/Utilities.h"
#include "GameEventMessenger.h"

OverWorldWindow OverWorldGUI::CURRENT_WINDOW = OverWorldWindow::eLevelSelection;

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
	fleetWindowSliderSkinName(UI.LoadSkin(Utilities::getDataDirectory() + "fleetWindowSliderSkin.xml")),
	m_currentlySelected(nullptr),
	m_enitiesAdded(false),
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
	m_addRangeButton(HAPI_Sprites.MakeSprite(Utilities::getDataDirectory() + "addButton.png", 2))
{
	m_background = std::make_unique<Sprite>(Textures::m_background);
	HAPI_Wrapper::setPosition(m_background, { 0, 0 });
	GameEventMessenger::getInstance().subscribe(std::bind(&OverWorldGUI::onReset, this), "OverWorldGUI", GameEvent::eResetBattle);
}

OverWorldGUI::~OverWorldGUI()
{
	GameEventMessenger::getInstance().unsubscribe("OverWorldGUI", GameEvent::eResetBattle);
}

void OverWorldGUI::render(Battle& battle)
{
	SCREEN_SURFACE->Clear();

	switch (CURRENT_WINDOW)
	{
		case OverWorldWindow::ePreBattle:
		{
			m_prebattleUIBackground->Render(SCREEN_SURFACE);
			m_playButton->Render(SCREEN_SURFACE);
			m_backButton->Render(SCREEN_SURFACE);
			m_upgradesButton->Render(SCREEN_SURFACE);
			if (m_currentlySelected != nullptr)
			{
				SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1600, 360), HAPISPACE::Colour255::BLACK, std::to_string(m_currentlySelected->m_currentHealth), 50);
				SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1600, 445), HAPISPACE::Colour255::BLACK, std::to_string(m_currentlySelected->m_damage), 50);
				SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1600, 535), HAPISPACE::Colour255::BLACK, std::to_string(m_currentlySelected->m_movementPoints), 50);
				SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1600, 625), HAPISPACE::Colour255::BLACK, std::to_string(m_currentlySelected->m_range), 50);
			}
			
			break;
		}
		case OverWorldWindow::eBattle:
		{
			battle.render();
			break;
		}
		case OverWorldWindow::eLevelSelection:
		{
			//HAPI_Wrapper::render(m_battleMapBackground);
			HAPI_Wrapper::render(m_background);
			HAPI_Wrapper::render(m_enemyTerritoryHexSheet);

			SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1160, 340), HAPISPACE::Colour255::BLACK, "Plunder Dogs", 105, {}, {}, 2.5f);
			break;
		}
		case OverWorldWindow::eUpgrade:
		{
			m_upgradesScreenBackground->Render(SCREEN_SURFACE);
			SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1330, 240), HAPISPACE::Colour255::BLACK, "36", 50);//draw text gold
			SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1270, 160), HAPISPACE::Colour255::BLACK, "BUY", 50);
			SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1270, 405), HAPISPACE::Colour255::BLACK, "SHIPS", 50);
			SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(870, 240), HAPISPACE::Colour255::BLACK, "56", 50);//draw stat text*4
			SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(870, 370), HAPISPACE::Colour255::BLACK, "6", 50);//draw stat text*4
			SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(870, 500), HAPISPACE::Colour255::BLACK, "4", 50);//draw stat text*4
			SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(870, 630), HAPISPACE::Colour255::BLACK, "3", 50);//draw stat text*4
			SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(800, 760), HAPISPACE::Colour255::BLACK, "UPGRADES: " "0" "/" "2", 50);
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

void OverWorldGUI::onLeftClick(const HAPI_TMouseData& mouseData, Player& currentSelectedPlayer, bool& selectNextPlayer)
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
			if (HAPI_Wrapper::isTranslated(m_upgradesButton, mouseData, 0))
			{
				CURRENT_WINDOW = OverWorldWindow::eUpgrade;
			}
			break;
		}
		//Play Button
		case OverWorldWindow::ePreBattle:
		{
			if (HAPI_Wrapper::isTranslated(m_playButton, mouseData, 0))
			{
				if (!currentSelectedPlayer.m_selectedEntities.empty())
				{
					CURRENT_WINDOW = OverWorldWindow::eLevelSelection;
					selectNextPlayer = true;
					return;
				}


				////TODO: Change at some point
				//if (!currentSelectedPlayer.m_selectedEntities.empty())
				//{
				//	UI.CloseWindow(FLEET_WINDOW);
				//	UI.CloseWindow(BATTLE_FLEET_WINDOW);
				//	CURRENT_WINDOW = OverWorldWindow::eBattle;
				//	startBattle = true;
				//}
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
				for (int i = 0; i < currentSelectedPlayer.m_entities.size(); i++)
				{
					positionEntity(FLEET_WINDOW, FLEET_SLIDER, ENTITY + std::to_string(i), i, currentSelectedPlayer.m_entities.size());
					if (entityContainsMouse(FLEET_WINDOW, ENTITY + std::to_string(i), m_fleetWindowTopLeft, HAPISPACE::VectorI(mouseData.x, mouseData.y)))
					{
						m_currentlySelected = &currentSelectedPlayer.m_entities[i];
						selection = true;
					}
				}
			}
			//same for the battlefleet window
			if (windowScreenRect(BATTLE_FLEET_WINDOW).Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
			{
				for (int i = 0; i < currentSelectedPlayer.m_selectedEntities.size(); i++)
				{
					positionEntity(BATTLE_FLEET_WINDOW, BATTLE_FLEET_SLIDER, ENTITY + std::to_string(i), i, currentSelectedPlayer.m_selectedEntities.size());
					if (entityContainsMouse(BATTLE_FLEET_WINDOW, ENTITY + std::to_string(i), m_battleFleetWindowTopLeft, HAPISPACE::VectorI(mouseData.x, mouseData.y)))
					{
						m_currentlySelected = currentSelectedPlayer.m_selectedEntities[i];
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
		case OverWorldWindow::eUpgrade:
		{
			if (m_upgradeBackButton->GetSpritesheet()->GetFrameRect(0).Translated(m_upgradeBackButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				CURRENT_WINDOW = OverWorldWindow::eLevelSelection;
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

void OverWorldGUI::onRightClick(const HAPI_TMouseData& mouseData, Player& currentSelectedPlayer)
{
	switch (CURRENT_WINDOW)
	{
	case OverWorldWindow::ePreBattle:
	{
		//loops through the entity vector to make sure the object is positioned correctly and tests to see if the mouse is on one of the objects.
		//selects a ship to go into battle
		if (windowScreenRect(FLEET_WINDOW).Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
		{
			for (int i = 0; i < currentSelectedPlayer.m_entities.size(); i++)
			{
				positionEntity(FLEET_WINDOW, FLEET_SLIDER, (ENTITY + std::to_string(i)), i, currentSelectedPlayer.m_entities.size());
				if (entityContainsMouse(FLEET_WINDOW, ENTITY + std::to_string(i), m_fleetWindowTopLeft, HAPISPACE::VectorI(mouseData.x, mouseData.y)))
				{
					bool isSelected{ false };
					for (int it = 0; it < currentSelectedPlayer.m_selectedEntities.size(); it++)
					{
						if (currentSelectedPlayer.m_selectedEntities[it] == &currentSelectedPlayer.m_entities[i])
						{
							isSelected = true;
						}
					}
					if (!isSelected)
					{
						currentSelectedPlayer.m_selectedEntities.push_back(&currentSelectedPlayer.m_entities[i]);
						for (int j = 0; j < currentSelectedPlayer.m_selectedEntities.size(); j++)
						{
							if (!windowObjectExists(BATTLE_FLEET_WINDOW, ENTITY + std::to_string(j)))
							{
								UI.GetWindow(BATTLE_FLEET_WINDOW)->AddCanvas(ENTITY + std::to_string(j), calculateObjectWindowPosition(j), currentSelectedPlayer.m_selectedEntities[j]->m_sprite);
							}
						}
					}
				}
			}
		}

		//same for the battlefleet window but deselects ships
		if (windowScreenRect(BATTLE_FLEET_WINDOW).Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
		{
			for (int i = 0; i < currentSelectedPlayer.m_selectedEntities.size(); i++)
			{
				positionEntity(BATTLE_FLEET_WINDOW, BATTLE_FLEET_SLIDER, ENTITY + std::to_string(i), i, currentSelectedPlayer.m_selectedEntities.size());
				if (entityContainsMouse(BATTLE_FLEET_WINDOW, ENTITY + std::to_string(i), m_battleFleetWindowTopLeft, HAPISPACE::VectorI(mouseData.x, mouseData.y)))
				{
					for (int j = 0; j < currentSelectedPlayer.m_selectedEntities.size(); j++)
					{
						UI.GetWindow(BATTLE_FLEET_WINDOW)->DeleteObject(ENTITY + std::to_string(j));
					}
					currentSelectedPlayer.m_selectedEntities.erase(currentSelectedPlayer.m_selectedEntities.begin() + i);
					for (int j = 0; j < currentSelectedPlayer.m_selectedEntities.size(); j++)
					{

						if (!windowObjectExists(BATTLE_FLEET_WINDOW, ENTITY + std::to_string(j)))
						{
							UI.GetWindow(BATTLE_FLEET_WINDOW)->AddCanvas(ENTITY + std::to_string(j), calculateObjectWindowPosition(j), currentSelectedPlayer.m_selectedEntities[j]->m_sprite);
						}
					}
				}
			}
		}
		break;
	}
	}
}

void OverWorldGUI::onMouseMove(const HAPI_TMouseData& mouseData, Player& currentSelectedPlayer)
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
		if (HAPI_Wrapper::isTranslated(m_upgradesButton, mouseData, 0))
		{
			m_upgradesButton->SetFrameNumber(1);
		}
		else if (m_upgradesButton->GetFrameNumber() != 0)
		{
			m_upgradesButton->SetFrameNumber(0);
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
				for (int i = 0; i < currentSelectedPlayer.m_entities.size(); i++)
				{
					positionEntity(FLEET_WINDOW, FLEET_SLIDER, "entity" + std::to_string(i), i, currentSelectedPlayer.m_entities.size());
				}
			}
			if (windowScreenRect(BATTLE_FLEET_WINDOW).Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
			{
				for (int i = 0; i < currentSelectedPlayer.m_selectedEntities.size(); i++)
				{
					positionEntity(BATTLE_FLEET_WINDOW, BATTLE_FLEET_SLIDER, "entity" + std::to_string(i), i, currentSelectedPlayer.m_selectedEntities.size());
				}
			}
		}
		break;
	}
	case OverWorldWindow::eUpgrade:
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

void OverWorldGUI::reset(const std::vector<EntityProperties>& playerEntities)
{
	UI.DeleteWindow(FLEET_WINDOW);
	UI.DeleteWindow(BATTLE_FLEET_WINDOW);

	HAPI_Wrapper::setPosition(m_prebattleUIBackground, { 160, 90 });
	HAPI_Wrapper::setPosition(m_enemyTerritoryHexSheet, { 260, 690 });
	HAPI_Wrapper::setPosition(m_playButton, { 1310, 812 });
	HAPI_Wrapper::setPosition(m_backButton, { 345, 837 });
	HAPI_Wrapper::setPosition(m_upgradesButton, { 1460, 115 });
	//adding the windows and sliders, also populates the fleet window with all current entities
	UI.AddWindow(FLEET_WINDOW, HAPISPACE::RectangleI(220, 1050, 510, 710), fleetWindowSkinName);
	for (int i = 0; i < playerEntities.size(); i++)
	{
		UI.GetWindow(FLEET_WINDOW)->AddCanvas(ENTITY + std::to_string(i), calculateObjectWindowPosition(i), playerEntities[i].m_sprite);
	}
	UI.GetWindow(FLEET_WINDOW)->AddSlider(FLEET_SLIDER, HAPISPACE::RectangleI(0, 830, 160, 210), sliderLayout);
	UI.AddWindow(BATTLE_FLEET_WINDOW, HAPISPACE::RectangleI(220, 1050, 220, 420), fleetWindowSkinName);
	UI.GetWindow(BATTLE_FLEET_WINDOW)->AddSlider(BATTLE_FLEET_SLIDER, HAPISPACE::RectangleI(0, 830, 160, 210), sliderLayout);

	//upgrade buttons positions
	HAPI_Wrapper::setPosition(m_upgradesScreenBackground, { 345, 140 });
	HAPI_Wrapper::setPosition(m_upgradeBackButton, { 1350, 785 });
	HAPI_Wrapper::setPosition(m_removeHealthButton, { 785, 240 });//remove buttons
	HAPI_Wrapper::setPosition(m_removeMovementButton, { 785, 370 });
	HAPI_Wrapper::setPosition(m_removeDamageButton, { 785, 500 });
	HAPI_Wrapper::setPosition(m_removeRangeButton, { 785, 630 });
	HAPI_Wrapper::setPosition(m_addHealthButton, { 1035, 240 });//add buttons
	HAPI_Wrapper::setPosition(m_addMovementButton, { 1035, 370 });
	HAPI_Wrapper::setPosition(m_addDamageButton, { 1035, 500 });
	HAPI_Wrapper::setPosition(m_addRangeButton, { 1035, 630 });


	//UI.DeleteWindow(FLEET_WINDOW);
	//UI.DeleteWindow(BATTLE_FLEET_WINDOW);

	//UI.AddWindow(FLEET_WINDOW, HAPISPACE::RectangleI(220, 1050, 510, 710), fleetWindowSkinName);
	//for (int i = 0; i < currentSelectedPlayer.m_entities.size(); i++) TODO:
	//{
	//	UI.GetWindow(FLEET_WINDOW)->AddCanvas(ENTITY + std::to_string(i), calculateObjectWindowPosition(i), currentSelectedPlayer.m_entities[i].m_sprite);
	//}
	//UI.GetWindow(FLEET_WINDOW)->AddSlider(FLEET_SLIDER, HAPISPACE::RectangleI(0, 830, 160, 210), sliderLayout);
	//UI.AddWindow(BATTLE_FLEET_WINDOW, HAPISPACE::RectangleI(220, 1050, 220, 420), fleetWindowSkinName);
	//UI.GetWindow(BATTLE_FLEET_WINDOW)->AddSlider(BATTLE_FLEET_SLIDER, HAPISPACE::RectangleI(0, 830, 160, 210), sliderLayout);
}

void OverWorldGUI::clear()
{
	UI.DeleteWindow(FLEET_WINDOW);
	UI.DeleteWindow(BATTLE_FLEET_WINDOW);
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

void OverWorldGUI::onReset()
{
	CURRENT_WINDOW = OverWorldWindow::eLevelSelection;
}

bool OverWorldGUI::windowObjectExists(const std::string & windowName, const std::string& windowObjectName) const
{
	if (UI.GetWindow(windowName)->GetObject(windowObjectName) != nullptr)
	{
		return true;
	}
	return false;
}
#include "OverWorldGUI.h"
#include "Textures.h"
#include "OverWorld.h"
#include "Utilities/Utilities.h"
#include "GameEventMessenger.h"

OverWorldWindow OverWorldGUI::CURRENT_WINDOW = OverWorldWindow::ePlayerSelection;

constexpr int WINDOW_OBJECTWIDTH = 75;
constexpr int WINDOW_OBJECTHEIGHT = 150;
constexpr int WINDOW_WIDTH = 830;
constexpr int WINDOW_HEIGHT = 200;

void OverWorldGUI::setShipSelectionTrigger(bool trigger)
{
	shipSelectionTrigger = trigger;
}

std::string OverWorldGUI::getSelectedMap()
{
	return selectedMap;
}



OverWorldGUI::OverWorldGUI()
	: m_battleMapBackground(std::make_unique<Sprite>(Textures::m_levelSelectBackground)),
	m_selectMapButtons1(std::make_unique<Sprite>(Textures::m_levelSelectSheet)),
	m_selectMapButtons2(std::make_unique<Sprite>(Textures::m_levelSelectSheet)),
	m_selectMapButtons3(std::make_unique<Sprite>(Textures::m_levelSelectSheet)),
	m_prebattleUIBackground(std::make_unique<Sprite>(Textures::m_prebattleUIBackground)),
	m_playButton(std::make_unique<Sprite>(Textures::m_preBattleUIPlayButton)),
	m_backButton(std::make_unique<Sprite>(Textures::m_preBattleUIBackButton)),
	m_playerSelectYellow(std::make_unique<Sprite>(Textures::m_playerSelectIconYellow)),
	m_playerSelectGreen(std::make_unique<Sprite>(Textures::m_playerSelectIconGreen)),
	m_playerSelectRed(std::make_unique<Sprite>(Textures::m_playerSelectIconRed)),
	m_playerSelectBlue(std::make_unique<Sprite>(Textures::m_playerSelectIconBlue)),
	m_playerSelectBackground(std::make_unique<Sprite>(Textures::m_levelSelectBackground)),
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
		case OverWorldWindow::ePlayerSelection:
		{
			m_playerSelectBackground->Render(SCREEN_SURFACE);
			m_playerSelectYellow->Render(SCREEN_SURFACE);
			m_playerSelectGreen->Render(SCREEN_SURFACE);
			m_playerSelectRed->Render(SCREEN_SURFACE);
			m_playerSelectBlue->Render(SCREEN_SURFACE);
			m_playButton->Render(SCREEN_SURFACE);
			break;
		}
		case OverWorldWindow::eShipSelection:
		{
			m_prebattleUIBackground->Render(SCREEN_SURFACE);
			m_playButton->Render(SCREEN_SURFACE);
			m_backButton->Render(SCREEN_SURFACE);
			m_upgradesButton->Render(SCREEN_SURFACE);
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
			battle.render();
			break;
		}
		case OverWorldWindow::eLevelSelection:
		{

			//HAPI_Wrapper::render(m_battleMapBackground);
			HAPI_Wrapper::render(m_background);
			//HAPI_Wrapper::render(m_enemyTerritoryHexSheet);

			SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1000, 250), HAPISPACE::Colour255::BLACK, "Plunder Dogs", 105, {}, {}, 2.5f);
			
			HAPI_Wrapper::render(m_battleMapBackground);
			HAPI_Wrapper::render(m_backButton);
			HAPI_Wrapper::render(m_selectMapButtons1);
			HAPI_Wrapper::render(m_selectMapButtons2);
			HAPI_Wrapper::render(m_selectMapButtons3);

			break;
		}
		case OverWorldWindow::eUpgrade:
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

void OverWorldGUI::onLeftClick(const HAPI_TMouseData& mouseData, Player& currentSelectedPlayer, bool& selectNextPlayer, bool& resetPlayer)
{
	switch (CURRENT_WINDOW)
	{
		case OverWorldWindow::ePlayerSelection:
		{
			if (HAPI_Wrapper::isTranslated(m_playerSelectYellow, mouseData, 0))
			{
				m_playerSelectYellow->AdvanceToNextFrame();
			}
			if (HAPI_Wrapper::isTranslated(m_playerSelectGreen, mouseData, 0))
			{
				m_playerSelectGreen->AdvanceToNextFrame();
			}
			if (HAPI_Wrapper::isTranslated(m_playerSelectRed, mouseData, 0))
			{
				m_playerSelectRed->AdvanceToNextFrame();
			}
			if (HAPI_Wrapper::isTranslated(m_playerSelectBlue, mouseData, 0))
			{
				m_playerSelectBlue->AdvanceToNextFrame();
			}
			if (HAPI_Wrapper::isTranslated(m_playButton, mouseData, 0))
			{
				CURRENT_WINDOW = OverWorldWindow::eLevelSelection;
			}
			break;

		}
		case OverWorldWindow::eLevelSelection:
		{
			if (HAPI_Wrapper::isTranslated(m_backButton, mouseData, 0))
			{
			
			CURRENT_WINDOW = OverWorldWindow::ePlayerSelection;
			}

			if (HAPI_Wrapper::isTranslated(m_selectMapButtons1, mouseData, 0))
			{
				CURRENT_WINDOW = OverWorldWindow::eShipSelection;
				selectedMap = "Level1.tmx";
				UI.OpenWindow(FLEET_WINDOW);
				UI.OpenWindow(BATTLE_FLEET_WINDOW);// put different load map strings here
			}
			if (HAPI_Wrapper::isTranslated(m_selectMapButtons2, mouseData, 0))
			{
				CURRENT_WINDOW = OverWorldWindow::eShipSelection;
				selectedMap = "Level2.tmx";
				UI.OpenWindow(FLEET_WINDOW);
				UI.OpenWindow(BATTLE_FLEET_WINDOW);
			}
			if (HAPI_Wrapper::isTranslated(m_selectMapButtons3, mouseData, 0))
			{
				CURRENT_WINDOW = OverWorldWindow::eShipSelection;
				selectedMap = "Level3.tmx";
				UI.OpenWindow(FLEET_WINDOW);
				UI.OpenWindow(BATTLE_FLEET_WINDOW);
			}
			break;
		}
		//Play Button
		case OverWorldWindow::eShipSelection:
		{
			if (HAPI_Wrapper::isTranslated(m_playButton, mouseData, 0))
			{
				if (!currentSelectedPlayer.m_selectedEntities.empty())
				{
					//CURRENT_WINDOW = OverWorldWindow::eLevelSelection;
					selectNextPlayer = true;
					UI.CloseWindow(FLEET_WINDOW);
					UI.CloseWindow(BATTLE_FLEET_WINDOW);
					shipSelectionTrigger=  true ;
;					//reset here 
					return;
				}
			}

			if (HAPI_Wrapper::isTranslated(m_upgradesButton, mouseData, 0))
			{
				CURRENT_WINDOW = OverWorldWindow::eUpgrade;
				UI.CloseWindow(FLEET_WINDOW);
				UI.CloseWindow(BATTLE_FLEET_WINDOW);
			}


			else if (HAPI_Wrapper::isTranslated(m_backButton, mouseData, 0))
			{
				resetPlayer = true;
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
	case OverWorldWindow::eShipSelection:
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

	case OverWorldWindow::ePlayerSelection:
	{
		if (HAPI_Wrapper::isTranslated(m_playButton, mouseData, 0))
		{
			m_playButton->SetFrameNumber(1);
		}
		else if (m_playButton->GetFrameNumber() != 0)
		{
			m_playButton->SetFrameNumber(0);
		}
	}
	case OverWorldWindow::eLevelSelection:
	{
		if (HAPI_Wrapper::isTranslated(m_backButton, mouseData, 0))
		{
			m_backButton->SetFrameNumber(1);
		}
		else if (m_backButton->GetFrameNumber() != 0)
		{
			m_backButton->SetFrameNumber(0);
		}
		if (HAPI_Wrapper::isTranslated(m_selectMapButtons1, mouseData, 0))//checks if mouse is over button
		{
			m_selectMapButtons1->SetFrameNumber(3);//changes the buttons sprite to hover sprite
		}
		else if (m_selectMapButtons1->GetFrameNumber() != 0)//if mouse is not over the button and the button has the hover sprite
		{
			m_selectMapButtons1->SetFrameNumber(0);// sets it to the default sprite
		}
		if (HAPI_Wrapper::isTranslated(m_selectMapButtons2, mouseData, 0))//checks if mouse is over button
		{
			m_selectMapButtons2->SetFrameNumber(4);//changes the buttons sprite to hover sprite
		}
		else if (m_selectMapButtons2->GetFrameNumber() != 0)//if mouse is not over the button and the button has the hover sprite
		{
			m_selectMapButtons2->SetFrameNumber(1);// sets it to the default sprite
		}
		if (HAPI_Wrapper::isTranslated(m_selectMapButtons3, mouseData, 0))//checks if mouse is over button
		{
			m_selectMapButtons3->SetFrameNumber(5);//changes the buttons sprite to hover sprite
		}
		else if (m_selectMapButtons3->GetFrameNumber() != 0)//if mouse is not over the button and the button has the hover sprite
		{
			m_selectMapButtons3->SetFrameNumber(2);// sets it to the default sprite
		}

		break;
	}
	case OverWorldWindow::eShipSelection:
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

		if (HAPI_Wrapper::isTranslated(m_upgradesButton, mouseData, 0))
		{
			m_upgradesButton->SetFrameNumber(1);
		}
		else if (m_upgradesButton->GetFrameNumber() != 0)
		{
			m_upgradesButton->SetFrameNumber(0);
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
	m_selectMapButtons1->SetFrameNumber(0);
	m_selectMapButtons2->SetFrameNumber(1);
	m_selectMapButtons3->SetFrameNumber(2);

	HAPI_Wrapper::setPosition(m_selectMapButtons1, { 300, 300 });
	HAPI_Wrapper::setPosition(m_selectMapButtons2, { 500, 300 });
	HAPI_Wrapper::setPosition(m_selectMapButtons3, { 700, 300 });

	HAPI_Wrapper::setPosition(m_playerSelectYellow, { 300, 300 });
	HAPI_Wrapper::setPosition(m_playerSelectGreen, { 600, 300 });
	HAPI_Wrapper::setPosition(m_playerSelectRed, { 900, 300 });
	HAPI_Wrapper::setPosition(m_playerSelectBlue, { 1200, 300 });

	HAPI_Wrapper::setPosition(m_playButton, { 1150, 722 });
	HAPI_Wrapper::setPosition(m_backButton, { 185, 747 });
	HAPI_Wrapper::setPosition(m_upgradesButton, { 1300, 25 });
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
	HAPI_Wrapper::setPosition(m_upgradesScreenBackground, { 185, 50 });
	HAPI_Wrapper::setPosition(m_upgradeBackButton, { 1190, 785 });
	HAPI_Wrapper::setPosition(m_removeHealthButton, { 625, 150 });//remove buttons
	HAPI_Wrapper::setPosition(m_removeMovementButton, { 625, 280 });
	HAPI_Wrapper::setPosition(m_removeDamageButton, { 625, 410 });
	HAPI_Wrapper::setPosition(m_removeRangeButton, { 625, 540 });
	HAPI_Wrapper::setPosition(m_addHealthButton, { 875, 150 });//add buttons
	HAPI_Wrapper::setPosition(m_addMovementButton, { 875, 280 });
	HAPI_Wrapper::setPosition(m_addDamageButton, { 875, 410 });
	HAPI_Wrapper::setPosition(m_addRangeButton, { 875, 540 });


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
	
	if (shipSelectionTrigger)
	{
		UI.OpenWindow(FLEET_WINDOW);
		UI.OpenWindow(BATTLE_FLEET_WINDOW);
	}
	
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
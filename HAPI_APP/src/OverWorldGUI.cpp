#include "OverWorldGUI.h"
#include "Textures.h"
#include "OverWorld.h"
#include "Utilities/Utilities.h"
#include "GameEventMessenger.h"

OverWorldWindow OverWorldGUI::CURRENT_WINDOW = OverWorldWindow::eMainMenu;

constexpr int WINDOW_OBJECTWIDTH = 75;
constexpr int WINDOW_OBJECTHEIGHT = 150;
constexpr int WINDOW_WIDTH = 830;
constexpr int WINDOW_HEIGHT = 200;
constexpr int UPGRADE_WINDOW_OBJECTWIDTH = 150;
constexpr int UPGRADE_WINDOW_OBJECTHEIGHT = 300;
constexpr int UPGRADE_WINDOW_WIDTH = 200;
constexpr int UPGRADE_WINDOW_HEIGHT = 600;

int OverWorldGUI::getActivePlayerCount()
{
	int playerCount = 0;

	if (m_playerSelectYellow->GetFrameNumber() == ePlayerSelect::eHuman)
	{
		playerCount++;
	}
	if (m_playerSelectGreen->GetFrameNumber() == ePlayerSelect::eHuman)
	{
		playerCount++;
	}
	if (m_playerSelectRed->GetFrameNumber() == ePlayerSelect::eHuman)
	{
		playerCount++;
	}
	if (m_playerSelectBlue->GetFrameNumber() == ePlayerSelect::eHuman)
	{
		playerCount++;
	}

	return playerCount;
}

void OverWorldGUI::setActivePlayers(std::vector<Player>& players)
{
	players.clear();
	if (m_playerSelectYellow->GetFrameNumber() == ePlayerSelect::eHuman)
	{
		players.emplace_back(FactionName::eYellow);
	}
	if (m_playerSelectGreen->GetFrameNumber() == ePlayerSelect::eHuman)
	{
		players.emplace_back(FactionName::eGreen);
	}
	if (m_playerSelectRed->GetFrameNumber() == ePlayerSelect::eHuman)
	{
		players.emplace_back(FactionName::eRed);
	}
	if (m_playerSelectBlue->GetFrameNumber() == ePlayerSelect::eHuman)
	{
		players.emplace_back(FactionName::eBlue);
	}
}

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
	m_selectMapButtons1(std::make_unique<Sprite>(Textures::m_levelSelect1)),
	m_selectMapButtons2(std::make_unique<Sprite>(Textures::m_levelSelect2)),
	m_selectMapButtons3(std::make_unique<Sprite>(Textures::m_levelSelect3)),
	m_selectMapButtons4(std::make_unique<Sprite>(Textures::m_levelSelect4)),
	m_selectMapButtons5(std::make_unique<Sprite>(Textures::m_levelSelect5)),
	m_prebattleUIBackground(std::make_unique<Sprite>(Textures::m_prebattleUIBackground)),
	m_playButton(std::make_unique<Sprite>(Textures::m_preBattleUIPlayButton)),
	m_backButton(std::make_unique<Sprite>(Textures::m_preBattleUIBackButton)),
	m_done(std::make_unique<Sprite>(Textures::m_doneButton)),
	m_quitButton(std::make_unique<Sprite>(Textures::m_quitButton)),
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
		case OverWorldWindow::eMainMenu:
		{
			
			m_background->Render(SCREEN_SURFACE);
			m_playButton->Render(SCREEN_SURFACE);
			m_quitButton->Render(SCREEN_SURFACE);
			HAPI_Sprites.ChangeFontFromFile("data/RAPSCALL.TTF");
			SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1380, 50), HAPISPACE::Colour255::YELLOW, "Plunder\n Dogs", 190, {}, HAPISPACE::Colour255::BLACK, 2.5f);
			
			break;
		}
	

		case OverWorldWindow::ePlayerSelection:
		{
			m_playerSelectBackground->Render(SCREEN_SURFACE);
			m_playerSelectYellow->Render(SCREEN_SURFACE);
			m_playerSelectGreen->Render(SCREEN_SURFACE);
			m_playerSelectRed->Render(SCREEN_SURFACE);
			m_playerSelectBlue->Render(SCREEN_SURFACE);
			m_backButton->Render(SCREEN_SURFACE);
			m_done->Render(SCREEN_SURFACE);
			break;
		}
		case OverWorldWindow::eShipSelection:
		{
			m_prebattleUIBackground->Render(SCREEN_SURFACE);
			m_done->Render(SCREEN_SURFACE);
			m_backButton->Render(SCREEN_SURFACE);
			m_upgradesButton->Render(SCREEN_SURFACE);
			
			SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(550, 100), HAPISPACE::Colour255::BLACK, selectedMapName, 100);
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
			//HAPI_Wrapper::render(m_enemyTerritoryHexSheet);

			HAPI_Wrapper::render(m_battleMapBackground);
			HAPI_Wrapper::render(m_backButton);
			HAPI_Wrapper::render(m_selectMapButtons1);
			HAPI_Wrapper::render(m_selectMapButtons2);
			HAPI_Wrapper::render(m_selectMapButtons3);
			HAPI_Wrapper::render(m_selectMapButtons4);
			HAPI_Wrapper::render(m_selectMapButtons5);

			break;
		}
		case OverWorldWindow::eUpgrade:
		{
			m_upgradesScreenBackground->Render(SCREEN_SURFACE);

			SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1170, 150), HAPISPACE::Colour255::BLACK, "36", 50);//draw text gold
			SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1110, 270), HAPISPACE::Colour255::BLACK, "BUY", 50);
			SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1110, 315), HAPISPACE::Colour255::BLACK, "SHIPS", 50);
			if (m_currentlySelected != nullptr)
			{
				SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(710, 150), HAPISPACE::Colour255::BLACK, std::to_string(m_currentlySelected->m_currentHealth), 50);
				SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(710, 280), HAPISPACE::Colour255::BLACK, std::to_string(m_currentlySelected->m_damage), 50);
				SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(710, 410), HAPISPACE::Colour255::BLACK, std::to_string(m_currentlySelected->m_movementPoints), 50);
				SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(710, 540), HAPISPACE::Colour255::BLACK, std::to_string(m_currentlySelected->m_range), 50);
				SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(640, 670), HAPISPACE::Colour255::BLACK, "UPGRADES: " + std::to_string(m_currentlySelected->m_upgradePoints), 50);
			}
			else
			{
				SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(640, 670), HAPISPACE::Colour255::BLACK, "UPGRADES: ", 50);
			}
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

		case OverWorldWindow::eMainMenu:
		{
			if (HAPI_Wrapper::isTranslated(m_playButton, mouseData, 0))
			{
				CURRENT_WINDOW = OverWorldWindow::ePlayerSelection;
			}
			if (HAPI_Wrapper::isTranslated(m_quitButton, mouseData, 0))
			{
				HAPI_Sprites.Close();
			}
			break;
		}
		
		case OverWorldWindow::ePlayerSelection:
		{
			if (HAPI_Wrapper::isTranslated(m_playerSelectYellow, mouseData, 0))
			{
			//	m_playerSelectYellow->AdvanceToNextFrame();
				if (m_playerSelectYellow->GetFrameNumber() == ePlayerSelect::eHuman)
				{
					m_playerSelectYellow->SetFrameNumber(ePlayerSelect::eNone);
				}
				else
				{
					m_playerSelectYellow->SetFrameNumber(ePlayerSelect::eHuman);
				}
			}
			if (HAPI_Wrapper::isTranslated(m_playerSelectGreen, mouseData, 0))
			{
				//m_playerSelectGreen->AdvanceToNextFrame();
				if (m_playerSelectGreen->GetFrameNumber() == ePlayerSelect::eHuman)
				{
					m_playerSelectGreen->SetFrameNumber(ePlayerSelect::eNone);
				}
				else
				{
					m_playerSelectGreen->SetFrameNumber(ePlayerSelect::eHuman);
				}
			}


			if (HAPI_Wrapper::isTranslated(m_playerSelectRed, mouseData, 0))
			{
				//m_playerSelectRed->AdvanceToNextFrame();
				if (m_playerSelectRed->GetFrameNumber() == ePlayerSelect::eHuman)
				{
					m_playerSelectRed->SetFrameNumber(ePlayerSelect::eNone);
				}
				else
				{
					m_playerSelectRed->SetFrameNumber(ePlayerSelect::eHuman);
				}
			}
			if (HAPI_Wrapper::isTranslated(m_playerSelectBlue, mouseData, 0))
			{
				//m_playerSelectBlue->AdvanceToNextFrame();
				if (m_playerSelectBlue->GetFrameNumber() == ePlayerSelect::eHuman)
				{
					m_playerSelectBlue->SetFrameNumber(ePlayerSelect::eNone);
				}
				else
				{
					m_playerSelectBlue->SetFrameNumber(ePlayerSelect::eHuman);
				}
			}
			if (HAPI_Wrapper::isTranslated(m_backButton, mouseData, 0))
			{
				CURRENT_WINDOW = OverWorldWindow::eMainMenu;
			}
			if (HAPI_Wrapper::isTranslated(m_done, mouseData, 0))
			{

				// put int function
				if (getActivePlayerCount() >=2)
				{
					CURRENT_WINDOW = OverWorldWindow::eLevelSelection;
				}
				
		
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
				selectedMapName = "Isle of Turtles";
				UI.OpenWindow(FLEET_WINDOW);
				UI.OpenWindow(BATTLE_FLEET_WINDOW);
			}
			if (HAPI_Wrapper::isTranslated(m_selectMapButtons2, mouseData, 0))
			{
				CURRENT_WINDOW = OverWorldWindow::eShipSelection;
				selectedMap = "Level2.tmx";
				selectedMapName = "Frozen Depths";
				UI.OpenWindow(FLEET_WINDOW);
				UI.OpenWindow(BATTLE_FLEET_WINDOW);
			}

			if (HAPI_Wrapper::isTranslated(m_selectMapButtons3, mouseData, 0))
			{
				CURRENT_WINDOW = OverWorldWindow::eShipSelection;
				selectedMap = "Level3.tmx";
				selectedMapName = "Sun Sand Cay";
				UI.OpenWindow(FLEET_WINDOW);
				UI.OpenWindow(BATTLE_FLEET_WINDOW);
			}

			if (HAPI_Wrapper::isTranslated(m_selectMapButtons4, mouseData, 0))
			{
				CURRENT_WINDOW = OverWorldWindow::eShipSelection;
				selectedMap = "Level5.tmx";
				selectedMapName = "Misty Mountains";
				UI.OpenWindow(FLEET_WINDOW);
				UI.OpenWindow(BATTLE_FLEET_WINDOW);
			}

			if (HAPI_Wrapper::isTranslated(m_selectMapButtons5, mouseData, 0))
			{
				CURRENT_WINDOW = OverWorldWindow::eShipSelection;
				selectedMap = "Level6.tmx";
				selectedMapName = "Krakens Den";
				UI.OpenWindow(FLEET_WINDOW);
				UI.OpenWindow(BATTLE_FLEET_WINDOW);
			}
			
			break;
		}
		//Play Button
		case OverWorldWindow::eShipSelection:
		{
			if (HAPI_Wrapper::isTranslated(m_upgradesButton, mouseData, 0))
			{
				UI.CloseWindow(FLEET_WINDOW);
				UI.CloseWindow(BATTLE_FLEET_WINDOW);
				CURRENT_WINDOW = OverWorldWindow::eUpgrade;
				UI.OpenWindow(UPGRADE_FLEET_WINDOW);
			}
			if (HAPI_Wrapper::isTranslated(m_done, mouseData, 0))
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
				CURRENT_WINDOW = OverWorldWindow::ePlayerSelection;
				UI.CloseWindow(FLEET_WINDOW);
				UI.CloseWindow(BATTLE_FLEET_WINDOW);
			}
			selectBattleShip(FLEET_WINDOW, FLEET_SLIDER, BATTLE_FLEET_WINDOW, BATTLE_FLEET_SLIDER, HAPISPACE::VectorI(mouseData.x, mouseData.y), m_fleetWindowTopLeft, m_battleFleetWindowTopLeft, currentSelectedPlayer.m_entities, currentSelectedPlayer.m_selectedEntities);
			deselectBattleShip(BATTLE_FLEET_WINDOW, BATTLE_FLEET_SLIDER, m_battleFleetWindowTopLeft, currentSelectedPlayer.m_selectedEntities, HAPISPACE::VectorI(mouseData.x, mouseData.y));
			updateSelectedShips(FLEET_WINDOW, m_fleetWindowTopLeft, currentSelectedPlayer.m_entities, currentSelectedPlayer.m_selectedEntities);
			break;
		}
		case OverWorldWindow::eUpgrade:
		{
			if (m_upgradeBackButton->GetSpritesheet()->GetFrameRect(0).Translated(m_upgradeBackButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				CURRENT_WINDOW = OverWorldWindow::eLevelSelection;
				UI.CloseWindow(UPGRADE_FLEET_WINDOW);
			}
			else if (m_addHealthButton->GetSpritesheet()->GetFrameRect(0).Translated(m_addHealthButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				//health+
				if (m_currentlySelected != nullptr)
				{
					if (m_currentlySelected->m_upgradePoints > 0)
					{
						m_currentlySelected->m_upgradePoints--;
						m_currentlySelected->m_healthMax++;
						m_currentlySelected->m_currentHealth++;
					}
				}
			}
			else if (m_addMovementButton->GetSpritesheet()->GetFrameRect(0).Translated(m_addMovementButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				//movement+
				if (m_currentlySelected != nullptr)
				{
					if (m_currentlySelected->m_upgradePoints > 0)
					{
						m_currentlySelected->m_upgradePoints--;
						//m_currentlySelected->m_movementPoints++;
						m_currentlySelected->m_damage++;
					}
				}
			}
			else if (m_addDamageButton->GetSpritesheet()->GetFrameRect(0).Translated(m_addDamageButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				//damage+
				if (m_currentlySelected != nullptr)
				{
					if (m_currentlySelected->m_upgradePoints > 0)
					{
						m_currentlySelected->m_upgradePoints--;
						//m_currentlySelected->m_damage++;
						m_currentlySelected->m_movementPoints++;
					}
				}
			}
			else if (m_addRangeButton->GetSpritesheet()->GetFrameRect(0).Translated(m_addRangeButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				//range+
				if (m_currentlySelected != nullptr)
				{
					if (m_currentlySelected->m_upgradePoints > 0)
					{
						m_currentlySelected->m_upgradePoints--;
						m_currentlySelected->m_range++;
					}
				}
			}
			else if (m_removeHealthButton->GetSpritesheet()->GetFrameRect(0).Translated(m_removeHealthButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				//health-
				if (m_currentlySelected != nullptr)
				{
					if (m_currentlySelected->m_upgradePoints < m_currentlySelected->m_maxUpgradePoints)
					{
						m_currentlySelected->m_currentHealth--;
						m_currentlySelected->m_healthMax--;
						m_currentlySelected->m_upgradePoints++;
					}
				}
			}
			else if (m_removeMovementButton->GetSpritesheet()->GetFrameRect(0).Translated(m_removeMovementButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				//movement-
				if (m_currentlySelected != nullptr)
				{
					if (m_currentlySelected->m_upgradePoints < m_currentlySelected->m_maxUpgradePoints)
					{
						//m_currentlySelected->m_movementPoints--;
						m_currentlySelected->m_damage--;
						m_currentlySelected->m_upgradePoints++;
					}
				}
			}
			else if (m_removeDamageButton->GetSpritesheet()->GetFrameRect(0).Translated(m_removeDamageButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				//damage-
				if (m_currentlySelected != nullptr)
				{
					if (m_currentlySelected->m_upgradePoints < m_currentlySelected->m_maxUpgradePoints)
					{
						//m_currentlySelected->m_damage--;
						m_currentlySelected->m_movementPoints--;
						m_currentlySelected->m_upgradePoints++;
					}
				}
			}
			else if (m_removeRangeButton->GetSpritesheet()->GetFrameRect(0).Translated(m_removeRangeButton->GetTransformComp().GetPosition()).Contains(HAPISPACE::RectangleI(mouseData.x, mouseData.x, mouseData.y, mouseData.y)))
			{
				//range-
				if (m_currentlySelected != nullptr)
				{
					if (m_currentlySelected->m_upgradePoints < m_currentlySelected->m_maxUpgradePoints)
					{
						m_currentlySelected->m_range--;
						m_currentlySelected->m_upgradePoints++;
					}
				}
			}
			bool selection = false;
			checkShipSelect(selection, UPGRADE_FLEET_WINDOW, UPGRADE_FLEET_SCROLLBAR, HAPISPACE::VectorI(mouseData.x, mouseData.y), m_upgradeFleetWindowTopLeft, currentSelectedPlayer.m_entities, true);
			if (selection = false)
			{
				m_currentlySelected = nullptr;
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
		break;
	}
	}
}

void OverWorldGUI::onMouseMove(const HAPI_TMouseData& mouseData, Player& currentSelectedPlayer)
{
	switch (CURRENT_WINDOW)
	{
	case OverWorldWindow::eMainMenu:
	{
		if (HAPI_Wrapper::isTranslated(m_playButton, mouseData, 0))
		{
			m_playButton->SetFrameNumber(1);
		}
		else if (m_playButton->GetFrameNumber() != 0)
		{
			m_playButton->SetFrameNumber(0);
		}

		if (HAPI_Wrapper::isTranslated(m_quitButton, mouseData, 0))
		{
			m_quitButton->SetFrameNumber(1);
		}
		else if (m_quitButton->GetFrameNumber() != 0)
		{
			m_quitButton->SetFrameNumber(0);
		}
	}
	case OverWorldWindow::ePlayerSelection:
	{
		if (HAPI_Wrapper::isTranslated(m_done, mouseData, 0))
		{
			m_done->SetFrameNumber(1);
		}
		else if (m_done->GetFrameNumber() != 0)
		{
			m_done->SetFrameNumber(0);
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
			m_selectMapButtons1->SetFrameNumber(1);//changes the buttons sprite to hover sprite
		}
		else if (m_selectMapButtons1->GetFrameNumber() != 0)//if mouse is not over the button and the button has the hover sprite
		{
			m_selectMapButtons1->SetFrameNumber(0);// sets it to the default sprite
		}
		if (HAPI_Wrapper::isTranslated(m_selectMapButtons2, mouseData, 0))//checks if mouse is over button
		{
			m_selectMapButtons2->SetFrameNumber(1);//changes the buttons sprite to hover sprite
		}
		else if (m_selectMapButtons2->GetFrameNumber() != 0)//if mouse is not over the button and the button has the hover sprite
		{
			m_selectMapButtons2->SetFrameNumber(0);// sets it to the default sprite
		}
		if (HAPI_Wrapper::isTranslated(m_selectMapButtons3, mouseData, 0))//checks if mouse is over button
		{
			m_selectMapButtons3->SetFrameNumber(1);//changes the buttons sprite to hover sprite
		}
		else if (m_selectMapButtons3->GetFrameNumber() != 0)//if mouse is not over the button and the button has the hover sprite
		{
			m_selectMapButtons3->SetFrameNumber(0);// sets it to the default sprite
		}
		if (HAPI_Wrapper::isTranslated(m_selectMapButtons4, mouseData, 0))//checks if mouse is over button
		{
			m_selectMapButtons4->SetFrameNumber(1);//changes the buttons sprite to hover sprite
		}
		else if (m_selectMapButtons4->GetFrameNumber() != 0)//if mouse is not over the button and the button has the hover sprite
		{
			m_selectMapButtons4->SetFrameNumber(0);// sets it to the default sprite
		}
		if (HAPI_Wrapper::isTranslated(m_selectMapButtons5, mouseData, 0))//checks if mouse is over button
		{
			m_selectMapButtons5->SetFrameNumber(1);//changes the buttons sprite to hover sprite
		}
		else if (m_selectMapButtons5->GetFrameNumber() != 0)//if mouse is not over the button and the button has the hover sprite
		{
			m_selectMapButtons5->SetFrameNumber(0);// sets it to the default sprite
		}

		break;
	}
	case OverWorldWindow::eShipSelection:
	{
		if (HAPI_Wrapper::isTranslated(m_done, mouseData, 0))
		{
			m_done->SetFrameNumber(1);
		}
		else if (m_done->GetFrameNumber() != 0)
		{
			m_done->SetFrameNumber(0);
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
		bool selection{ false };
		checkShipSelect(selection, FLEET_WINDOW, FLEET_SLIDER, HAPISPACE::VectorI(mouseData.x, mouseData.y), m_fleetWindowTopLeft, currentSelectedPlayer.m_entities);
		checkShipSelect(selection, BATTLE_FLEET_WINDOW, BATTLE_FLEET_SLIDER, HAPISPACE::VectorI(mouseData.x, mouseData.y), m_battleFleetWindowTopLeft, currentSelectedPlayer.m_selectedEntities);
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
		if (mouseData.leftButtonDown)
		{
			if (windowScreenRect(UPGRADE_FLEET_WINDOW).Contains(HAPISPACE::VectorI(mouseData.x, mouseData.y)))
			{
				for (int i = 0; i < currentSelectedPlayer.m_entities.size(); i++)
				{
					positionUpgradeEntity(UPGRADE_FLEET_WINDOW, UPGRADE_FLEET_SCROLLBAR, "entity" + std::to_string(i), i, currentSelectedPlayer.m_entities.size());
				}
			}
		}
		break;
	}
	}
}

void OverWorldGUI::reset(const std::vector<EntityProperties>& playerEntities)
{
	m_currentShips = 0;
	UI.DeleteWindow(FLEET_WINDOW);
	UI.DeleteWindow(BATTLE_FLEET_WINDOW);
	UI.DeleteWindow(UPGRADE_FLEET_WINDOW);

	
	HAPI_Wrapper::setPosition(m_playButton, { 1310, 812 });
	HAPI_Wrapper::setPosition(m_done, { 1310, 812 });
	HAPI_Wrapper::setPosition(m_quitButton, { 1600, 812 });
	HAPI_Wrapper::setPosition(m_backButton, { 345, 837 });
	HAPI_Wrapper::setPosition(m_upgradesButton, { 1460, 115 });
	

	HAPI_Wrapper::setPosition(m_selectMapButtons1, { 1300,100});
	HAPI_Wrapper::setPosition(m_selectMapButtons2, { 90, 48 });
	HAPI_Wrapper::setPosition(m_selectMapButtons3, { 1470, 584 });
	HAPI_Wrapper::setPosition(m_selectMapButtons4, { 570, 370 });
	HAPI_Wrapper::setPosition(m_selectMapButtons5, { 904, 720 });

	HAPI_Wrapper::setPosition(m_playerSelectYellow, { 300, 300 });
	HAPI_Wrapper::setPosition(m_playerSelectGreen, { 600, 300 });
	HAPI_Wrapper::setPosition(m_playerSelectRed, { 900, 300 });
	HAPI_Wrapper::setPosition(m_playerSelectBlue, { 1200, 300 });

	
	//adding the windows and sliders, also populates the fleet window with all current entities
	UI.AddWindow(FLEET_WINDOW, HAPISPACE::RectangleI(m_fleetWindowTopLeft.x, m_fleetWindowTopLeft.x + WINDOW_WIDTH, m_fleetWindowTopLeft.y, m_fleetWindowTopLeft.y + WINDOW_HEIGHT), fleetWindowSkinName);
	for (int i = 0; i < playerEntities.size(); i++)
	{
		UI.GetWindow(FLEET_WINDOW)->AddCanvas(ENTITY + std::to_string(i), calculateObjectWindowPosition(i), playerEntities[i].m_sprite);
	}

	UI.GetWindow(FLEET_WINDOW)->AddSlider(FLEET_SLIDER, HAPISPACE::RectangleI(0, WINDOW_WIDTH, 160, WINDOW_HEIGHT), sliderLayout);
	UI.AddWindow(BATTLE_FLEET_WINDOW, HAPISPACE::RectangleI(m_battleFleetWindowTopLeft.x, m_battleFleetWindowTopLeft.x + WINDOW_WIDTH, m_battleFleetWindowTopLeft.y, m_battleFleetWindowTopLeft.y + WINDOW_HEIGHT), fleetWindowSkinName);
	UI.GetWindow(BATTLE_FLEET_WINDOW)->AddSlider(BATTLE_FLEET_SLIDER, HAPISPACE::RectangleI(0, WINDOW_WIDTH, 160, WINDOW_HEIGHT), sliderLayout);
	UI.AddWindow(UPGRADE_FLEET_WINDOW, HAPISPACE::RectangleI(m_upgradeFleetWindowTopLeft.x, m_upgradeFleetWindowTopLeft.x + UPGRADE_WINDOW_WIDTH, m_upgradeFleetWindowTopLeft.y, m_upgradeFleetWindowTopLeft.y + UPGRADE_WINDOW_HEIGHT));
	for (int i = 0; i < playerEntities.size(); i++)
	{
		UI.GetWindow(UPGRADE_FLEET_WINDOW)->AddCanvas(ENTITY + std::to_string(i), calculateUpgradeObjectWindowPosition(i), playerEntities[i].m_sprite);
	}
	UI.GetWindow(UPGRADE_FLEET_WINDOW)->AddScrollbar(UPGRADE_FLEET_SCROLLBAR);
	UI.GetWindow(UPGRADE_FLEET_WINDOW)->GetObject(UPGRADE_FLEET_SCROLLBAR)->SetValue(1.0f);

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

void OverWorldGUI::positionUpgradeEntity(const std::string & windowName, const std::string & windowScrollbarName, const std::string & windowObjectName, int objectNumber, size_t vectorSize)
{
	UI.GetWindow(windowName)->PositionObject(windowObjectName, calculateUpgradeObjectScrolledPosition(windowName, windowScrollbarName, objectNumber, vectorSize));
}

float OverWorldGUI::getWindowSliderValue(const std::string & windowName, const std::string & windowSliderName) const
{
	return UI.GetWindow(windowName)->GetObject(windowSliderName)->GetValue();
}

HAPISPACE::RectangleI OverWorldGUI::calculateObjectWindowPosition(int objectNumber) const
{
	return HAPISPACE::RectangleI(WINDOW_OBJECTWIDTH * objectNumber, (WINDOW_OBJECTWIDTH * objectNumber) + WINDOW_OBJECTWIDTH, 0, WINDOW_OBJECTHEIGHT);
}

HAPISPACE::RectangleI OverWorldGUI::calculateUpgradeObjectWindowPosition(int objectNumber) const
{
	return HAPISPACE::RectangleI(0, UPGRADE_WINDOW_OBJECTWIDTH, UPGRADE_WINDOW_OBJECTHEIGHT * objectNumber, (UPGRADE_WINDOW_OBJECTHEIGHT * objectNumber) + UPGRADE_WINDOW_OBJECTHEIGHT);
}

HAPISPACE::RectangleI OverWorldGUI::windowScreenRect(const std::string & windowName) const
{
	return UI.GetWindow(windowName)->GetScreenRect();
}

HAPISPACE::VectorI OverWorldGUI::calculateObjectScrolledPosition(const std::string & windowName, const std::string & windowSliderName, int objectNumber, size_t vectorSize)
{
	return HAPISPACE::VectorI((WINDOW_OBJECTWIDTH * objectNumber) - ((WINDOW_OBJECTWIDTH * vectorSize) * getWindowSliderValue(windowName, windowSliderName)), 0);
}

HAPISPACE::VectorI OverWorldGUI::calculateUpgradeObjectScrolledPosition(const std::string & windowName, const std::string & windowSliderName, int objectNumber, size_t vectorSize)
{
	return HAPISPACE::VectorI(0, (UPGRADE_WINDOW_OBJECTHEIGHT * objectNumber) - ((UPGRADE_WINDOW_OBJECTHEIGHT * vectorSize) * (1.f - getWindowSliderValue(windowName, windowSliderName))));
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
	CURRENT_WINDOW = OverWorldWindow::eMainMenu;
}


bool OverWorldGUI::windowObjectExists(const std::string & windowName, const std::string& windowObjectName) const
{
	if (UI.GetWindow(windowName)->GetObject(windowObjectName) != nullptr)
	{
		return true;
	}
	return false;
}

void OverWorldGUI::checkShipSelect(bool & selection, const std::string & shipWindow, const std::string& windowSlider, const HAPISPACE::VectorI & mouseData, const HAPISPACE::VectorI & windowTopLeft, std::vector<EntityProperties>& entities, const bool vertical)
{
	for (int i = 0; i < entities.size(); i++)
	{
		if (vertical == true)
		{
			positionUpgradeEntity(shipWindow, windowSlider, ENTITY + std::to_string(i), i, entities.size());
		}
		else
		{
			positionEntity(shipWindow, windowSlider, ENTITY + std::to_string(i), i, entities.size());
		}
		if (entityContainsMouse(shipWindow, ENTITY + std::to_string(i), windowTopLeft, mouseData))
		{
			m_currentlySelected = &entities[i];
			selection = true;
		}
	}
}

void OverWorldGUI::checkShipSelect(bool & selection, const std::string & shipWindow, const std::string& windowSlider, const HAPISPACE::VectorI & mouseData, const HAPISPACE::VectorI & windowTopLeft, std::vector<EntityProperties*>& entities, const bool vertical)
{
	for (int i = 0; i < entities.size(); i++)
	{
		if (vertical == true)
		{
			positionUpgradeEntity(shipWindow, windowSlider, ENTITY + std::to_string(i), i, entities.size());
		}
		else
		{
			positionEntity(shipWindow, windowSlider, ENTITY + std::to_string(i), i, entities.size());
		}
		if (entityContainsMouse(shipWindow, ENTITY + std::to_string(i), windowTopLeft, mouseData))
		{
			m_currentlySelected = entities[i];
			selection = true;
		}
	}
}

void OverWorldGUI::selectBattleShip(const std::string & shipWindow, const std::string & windowSlider, const std::string & selectedShipWindow, const std::string & selectedWindowSlider, const HAPISPACE::VectorI & mouseData, const HAPISPACE::VectorI & windowTopLeft, const HAPISPACE::VectorI & selectedTopLeft, std::vector<EntityProperties>& entities, std::vector<EntityProperties*>& selectedEntities)
{
	if (m_currentShips < m_maxShips)
	{
		if (windowScreenRect(shipWindow).Contains(mouseData))
		{
			for (int i = 0; i < entities.size(); i++)
			{
				positionEntity(shipWindow, windowSlider, (ENTITY + std::to_string(i)), i, entities.size());
				if (entityContainsMouse(shipWindow, ENTITY + std::to_string(i), windowTopLeft, mouseData))
				{
					bool isSelected{ false };
					for (int j = 0; j < selectedEntities.size(); j++)
					{
						if (selectedEntities[j] == &entities[i])
						{
							isSelected = true;
						}
					}
					if (!isSelected)
					{
						m_currentShips++;
						selectedEntities.push_back(&entities[i]);
						for (int j = 0; j < selectedEntities.size(); j++)
						{
							if (!windowObjectExists(selectedShipWindow, ENTITY + std::to_string(j)))
							{
								UI.GetWindow(selectedShipWindow)->AddCanvas(ENTITY + std::to_string(j), calculateObjectWindowPosition(j), selectedEntities[j]->m_sprite);
							}
						}
					}
				}
			}
		}
	}
}

void OverWorldGUI::deselectBattleShip(const std::string & selectedShipWindow, const std::string & selectedShipSlider, const HAPISPACE::VectorI& selectedWindowTopLeft, std::vector<EntityProperties*>& selectedEntities, const HAPISPACE::VectorI & mouseData)
{
	for (int i = 0; i < selectedEntities.size(); i++)
	{
		positionEntity(selectedShipWindow, selectedShipSlider, ENTITY + std::to_string(i), i, selectedEntities.size());
		if (entityContainsMouse(selectedShipWindow, ENTITY + std::to_string(i), selectedWindowTopLeft, mouseData))
		{
			for (int j = 0; j < selectedEntities.size(); j++)
			{
				UI.GetWindow(selectedShipWindow)->DeleteObject(ENTITY + std::to_string(j));
			}
			m_currentShips--;
			selectedEntities.erase(selectedEntities.begin() + i);
			for (int j = 0; j < selectedEntities.size(); j++)
			{
				if (!windowObjectExists(selectedShipWindow, ENTITY + std::to_string(j)))
				{
					UI.GetWindow(selectedShipWindow)->AddCanvas(ENTITY + std::to_string(j), calculateObjectWindowPosition(j), selectedEntities[j]->m_sprite);
				}
			}
		}
	}
}

void OverWorldGUI::updateSelectedShips(const std::string & shipWindow, const HAPISPACE::VectorI & windowTopLeft, std::vector<EntityProperties>& entities, std::vector<EntityProperties*>& selectedEntities)
{
	for (int i = 0; i < entities.size(); i++)
	{
		UI.GetWindow(shipWindow)->DeleteObject(ENTITY + std::to_string(i));
	}
	for (int i = 0; i < entities.size(); i++)
	{
		bool isSelected{ false };
		for (int j = 0; j < selectedEntities.size(); j++)
		{
			if (&entities[i] == selectedEntities[j])
			{
				isSelected = true;
			}
		}
		if (!isSelected)
		{
			UI.GetWindow(shipWindow)->AddCanvas(ENTITY + std::to_string(i), calculateObjectWindowPosition(i), entities[i].m_sprite);
		}
		else
		{
			UI.GetWindow(shipWindow)->AddCanvas(ENTITY + std::to_string(i), calculateObjectWindowPosition(i), entities[i].m_selectedSprite);
		}
	}
}







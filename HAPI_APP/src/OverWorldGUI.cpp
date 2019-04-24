#include "OverWorldGUI.h"
#include "Textures.h"
#include "OverWorld.h"
#include "Utilities/Utilities.h"

OverWorldWindow OverWorldGUI::CURRENT_WINDOW = OverWorldWindow::eLevelSelection;

constexpr int WINDOW_OBJECTWIDTH = 75;
constexpr int WINDOW_OBJECTHEIGHT = 150;
constexpr int WINDOW_WIDTH = 830;
constexpr int WINDOW_HEIGHT = 200;
constexpr int UPGRADE_WINDOW_OBJECTWIDTH = 150;
constexpr int UPGRADE_WINDOW_OBJECTHEIGHT = 300;
constexpr int UPGRADE_WINDOW_WIDTH = 200;
constexpr int UPGRADE_WINDOW_HEIGHT = 600;

OverWorldGUI::OverWorldGUI(std::vector<EntityProperties>& entities)
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
	HAPI_Wrapper::setPosition(m_enemyTerritoryHexSheet, { 100, 600 });
	HAPI_Wrapper::setPosition(m_playButton, { 1150, 722 });
	HAPI_Wrapper::setPosition(m_backButton, { 185, 747 });
	HAPI_Wrapper::setPosition(m_upgradesButton, { 1300, 25 });
	//adding the windows and sliders, also populates the fleet window with all current entities
	UI.AddWindow(FLEET_WINDOW, HAPISPACE::RectangleI(m_fleetWindowTopLeft.x, m_fleetWindowTopLeft.x + WINDOW_WIDTH, m_fleetWindowTopLeft.y, m_fleetWindowTopLeft.y + WINDOW_HEIGHT), fleetWindowSkinName);
	for (int i = 0; i < entities.size(); i++) TODO:
	{
		UI.GetWindow(FLEET_WINDOW)->AddCanvas(ENTITY + std::to_string(i), calculateObjectWindowPosition(i), entities[i].m_sprite);
	}
	UI.GetWindow(FLEET_WINDOW)->AddSlider(FLEET_SLIDER, HAPISPACE::RectangleI(0, WINDOW_WIDTH, 160, WINDOW_HEIGHT), sliderLayout);
	UI.AddWindow(BATTLE_FLEET_WINDOW, HAPISPACE::RectangleI(m_battleFleetWindowTopLeft.x, m_battleFleetWindowTopLeft.x + WINDOW_WIDTH, m_battleFleetWindowTopLeft.y, m_battleFleetWindowTopLeft.y + WINDOW_HEIGHT), fleetWindowSkinName);
	UI.GetWindow(BATTLE_FLEET_WINDOW)->AddSlider(BATTLE_FLEET_SLIDER, HAPISPACE::RectangleI(0, WINDOW_WIDTH, 160, WINDOW_HEIGHT), sliderLayout);
	UI.AddWindow(UPGRADE_FLEET_WINDOW, HAPISPACE::RectangleI(m_upgradeFleetWindowTopLeft.x, m_upgradeFleetWindowTopLeft.x + UPGRADE_WINDOW_WIDTH, m_upgradeFleetWindowTopLeft.y, m_upgradeFleetWindowTopLeft.y + UPGRADE_WINDOW_HEIGHT));
	for (int i = 0; i < entities.size(); i++)
	{
		UI.GetWindow(UPGRADE_FLEET_WINDOW)->AddCanvas(ENTITY + std::to_string(i), calculateUpgradeObjectWindowPosition(i), entities[i].m_sprite);
	}
	UI.GetWindow(UPGRADE_FLEET_WINDOW)->AddScrollbar(UPGRADE_FLEET_SCROLLBAR);
	UI.GetWindow(UPGRADE_FLEET_WINDOW)->GetObject(UPGRADE_FLEET_SCROLLBAR)->SetValue(1.0f);

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
		case OverWorldWindow::eUpgrade:
		{
			m_upgradesScreenBackground->Render(SCREEN_SURFACE);
			SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1170, 150), HAPISPACE::Colour255::BLACK, "36", 50);//draw text gold
			SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1110, 270), HAPISPACE::Colour255::BLACK, "BUY", 50);
			SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(1110, 315), HAPISPACE::Colour255::BLACK, "SHIPS", 50);
			//SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(710, 150), HAPISPACE::Colour255::BLACK, "56", 50);//draw stat text*4
			//SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(710, 280), HAPISPACE::Colour255::BLACK, "6", 50);//draw stat text*4
			//SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(710, 410), HAPISPACE::Colour255::BLACK, "4", 50);//draw stat text*4
			//SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(710, 540), HAPISPACE::Colour255::BLACK, "3", 50);//draw stat text*4
			//SCREEN_SURFACE->DrawText(HAPISPACE::VectorI(640, 670), HAPISPACE::Colour255::BLACK, "UPGRADES: " "0" "/" "2", 50);
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
				break;
			}
			else if (HAPI_Wrapper::isTranslated(m_backButton, mouseData, 0))
			{
				CURRENT_WINDOW = OverWorldWindow::eLevelSelection;
				UI.CloseWindow(FLEET_WINDOW);
				UI.CloseWindow(BATTLE_FLEET_WINDOW);
			}
			if (HAPI_Wrapper::isTranslated(m_upgradesButton, mouseData, 0))
			{
				UI.CloseWindow(FLEET_WINDOW);
				UI.CloseWindow(BATTLE_FLEET_WINDOW);
				CURRENT_WINDOW = OverWorldWindow::eUpgrade;
				UI.OpenWindow(UPGRADE_FLEET_WINDOW);
			}
			selectBattleShip(FLEET_WINDOW, FLEET_SLIDER, BATTLE_FLEET_WINDOW, BATTLE_FLEET_SLIDER, HAPISPACE::VectorI(mouseData.x, mouseData.y), m_fleetWindowTopLeft, m_battleFleetWindowTopLeft, entities, selectedEntities);
			deselectBattleShip(BATTLE_FLEET_WINDOW, BATTLE_FLEET_SLIDER, m_battleFleetWindowTopLeft, selectedEntities, HAPISPACE::VectorI(mouseData.x, mouseData.y));
			updateSelectedShips(FLEET_WINDOW, m_fleetWindowTopLeft, entities, selectedEntities);
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
						m_currentlySelected->m_movementPoints++;
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
						m_currentlySelected->m_damage++;
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
						m_currentlySelected->m_movementPoints--;
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
						m_currentlySelected->m_damage--;
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
			checkShipSelect(selection, UPGRADE_FLEET_WINDOW, UPGRADE_FLEET_SCROLLBAR, HAPISPACE::VectorI(mouseData.x, mouseData.y), m_upgradeFleetWindowTopLeft, entities, true);
			if (selection = false)
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
		bool selection{ false };
		checkShipSelect(selection, FLEET_WINDOW, FLEET_SLIDER, HAPISPACE::VectorI(mouseData.x, mouseData.y), m_fleetWindowTopLeft, entities);
		checkShipSelect(selection, BATTLE_FLEET_WINDOW, BATTLE_FLEET_SLIDER, HAPISPACE::VectorI(mouseData.x, mouseData.y), m_battleFleetWindowTopLeft, selectedEntities);
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
				for (int i = 0; i < entities.size(); i++)
				{
					positionUpgradeEntity(UPGRADE_FLEET_WINDOW, UPGRADE_FLEET_SCROLLBAR, "entity" + std::to_string(i), i, entities.size());
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
	return HAPISPACE::VectorI((WINDOW_OBJECTWIDTH * objectNumber) - ((WINDOW_OBJECTWIDTH * vectorSize) * getWindowSliderValue(windowName, windowSliderName)), 0 );
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

#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include <memory>

#include "Global.h"
#include <string>

struct EntityProperties;
class OverWorld;
class Battle;
class OverWorldGUI 
{
private:
	EntityProperties* m_currentlySelected;
	bool m_enitiesAdded;

	std::unique_ptr<Sprite> m_battleMapBackground;
	std::unique_ptr<Sprite> m_enemyTerritoryHexSheet;
	std::unique_ptr<Sprite> m_prebattleUIBackground;
	std::unique_ptr<Sprite> m_playButton;
	std::unique_ptr<Sprite> m_backButton;



	void positionEntity(const std::string& windowName, const std::string& windowSliderName, const std::string& windowObjectName, int objectNumber, size_t vectorSize);
	void positionUpgradeEntity(const std::string& windowName, const std::string& windowScrollbarName, const std::string& windowObjectName, int objectNumber, size_t vectorSize);
	float getWindowSliderValue(const std::string &windowName, const std::string &windowSliderName) const;
	HAPISPACE::RectangleI calculateObjectWindowPosition(int objectNumber) const;
	HAPISPACE::RectangleI calculateUpgradeObjectWindowPosition(int objectNumber) const;
	HAPISPACE::RectangleI windowScreenRect(const std::string& windowName) const;
	HAPISPACE::VectorI calculateObjectScrolledPosition(const std::string& windowName, const std::string& windowSliderName, int objectNumber, size_t vectorSize);
	HAPISPACE::VectorI calculateUpgradeObjectScrolledPosition(const std::string& windowName, const std::string& windowSliderName, int objectNumber, size_t vectorSize);
	bool entityContainsMouse(const std::string& windowName, const std::string& windowObjectName, HAPISPACE::VectorI windowTopLeft, HAPISPACE::VectorI mousePosition) const;
	bool windowObjectExists(const std::string& windowName, const std::string& windowObjectName) const;
	void checkShipSelect(bool& selection, const std::string& shipWindow, const std::string& windowSlider, const HAPISPACE::VectorI& mouseData, const HAPISPACE::VectorI& windowTopLeft, std::vector<EntityProperties>& entities, const bool vertical = false);
	void checkShipSelect(bool& selection, const std::string& shipWindow, const std::string& windowSlider, const HAPISPACE::VectorI& mouseData, const HAPISPACE::VectorI& windowTopLeft, std::vector<EntityProperties*>& entities, const bool vertical = false);
	void selectBattleShip(const std::string& shipWindow, const std::string& windowSlider, const std::string& selectedShipWindow, const std::string& selectedWindowSlider, const HAPISPACE::VectorI& mouseData, const HAPISPACE::VectorI& windowTopLeft, const HAPISPACE::VectorI& selectedTopLeft, std::vector<EntityProperties>& entities, std::vector<EntityProperties*>& selectedEntities);
	void deselectBattleShip(const std::string& selectedShipWindow, const std::string& selectedShipSlider, const HAPISPACE::VectorI& selectedWindowTopLeft, std::vector<EntityProperties*>& selectedEntities, const HAPISPACE::VectorI& mouseData);
	void updateSelectedShips(const std::string& shipWindow, const HAPISPACE::VectorI& windowTopLeft, std::vector<EntityProperties>& entities, std::vector<EntityProperties*>& selectedEntities);

	HAPISPACE::SliderLayout sliderLayout;
	HAPISPACE::VectorI m_fleetWindowTopLeft{ HAPISPACE::VectorI(220, 510) };
	HAPISPACE::VectorI m_battleFleetWindowTopLeft{ HAPISPACE::VectorI(220, 220) };
	HAPISPACE::VectorI m_upgradeFleetWindowTopLeft{ HAPISPACE::VectorI(220, 120) };

	const std::string fleetWindowSliderSkinName;
	const std::string fleetWindowSkinName;
	const std::string ENTITY = "entity";
	const std::string FLEET_WINDOW = "fleetwindow";
	const std::string BATTLE_FLEET_WINDOW = "battleFleetWindow";
	const std::string FLEET_SLIDER = "fleetSlider";
	const std::string BATTLE_FLEET_SLIDER = "battleFleetSlider";
	const std::string UPGRADE_FLEET_WINDOW = "upgradefleetwindow";
	const std::string UPGRADE_FLEET_SCROLLBAR = "upgradefleetscrollbar";


	std::unique_ptr<Sprite> m_upgradesButton;
	//upgrade buttons
	std::unique_ptr<Sprite> m_upgradesScreenBackground;
	std::unique_ptr<Sprite> m_removeHealthButton;
	std::unique_ptr<Sprite> m_removeMovementButton;
	std::unique_ptr<Sprite> m_removeDamageButton;
	std::unique_ptr<Sprite> m_removeRangeButton;
	std::unique_ptr<Sprite> m_addHealthButton;
	std::unique_ptr<Sprite> m_addMovementButton;
	std::unique_ptr<Sprite> m_addDamageButton;
	std::unique_ptr<Sprite> m_addRangeButton;
	std::unique_ptr<Sprite> m_upgradeBackButton;

public:
	static OverWorldWindow CURRENT_WINDOW;
	OverWorldGUI(std::vector<EntityProperties>& entities);

	void onLeftClick(const HAPI_TMouseData& mouseData, std::vector<EntityProperties>& entities,
		std::vector<EntityProperties*>& selectedEntities, bool& startBattle);
	void onRightClick(const HAPI_TMouseData& mouseData, std::vector<EntityProperties>& entities,
		std::vector<EntityProperties*>& selectedEntities);
	void onMouseMove(const HAPI_TMouseData& mouseData, std::vector<EntityProperties>& entities,
		std::vector<EntityProperties*>& selectedEntities);

	void render(std::unique_ptr<Battle>& battle);
};
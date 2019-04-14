#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>

struct Tile;
class Battle;
struct EntityProperties;

class BattleUI : public IHapiSpritesInputListener
{
	struct InvalidPosition
	{
		InvalidPosition();

		void render() const;
		void setPosition(std::pair<int, int> screenPosition, float mapDrawScale);

		std::unique_ptr<Sprite> m_sprite;
		bool m_activate;
	};

public:
	BattleUI(Battle& battle, std::vector<EntityProperties*>& selectedEntities);

	void render() const;

	void newPhase();

	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final {}
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;

private:
	Battle& m_battle;
	const Tile* m_currentTileSelected;
	InvalidPosition m_invalidPosition;


	//ShipPlacement Phase
	std::vector<EntityProperties*>* m_selectedEntities;
	EntityProperties* m_currentSelectedEntity;
	std::vector<const Tile*> m_spawnTiles;
	std::vector<std::unique_ptr<Sprite>> m_spawnSprites;

	void onMouseMoveShipPlacementPhase();
	void onLeftClickShipPlacement();


	//Movement Phase
	void onMouseMoveMovementPhase();
	void onLeftClickMovementPhase();
	void onRightClickMovementPhase();


	//Attack Phase
	void onLeftClickAttackPhase();
};
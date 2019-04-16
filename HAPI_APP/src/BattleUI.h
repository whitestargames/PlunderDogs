#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include "BattleGUI.h"

struct Tile;
class Battle;
struct EntityProperties;

class BattleUI : public IHapiSpritesInputListener
{
	struct InvalidPositionSprite
	{
		InvalidPositionSprite();

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
	InvalidPositionSprite m_invalidPositionSprite;

	//Movement Phase
	void handleOnMouseMoveMovementPhase();
	void handleOnLeftClickMovementPhase();
	void handleOnRightClickMovementPhase();

	//Attack Phase
	void handleOnLeftClickAttackPhase();
};
#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>

struct Tile;
class Map;
class Battle;
class BattleUI : public IHapiSpritesInputListener
{
public:
	BattleUI(Battle& battle);

	void render() const;

	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final {}
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;

private:
	Battle& m_battle;
	Tile* m_currentTileSelected;
	std::unique_ptr<Sprite> m_invalidLocationSprite;
	bool m_renderSprite;
};

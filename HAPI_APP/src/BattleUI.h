#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>

struct Tile;
class Battle;
class BattleUI : public IHapiSpritesInputListener
{
	struct InvalidMovementLocationSprite
	{
		InvalidMovementLocationSprite(const std::string& spriteName);

		void render() const;
		void setPosition(std::pair<int, int> screenPosition, float mapDrawScale);

		std::unique_ptr<Sprite> m_sprite;
		bool m_renderSprite;
	};

public:
	BattleUI(Battle& battle);

	void render() const;

	void OnKeyEvent(EKeyEvent keyEvent, BYTE keyCode) override final {}
	void OnMouseEvent(EMouseEvent mouseEvent, const HAPI_TMouseData& mouseData) override final;
	void OnMouseMove(const HAPI_TMouseData& mouseData) override final;

private:
	Battle& m_battle;
	Tile* m_currentTileSelected;
	InvalidMovementLocationSprite m_invalidMovementLocationSprite;
};
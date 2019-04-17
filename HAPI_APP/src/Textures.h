#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include <memory>
#include "HAPIWrapper.h"

struct Textures
{
	static bool loadAllTextures()
	{
		m_mouseCrossHair = HAPI_Wrapper::makeSpriteSheet("crossHair.png"); 
		if (!m_mouseCrossHair)
		{
			HAPI_Sprites.UserMessage("Cannot Load : 'mouseCrossHair.xml'", "Texture Error");
			return false;
		}

		m_battleMapBackground = HAPI_Wrapper::makeSpriteSheet("TempBattleMapBackground.png");
		if (!m_battleMapBackground)
		{
			HAPI_Sprites.UserMessage("Cannot Load : 'TempBattleMapBackground.png'", "Texture Error");
		}

		m_enemyTerritoryHexSheet = HAPI_Wrapper::makeSpriteSheet("EnemyTerritoryHexSheet.png", 2);
		if (!m_enemyTerritoryHexSheet)
		{
			HAPI_Sprites.UserMessage("Cannot Load : 'dsdasdasdasdasdasdasdasml'", "Texture Error");
			return false;
		}

		m_prebattleUIBackground = HAPI_Wrapper::makeSpriteSheet("PrebattleUI.png");
		if (!m_prebattleUIBackground)
		{
			HAPI_Sprites.UserMessage("Cannot Load : 'PrebattleUI.png'", "Texture Error");
			return false;
		}

		m_preBattleUIPlayButton = HAPI_Wrapper::makeSpriteSheet("PrebattleUIPlayButton.png", 2);
		if (!m_preBattleUIPlayButton)
		{
			HAPI_Sprites.UserMessage("Cannot Load : 'PrebattleUIPlayButton.png'", "Texture Error");
			return false;
		}

		m_preBattleUIBackButton = HAPI_Wrapper::makeSpriteSheet("PreBattleUIBackButton.png", 2);
		if (!m_preBattleUIBackButton)
		{
			HAPI_Sprites.UserMessage("Cannot Load : 'PrebattleUIBackButton.png'", "Texture Error");
			return false;
		}

		m_thing = HAPI_Wrapper::makeSpriteSheet("thing.png");
		if (!m_thing)
		{
			HAPI_Sprites.UserMessage("Cannot Load : 'thing.png'", "Texture Error");
			return false;
		}


		m_hexTiles = HAPI_Wrapper::loadSpriteSheet("hexTiles.xml");
		if (!m_hexTiles)
		{
			HAPI_Sprites.UserMessage("Cannot Load : 'hexTiles.xml'", "Texture Error");
			return false;
		}
		// xml names for ships shipSideCannon and straightShotCannonShip
		m_ship = HAPI_Wrapper::loadSpriteSheet("shipSideCannon.xml");
		if (!m_ship)
		{
			HAPI_Sprites.UserMessage("Connot Load: 'shipSideCannon.xml'", "Texture Error");
			return false;
		}
		
		m_spawnHex = HAPI_Wrapper::makeSpriteSheet("spawnHex.png");
		if (!m_spawnHex)
		{
			HAPI_Sprites.UserMessage("Connot Load: 'spawnHex.png'", "Texture Error");
			return false;
		}

		return true;
	}

	static std::shared_ptr<SpriteSheet> m_mouseCrossHair;
	static std::shared_ptr<SpriteSheet> m_battleMapBackground;
	static std::shared_ptr<SpriteSheet> m_enemyTerritoryHexSheet;
	static std::shared_ptr<SpriteSheet> m_prebattleUIBackground;
	static std::shared_ptr<SpriteSheet> m_preBattleUIPlayButton;
	static std::shared_ptr<SpriteSheet> m_preBattleUIBackButton;
	static std::shared_ptr<SpriteSheet> m_thing;
	static std::shared_ptr<SpriteSheet> m_hexTiles;
	static std::shared_ptr<SpriteSheet> m_ship;
	static std::shared_ptr<SpriteSheet> m_spawnHex;

};
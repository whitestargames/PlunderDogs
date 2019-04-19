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
		
		m_spawnHex = HAPI_Wrapper::makeSpriteSheet("spawnHex.png");
		if (!m_spawnHex)
		{
			HAPI_Sprites.UserMessage("Connot Load: 'spawnHex.png'", "Texture Error");
			return false;
		}

		m_selectedHex = HAPI_Wrapper::makeSpriteSheet("whiteHex75.png");
		if (!m_selectedHex)
		{
			HAPI_Sprites.UserMessage("Connot Load: 'WhiteHex75.png'", "Texture Error");
			return false;
		}

		//blueShips
		m_blueShipSideCannons = HAPI_Wrapper::loadSpriteSheet("shipBlueSide.xml");
		if (!m_blueShipSideCannons)
		{
			HAPI_Sprites.UserMessage("Connot Load: 'shipBlueSide.xml'", "Texture Error");
			return false;
		}
		m_blueShipBomb = HAPI_Wrapper::loadSpriteSheet("shipBlueBomb.xml");
		if (!m_blueShipBomb)
		{
			HAPI_Sprites.UserMessage("Connot Load: 'shipBlueBomb.xml'", "Texture Error");
			return false;
		}
		m_blueShipMelee = HAPI_Wrapper::loadSpriteSheet("shipBlueMelee.xml");
		if (!m_blueShipMelee)
		{
			HAPI_Sprites.UserMessage("Connot Load: 'shipBlueMelee.xml'", "Texture Error");
			return false;
		}
		m_blueShipSnipe = HAPI_Wrapper::loadSpriteSheet("shipBlueSnipe.xml");
		if (!m_blueShipSnipe)
		{
			HAPI_Sprites.UserMessage("Connot Load: 'shipBlueSnipe.xml'", "Texture Error");
			return false;
		}

		//RedShips
		m_redShipSideCannons = HAPI_Wrapper::loadSpriteSheet("shipRedSide.xml");
		if (!m_redShipSideCannons)
		{
			HAPI_Sprites.UserMessage("Connot Load: 'shipRedSide.xml'", "Texture Error");
			return false;
		}
		m_redShipBomb = HAPI_Wrapper::loadSpriteSheet("shipRedBomb.xml");
		if (!m_redShipBomb)
		{
			HAPI_Sprites.UserMessage("Connot Load: 'shipRedBomb.xml'", "Texture Error");
			return false;
		}
		m_redShipMelee = HAPI_Wrapper::loadSpriteSheet("shipRedMelee.xml");
		if (!m_redShipMelee)
		{
			HAPI_Sprites.UserMessage("Connot Load: 'shipRedMelee.xml'", "Texture Error");
			return false;
		}
		m_redShipSnipe = HAPI_Wrapper::loadSpriteSheet("shipRedSnipe.xml");
		if (!m_redShipSnipe)
		{
			HAPI_Sprites.UserMessage("Connot Load: 'shipRedSnipe.xml'", "Texture Error");
			return false;
		}

		//GreenShips
		m_greenShipSideCannons = HAPI_Wrapper::loadSpriteSheet("shipGreenSide.xml");
		if (!m_greenShipSideCannons)
		{
			HAPI_Sprites.UserMessage("Connot Load: 'shipGreenSide.xml'", "Texture Error");
			return false;
		}
		m_greenShipBomb = HAPI_Wrapper::loadSpriteSheet("shipGreenBomb.xml");
		if (!m_greenShipBomb)
		{
			HAPI_Sprites.UserMessage("Connot Load: 'shipGreenBomb.xml'", "Texture Error");
			return false;
		}
		m_greenShipMelee = HAPI_Wrapper::loadSpriteSheet("shipGreenMelee.xml");
		if (!m_greenShipMelee)
		{
			HAPI_Sprites.UserMessage("Connot Load: 'shipGreenMelee.xml'", "Texture Error");
			return false;
		}
		m_greenShipSnipe = HAPI_Wrapper::loadSpriteSheet("shipGreenSnipe.xml");
		if (!m_greenShipSnipe)
		{
			HAPI_Sprites.UserMessage("Connot Load: 'shipGreenSnipe.xml'", "Texture Error");
			return false;
		}

		//YellowShips
		m_yellowShipSideCannons = HAPI_Wrapper::loadSpriteSheet("shipYellowSide.xml");
		if (!m_yellowShipSideCannons)
		{
			HAPI_Sprites.UserMessage("Connot Load: 'shipYellowSide.xml'", "Texture Error");
			return false;
		}
		m_yellowShipBomb = HAPI_Wrapper::loadSpriteSheet("shipYellowBomb.xml");
		if (!m_yellowShipBomb)
		{
			HAPI_Sprites.UserMessage("Connot Load: 'shipYellowBomb.xml'", "Texture Error");
			return false;
		}
		m_yellowShipMelee = HAPI_Wrapper::loadSpriteSheet("shipYellowMelee.xml");
		if (!m_yellowShipMelee)
		{
			HAPI_Sprites.UserMessage("Connot Load: 'shipYellowMelee.xml'", "Texture Error");
			return false;
		}
		m_yellowShipSnipe = HAPI_Wrapper::loadSpriteSheet("shipYellowSnipe.xml");
		if (!m_yellowShipSnipe)
		{
			HAPI_Sprites.UserMessage("Connot Load: 'shipYellowSnipe.xml'", "Texture Error");
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
	static std::shared_ptr<SpriteSheet> m_spawnHex;
	static std::shared_ptr<SpriteSheet> m_selectedHex;
	static std::shared_ptr<SpriteSheet> m_blueShipSideCannons;
	static std::shared_ptr<SpriteSheet> m_blueShipBomb;
	static std::shared_ptr<SpriteSheet> m_blueShipMelee;
	static std::shared_ptr<SpriteSheet> m_blueShipSnipe;
	static std::shared_ptr<SpriteSheet> m_redShipSideCannons;
	static std::shared_ptr<SpriteSheet> m_redShipBomb;
	static std::shared_ptr<SpriteSheet> m_redShipMelee;
	static std::shared_ptr<SpriteSheet> m_redShipSnipe;
	static std::shared_ptr<SpriteSheet> m_greenShipSideCannons;
	static std::shared_ptr<SpriteSheet> m_greenShipBomb;
	static std::shared_ptr<SpriteSheet> m_greenShipMelee;
	static std::shared_ptr<SpriteSheet> m_greenShipSnipe;
	static std::shared_ptr<SpriteSheet> m_yellowShipSideCannons;
	static std::shared_ptr<SpriteSheet> m_yellowShipBomb;
	static std::shared_ptr<SpriteSheet> m_yellowShipMelee;
	static std::shared_ptr<SpriteSheet> m_yellowShipSnipe;
};
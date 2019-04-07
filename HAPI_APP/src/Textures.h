#pragma once

#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include <memory>
#include "HAPIWrapper.h"

//
//Reasons for this change.
//
//Currently the data file is a singular megabyte in size
//The simplest option for texture handling is to load all of the textures immediately even before the game starts.
//Doing so allows all of the textures to be loaded in one place and not throughout the project which causes confusion.
//It also allows for error checking to be handled within one location too. 

//An example of when not to do this would be when all of your textures take up a large number of megabytes 
//Whjen loading the texture - the data is stored on the GPU.
//At the moment we're storing a megabyte on it. Nothing at all. 
//If it was taking up say 500mb of GPU memory, then you'd change the textures to load on certain states.
//Implementing that sort of functionality here would be a good example of over engineering
//Thus making the code harder to understand 
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

		m_hexTiles = HAPI_Wrapper::makeSpriteSheet("hexTiles.xml");
		if (!m_hexTiles)
		{
			HAPI_Sprites.UserMessage("Cannot Load : 'hexTiles.xml'", "Texture Error");
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
};
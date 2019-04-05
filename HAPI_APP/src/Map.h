#pragma once
#include <utility>
#include <vector>
#include <string>
#include <HAPISprites_lib.h>
#include <HAPISprites_UI.h>
#include "global.h"

constexpr float DRAW_OFFSET_X{ 12 };
constexpr float DRAW_OFFSET_Y{ 28 };

//Info for HAPI_SPRITES - Leave for now. 
/*
//	//m_sprite->GetSpritesheet()->GenerateNormals(true);
**** HAPI Sprites: ERROR Detected
** Description: H_INVALID_PARAMETER - passed parameter was not valid
** System: HAPI Sprites
** Detail: surface width does not divide equally by numFrames
****
*/
struct BattleEntity;
struct Entity;
struct Tile
{
	enum eTileType m_type;
	//TODO: Dangerous exposure of raw pointer
	BattleEntity* m_entityOnTile;
	std::unique_ptr<HAPISPACE::Sprite> m_sprite;
	const std::pair<int, int> m_tileCoordinate;

	Tile(eTileType type, const std::string& spriteName, std::pair<int, int> coord) :
		m_type(type), m_tileCoordinate(coord),
		m_entityOnTile(nullptr)
	{
		//HAPI's Sprite constructor takes two strings: the name of the file to load (append .xml) 
		//and the path to that file relative to the program
		m_sprite = HAPI_Sprites.LoadSprite(spriteName);

	}
	Tile(eTileType type, std::shared_ptr<HAPISPACE::SpriteSheet> spriteSheet, std::pair<int, int> coord) :
		m_type(type), m_tileCoordinate(coord),
		m_entityOnTile(nullptr)
	{
		//HAPI's make sprite takes a pointer to an existing spritesheet
		m_sprite = HAPI_Sprites.MakeSprite(spriteSheet);
	}
	Tile(const Tile &other) : m_tileCoordinate(std::pair<int, int>(other.m_tileCoordinate.first, other.m_tileCoordinate.second))
	{
		m_type = other.m_type;
		m_entityOnTile = other.m_entityOnTile;
		m_sprite = HAPI_Sprites.MakeSprite(other.m_sprite->GetSpritesheet());
	}
};

class Map
{
private:
	const std::pair<int, int> m_mapDimensions;
	float m_windStrength;
	eDirection m_windDirection;
	float m_drawScale;
	std::pair<int, int> m_drawOffset;
	std::unique_ptr<HAPISPACE::Sprite> motherSprite; //All tiles inherit from this sprite
	std::vector<Tile> m_data;

	std::pair<int, int> offsetToCube(std::pair<int, int> offset) const;
	std::pair<int, int> cubeToOffset(std::pair<int, int> cube) const;
	int cubeDistance(std::pair<int, int> a, std::pair<int, int> b) const;
	bool inCone(std::pair<int, int> orgHex, std::pair<int, int> testHex, eDirection dir) const;
	//Finds the euclidean distance from a point to a tile's centre, used by getMouseClickCoord
	float tileDistanceMag(std::pair<int, int> tileCoord, std::pair<int, int> mouseClick) const;
public:
	//Returns a pointer to a given tile, returns nullptr if there is no tile there
	Tile *getTile(std::pair<int, int> coordinate);
	//An n = 1 version of getTileRadius for use in pathfinding, 
	//returns nullptr for each tile out of bounds
	std::vector<Tile*> getAdjacentTiles(std::pair<int, int> coord);
	//TODO:Returns tiles in a radius around a given tile, skipping the tile itself
	std::vector<Tile*> getTileRadius(std::pair<int, int> coord, int range);
	//TODO: Returns tiles in a cone emanating from a given tile, skipping the tile itself
	std::vector<Tile*> getTileCone(std::pair<int, int> coord, int range, eDirection direction);

	//For finding the location on the screen a given tile is being drawn
	std::pair<int, int> getTileScreenPos(std::pair<int, int> coord) const;

	//For finding the closest tile to the current mouse location, 
	//can give values that aren't valid tiles if you click off the map 
	//so check if getTile is null before using
	std::pair<int, int> getMouseClickCoord(std::pair<int, int> mouseCoord) const;

	//Moves an entitys position on the map, returns false if the position is already taken
	bool moveEntity(std::pair<int, int> originalPos, std::pair<int, int> newPos);
	//Places a new entity on the map (no check for duplicates yet so try to avoid creating multiples)
	void insertEntity(BattleEntity& newEntity);

	void drawMap() const;
	std::pair<int, int> getDrawOffset() const { return m_drawOffset; }
	void setDrawOffset(std::pair<int, int> newOffset) { m_drawOffset = newOffset; }

	std::pair<int, int> getDimensions() const { return m_mapDimensions;  }

	float getDrawScale() const { return m_drawScale; }
	void setDrawScale(float scale) { if (scale > 0.0) m_drawScale = scale; }

	float getWindStrength() const { return m_windStrength; }
	void setWindStrength(float strength) { if (strength > 0.0) m_windStrength = strength; }

	eDirection getWindDirection() const { return m_windDirection; }
	void setWindDirection(eDirection direction) { m_windDirection = direction; }

	//TODO: Get constructor working. Need tiled parser or load from xml set up
	Map(std::pair<int, int> size, const std::vector<std::vector<int>>& tileData);
};
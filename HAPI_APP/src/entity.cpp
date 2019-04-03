#include "entity.h"

constexpr size_t MOVEMENT_PATH_SIZE{ 32 };

MovementPath::MovementPath(Entity & owningEntity)
{
		//Initialize Movement Path
	m_movementPath.reserve(size_t(MOVEMENT_PATH_SIZE));
	for (int i = 0; i < MOVEMENT_PATH_SIZE; i++)
	{
		std::pair<std::unique_ptr<Sprite>, bool> sprite;
		sprite.first = HAPI_Sprites.MakeSprite(m_mouseCursor->GetSpritesheet());
		sprite.second = false;
		m_movementPath.push_back(std::move(sprite));
	}
}

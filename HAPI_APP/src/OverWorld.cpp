#include  "Overworld.h"
#include "Battle.h"
#include "Utilities/Utilities.h"
#include "HAPIWrapper.h"
#include "Textures.h"

OverWorldWindow OverWorld::CURRENT_WINDOW = OverWorldWindow::LevelSelection;

OverWorld::OverWorld()
	: m_GUI(),
	m_battle()
{}

void OverWorld::render()
{
	m_GUI.render();

	if (CURRENT_WINDOW == OverWorldWindow::Battle)
	{
		m_battle.render();
	}
}

void OverWorld::update(float deltaTime)
{
	switch (CURRENT_WINDOW)
	{
	case OverWorldWindow::Battle :
	{
		m_battle.update(deltaTime);
		break;
	}
	}
}
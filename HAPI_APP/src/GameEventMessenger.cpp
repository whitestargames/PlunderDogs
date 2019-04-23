#include "GameEventMessenger.h"

std::unordered_map<GameEvent, std::vector<Listener>>  GameEventMessenger::m_listeners;

//OverWorldWindow OverWorldGUI::CURRENT_WINDOW = OverWorldWindow::eLevelSelection;
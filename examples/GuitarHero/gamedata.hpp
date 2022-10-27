#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { Right, Left, Down};
enum class State { Playing, GameOver};

struct GameData {
  State m_state{State::Playing};
  std::bitset<3> m_input;  
};

#endif
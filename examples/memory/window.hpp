#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onPaintUI() override;

private:
  static int const m_N{4}; // Board size is m_N x m_N

  enum class GameState { Play, Finish, Close };
  GameState m_gameState;

  bool m_XsTurn{true};
  int m_board[4][4]; // 0, 1 or 2

  ImFont *m_font{};

  void checkEndCondition();
  void restartGame();
  void validaCor(int i, int j);
};

#endif
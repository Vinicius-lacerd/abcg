#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <random>

#include "abcgOpenGL.hpp"

#include "player.hpp"
#include "platforms.hpp"


class Window : public abcg::OpenGLWindow {
protected:
  void onEvent(SDL_Event const &event) override;
  void onCreate() override;
  void onUpdate() override;
  void onPaint() override;
  void onPaintUI() override;
  void onResize(glm::ivec2 const &size) override;
  void onDestroy() override;
  void checkHitColor(GameData const &gameData);
  void checkGameOver();

private:
  glm::ivec2 m_viewportSize{};

  int score = {}, highScore = {}, errors ={};
  GLuint m_objectsProgram{};

  GameData m_gameData;

  Player m_player;

  Platforms m_platforms;

  abcg::Timer m_restartWaitTimer;

  ImFont *m_fontBig{}, *m_fontSmall{};

  std::default_random_engine m_randomEngine;

  void restart();
};

#endif
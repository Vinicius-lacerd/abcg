#ifndef PLATFORMS_HPP_
#define PLATFORMS_HPP_

#include "abcgOpenGL.hpp"

#include <list>
#include <random>

#include "gamedata.hpp"
#include "player.hpp"

class Platforms {
public:
  void create(GLuint program,int quantity);
  void paint();
  void destroy();
  void update(const Player &player, float deltaTime);


  struct Platform {
    GLuint m_VAO{};
    GLuint m_VBO{};
    glm::vec4 m_color1{0.94f, 0.9f, 0.6f, 1.0f};//yellow
    glm::vec4 m_color2{1.0f, 0.0f, 0.0f, 1.0f};//red
    glm::vec4 m_color3{0.0f, 0.0f, 1.0f, 1.0f};//blue
    float m_rotation{};
    float m_scale{0.26f};
    glm::vec2 m_translation{};
    std::array<glm::vec2,4> positions;

    float height{};
    glm::vec2 m_top_left{};
    glm::vec2 m_top_right{};
    glm::vec2 m_bottom_left{};
    glm::vec2 m_bottom_right{}; 
    
    int platColor{};
  };

  float yvel{-0.1f};
  std::list<Platform> m_platforms;

  Platform makePlatform(); 

private:
  GLuint m_program{};
  GLint m_translationLoc{};
  GLint m_colorLoc{};
  GLint m_scaleLoc{};
  GLint m_rotationLoc{};

  float step_height{0.7f};
  float horizontal_drift{0.0f};
  std::default_random_engine m_randomEngine;
  std::uniform_real_distribution<float> m_randomDist{-0.8f, 0.8f};
  abcg::Timer speedUpCooldownTimer; 
  
};
#endif
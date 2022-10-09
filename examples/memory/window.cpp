#include "window.hpp"
#include <chrono>
#include <iostream>
#include <thread>
using namespace std;

void Window::onCreate() { restartGame(); }
int previous, second;
int previous_i, second_i;
int previous_j, second_j;
bool changeColor = false;
int errors = 0;

void Window::onPaintUI() {
  // Get size of application's window
  auto const appWindowWidth{gsl::narrow<float>(getWindowSettings().width)};
  auto const appWindowHeight{gsl::narrow<float>(getWindowSettings().height)};

  {
    ImGui::SetNextWindowSize(ImVec2(appWindowWidth, appWindowHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    auto const flags{ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoTitleBar};
    ImGui::Begin("Memory Game", nullptr, flags);

    ImGui::Spacing();

    // Static text showing current turn or win/draw messages
    {
      std::string text;
      switch (m_gameState) {
      case GameState::Play:
        text = fmt::format("{}  - errors {}",
                           m_XsTurn ? "Turn First Card" : "Turn Match Card",
                           errors);
        break;
      case GameState::Close:
        text = fmt::format(
            "Click in any card to finish yor move!!  - errors {}", errors);
        break;
      case GameState::Finish:
        text = fmt::format("Finish!!  - errors {}", errors);
        break;
      }
      // Center text
      ImGui::SetCursorPosX(
          (appWindowWidth - ImGui::CalcTextSize(text.c_str()).x) / 2);
      ImGui::Text("%s", text.c_str());
      ImGui::Spacing();
    }

    ImGui::Spacing();

    // Game board
    {
      auto const board_size = min((appWindowWidth - 10 * m_N),
                                  (appWindowHeight - 30 - 30 - 60 - 5 * m_N));

      auto const buttonWidth{board_size / m_N};
      auto const buttonHeight{buttonWidth};

      // Use custom font
      if (ImGui::BeginTable("Game board", m_N)) {
        for (auto i : iter::range(m_N)) {
          ImGui::TableNextRow();
          for (auto j : iter::range(m_N)) {
            ImGui::TableSetColumnIndex(j);

            auto buttonText = fmt::format("##{:#02}{:#02}", i, j);

            ImGui::PushID(i * 100 + j);

            validaCor(i, j);

            if (ImGui::Button(buttonText.c_str(),
                              ImVec2(buttonWidth, buttonHeight))) {
              if (changeColor) {
                changeColor = false;
                m_gameState = GameState::Play;

                //valida se os dois ultimos cars abertos sao iguais e zera o valor se sim
                if (m_board[second_i][second_j] == previous) {
                  m_board[second_i][second_j] = 0;
                  m_board[previous_i][previous_j] = 0;
                } else {
                  m_board[second_i][second_j] =
                      m_board[second_i][second_j] * -1;
                  m_board[previous_i][previous_j] =
                      m_board[previous_i][previous_j] * -1;
                  errors = errors + 1;
                }
              } else if (m_gameState == GameState::Play && m_board[i][j] != 0) {
                //clique do jogador salva os card abertos e altera o valor pra exibir a cor
                m_board[i][j] = m_board[i][j] * -1;
                if (m_XsTurn) {
                  previous = m_board[i][j];
                  previous_i = i;
                  previous_j = j;
                } else {
                  m_gameState = GameState::Close;
                  changeColor = true;
                  second = m_board[i][j];
                  second_i = i;
                  second_j = j;
                }
                m_XsTurn = !m_XsTurn;
              }
              checkEndCondition();
            }

            ImGui::PopStyleColor(3);
            ImGui::PopID();
          }
          ImGui::Spacing();
        }
        ImGui::EndTable();
      }
    }

    ImGui::Spacing();
    {
      if (ImGui::Button("Restart game", ImVec2(-1, 50))) {
        restartGame();
      }
    }

    ImGui::End();
  }
}

void Window::restartGame() {
  int grid[8];
  bool valido = false;
  //limito em 2 o numero de cards por cor - para existir todas as cores e não deixar o jogo incompleto
  std::fill(grid, grid + sizeof(8), 0);
  for (auto i : iter::range(m_N)) {
    for (auto j : iter::range(m_N)) {
      while (!valido) {
        int r = 1 + (rand() % 8);
        if (grid[r] < 2) {
          m_board[i][j] = r;
          grid[r] = grid[r] + 1;
          valido = true;
        } 
      }
      valido = false;
    }
  }
  m_XsTurn = true;
  m_gameState = GameState::Play;
  errors = 0;
}
void Window::validaCor(int i, int j) {
  static ImVec4 back_color = {0.5f, 0.5f, 0.5f, 1.0f};
  static ImVec4 finish_color = {0.25f, 0.25f, 0.25f, 1.0f};

  static ImVec4 yellow = {0.94f, 0.9f, 0.6f, 1.0f};//1
  static ImVec4 black = {0.0f, 0.0f, 0.0f, 1.0f};//2
  static ImVec4 white = {1.0f, 1.0f, 1.0f, 1.0f};//3
  static ImVec4 red = {1.0f, 0.0f, 0.0f, 1.0f};//4
  static ImVec4 green = {0.0f, 1.0f, 0.0f, 1.0f};//5
  static ImVec4 blue = {0.0f, 0.0f, 1.0f, 1.0f};//6
  static ImVec4 purple = {0.3f, 0.3f, 1.0f, 1.0f};//7
  static ImVec4 salmon = {1.0f, 0.3f, 0.3f, 1.0f};//8

  if (m_board[i][j] > 0) {
    ImGui::PushStyleColor(ImGuiCol_Button, back_color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, back_color);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, back_color);
  } else if (m_board[i][j] == 0) {
    ImGui::PushStyleColor(ImGuiCol_Button, finish_color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, finish_color);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, finish_color);
  } else {
    if (m_board[i][j] == -2) {
      ImGui::PushStyleColor(ImGuiCol_Button, white);
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, white);
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, white);
    } else if (m_board[i][j] == -1) {
      ImGui::PushStyleColor(ImGuiCol_Button, black);
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, black);
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, black);
    } else if (m_board[i][j] == -3) {
      ImGui::PushStyleColor(ImGuiCol_Button, yellow);
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, yellow);
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, yellow);
    } else if (m_board[i][j] == -4) {
      ImGui::PushStyleColor(ImGuiCol_Button, red);
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, red);
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, red);
    } else if (m_board[i][j] == -5) {
      ImGui::PushStyleColor(ImGuiCol_Button, green);
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, green);
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, green);
    } else if (m_board[i][j] == -6) {
      ImGui::PushStyleColor(ImGuiCol_Button, blue);
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, blue);
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, blue);
    } else if (m_board[i][j] == -7) {
      ImGui::PushStyleColor(ImGuiCol_Button, purple);
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, purple);
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, purple);
    } else if (m_board[i][j] == -8) {
      ImGui::PushStyleColor(ImGuiCol_Button, salmon);
      ImGui::PushStyleColor(ImGuiCol_ButtonHovered, salmon);
      ImGui::PushStyleColor(ImGuiCol_ButtonActive, salmon);
    }
  }
}
void Window::checkEndCondition() {
  //se so sobrou zeros = fim do game
  int sum = 0;
  for (auto i : iter::range(m_N)) {
    for (auto j : iter::range(m_N)) {
      sum = sum + m_board[i][j];
    }
  }
  if (sum == 0) {
    m_gameState = GameState::Finish;
    return;
  }
}
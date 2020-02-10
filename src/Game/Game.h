#pragma once

#include <SFML/Graphics.hpp>
#include "Config.h"

class Lookup;

enum class StateCell { None = 0, X = 1, O = 2 };

class Game : public sf::Drawable, public sf::Transformable {
private:
  Lookup& lookup_;
  unsigned int cells_count_;
  float cell_size_;
  std::pair<StateCell, std::vector<std::pair<int, int>>> win_state_;
  bool is_first_turn_ai_ = false;
public:
  Game(Lookup& l);
  std::vector<std::vector<StateCell>> board;
  void SetFirstTurnAI(bool state) { is_first_turn_ai_ = state; }
  bool GetFirstTurnAI() { return is_first_turn_ai_; }
  void ResizeBoard();
  void MouseButtonPressed(sf::Vector2f point);
  void MouseMoved(sf::Vector2f point) {};
  void SetCellsCount(unsigned int count) { cells_count_ = count; }
  unsigned int GetCellsCount() { return cells_count_; }
  bool IsFinished();
  StateCell GetWinner() { return win_state_.first; }
public:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

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
public:
  Game(Lookup& l);
  std::vector<std::vector<StateCell>> board;
  void ResizeBoard();
  void MouseButtonPressed(sf::Vector2f point);
  void MouseMoved(sf::Vector2f point) {};
  void SetCellsCount(unsigned int count);
  bool IsFinished();
  StateCell GetWinner();
  unsigned int GetCellsCount();
public:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

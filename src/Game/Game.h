#pragma once

#include <SFML/Graphics.hpp>

#include "Config.h"

class Lookup;

enum class StateCell { None = 0, X = 1, O = 2 };

class Game : public sf::Drawable, public sf::Transformable {
protected:
  Lookup& lookup;
  bool solved;
  unsigned int cellsCount;
  float cellSize;
public:
  Game(Lookup& l);
  std::vector<std::vector<StateCell>> board;
  void resizeBoard();
  void mouseButtonPressed(sf::Vector2f point);
  void mouseMoved(sf::Vector2f point) {};
  void setCellsCount(unsigned int count);
  bool isFinished();
  unsigned int getCellsCount();
public:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

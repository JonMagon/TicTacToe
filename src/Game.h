#pragma once

#include <SFML/Graphics.hpp>
#include "Config.h"

enum class StateCell { None = 0, X = 1, O = 2 };

class Game : public sf::Drawable, public sf::Transformable {
protected:
  bool solved;
  unsigned int cellsCount;
  float cellSize;
public:
  Game();
  void resizeMatrix();
  std::vector<std::vector<StateCell>> matrix;
  void mouseButtonPressed(sf::Vector2f point);
  void mouseMoved(sf::Vector2f point) {};
  void setCellsCount(unsigned int count);
public:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

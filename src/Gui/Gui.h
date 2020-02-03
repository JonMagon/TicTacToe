#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

// Предварительное объявление
class Game;
class Control;

class Gui : public sf::Drawable, public sf::Transformable {
protected:
  Game& game;
  std::vector<std::shared_ptr<Control>> controls;
  bool isPointInside(const std::shared_ptr<Control>& contol, sf::Vector2f point);
public:
  Gui(Game& game);
  void Init();
  void mouseButtonPressed(sf::Vector2f point);
  void mouseButtonReleased(sf::Vector2f point);
  void mouseMoved(sf::Vector2f point);
public:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

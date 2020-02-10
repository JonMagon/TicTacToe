#pragma once

#include <SFML/Graphics.hpp>

#include <memory>

// Предварительное объявление
class Game;
class Control;

class Gui : public sf::Drawable, public sf::Transformable {
private:
  Game& game_;
  std::map<std::string, std::shared_ptr<Control>> controls_;
  bool IsPointInside(const std::shared_ptr<Control>& contol,
                     sf::Vector2f point);
                     void Refresh();
public:
  Gui(Game& game);
  void Initialize();
  void MouseButtonPressed(sf::Vector2f point);
  void MouseButtonReleased(sf::Vector2f point);
  void MouseMoved(sf::Vector2f point);
public:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

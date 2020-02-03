#pragma once

#include <SFML/Graphics.hpp>

class Game;

class Control : public sf::Drawable, public sf::Transformable {
protected:
  std::wstring titleText;
  bool isHovered = false;
  bool isPressed = false;
  sf::Vector2f size;
  sf::Vector2f position;
public:
  Control(std::wstring title, sf::Vector2f pos) {
    position = pos;
    titleText = title;
  };
  void (*onClick) (Game& game) = NULL;
  sf::Vector2f getSize() { return size; }
  sf::Vector2f getPosition() { return position; }
  bool getPressed() { return isPressed; }
  void setSize(sf::Vector2f val) { size = val; }
  void setHovered(bool state) { isHovered = state; }
  void setPressed(bool state) { isPressed = state; }
};

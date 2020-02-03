#pragma once

#include "Control.h"
#include "Config.h"

class Button : public Control {
public:
  Button(std::wstring title) : Button(title, sf::Vector2f()) { };
  Button(std::wstring title, sf::Vector2f pos) :
    Control(title, pos) {
      size = sf::Vector2f(BUTTON_DEFAULT_WIDTH, BUTTON_DEFAULT_HEIGHT);
    }
  ~Button() {};
public:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

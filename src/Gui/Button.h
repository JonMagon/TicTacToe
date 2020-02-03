#pragma once

#include "Control.h"
#include "Config.h"

class Button : public Control {
public:
  Button(std::wstring title) : Button(title, 0, 0) { };
  Button(std::wstring title, unsigned int x, unsigned int y) :
    Control(title, x, y) {
      size = sf::Vector2f(BUTTON_DEFAULT_WIDTH, BUTTON_DEFAULT_HEIGHT);
    }
  ~Button() {};
public:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

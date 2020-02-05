#pragma once

#include "Control.h"

#include "Config.h"

class Button : public Control {
public:
  Button(std::wstring title_text) : Button(title_text, sf::Vector2f()) { };
  Button(std::wstring title_text, sf::Vector2f position) :
    Control(title_text, position) {
      size_ = sf::Vector2f(kButtonDefaultWidth, kButtonDefaultHeight);
    }
  ~Button() {};
public:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

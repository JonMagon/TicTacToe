#pragma once

#include "Control.h"

#include "Config.h"

class Label : public Control {
public:
  Label(std::wstring title) : Label(title, sf::Vector2f()) { };
  Label(std::wstring title, sf::Vector2f pos) :
    Control(title, pos) {};
  ~Label() {};
public:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

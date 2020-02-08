#pragma once

#include "Control.h"

#include "Config.h"

class Label : public Control {
public:
  Label(std::wstring title_text) : Label(title_text, sf::Vector2f()) {};
  Label(sf::Vector2f position) : Label(std::wstring(), position) {};
  Label(std::wstring title_text, sf::Vector2f position) :
    Control(title_text, position) {};
  ~Label() {};
public:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#pragma once

#include "Control.h"
#include "Config.h"

class Label : public Control {
public:
  Label(std::wstring title) : Label(title, 0, 0) { };
  Label(std::wstring title, unsigned int x, unsigned int y) :
    Control(title, x, y) {};
  ~Label() {};
public:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

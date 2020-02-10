#pragma once

#include "Control.h"

#include "Config.h"

class Checkbox : public Control {
private:
  bool is_checked_ = false;
public:
  Checkbox(std::wstring title_text) : Checkbox(title_text, sf::Vector2f()) { };
  Checkbox(std::wstring title_text, sf::Vector2f position) :
    Control(title_text, position) {
      size_ = sf::Vector2f(kCheckboxDefaultWidth, kCheckboxDefaultHeight);
    }
  void ChangeCheckedState() { is_checked_ = !is_checked_; }
  bool GetCheckedState() { return is_checked_; }
  ~Checkbox() {};
public:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

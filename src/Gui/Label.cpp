#include "Utils/Assets.h"
#include "Label.h"

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();
  
  sf::Text text(titleText, Assets::Instance().font, 20);
  text.setFillColor(sf::Color::Black);

  text.setPosition(position);

  target.draw(text, states);
}

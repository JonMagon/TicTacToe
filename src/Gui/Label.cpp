#include "Label.h"

#include "Utils/Assets.h"

void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();

  sf::Text text(title_text_, Assets::Instance().font, kDefaultFontSize);
  text.setFillColor(sf::Color::Black);

  text.setPosition(position_);

  target.draw(text, states);
}

#include "Utils/Assets.h"
#include "Button.h"

#include <iostream>

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();

  /* Отрисовка контура кнопки */

  sf::RectangleShape shape(sf::Vector2f(size.x, size.y));

  // Выбор цвета в зависимости от состояния
  if (isPressed)
    shape.setFillColor(sf::Color(230, 130, 60));
  else if (isHovered)
    shape.setFillColor(sf::Color(50, 50, 50));
  else
    shape.setFillColor(sf::Color(100, 100, 100));

  shape.setPosition(position);

  target.draw(shape, states);

  /* Отрисовка текста внутри кнопки */

  sf::Text text(titleText, Assets::Instance().font, DEFAULT_FONTSIZE);
  text.setFillColor(sf::Color::White);

  // Вычисление позиции для текста
  float titleX = shape.getPosition().x - text.getLocalBounds().left +
    shape.getSize().x / 2 - text.getGlobalBounds().width / 2,
  titleY = shape.getPosition().y - text.getLocalBounds().top +
    shape.getSize().y / 2 - text.getGlobalBounds().height / 2;

  text.setPosition(titleX, titleY);

  target.draw(text, states);
}

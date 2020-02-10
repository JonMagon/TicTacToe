#include "Checkbox.h"

#include "Utils/Assets.h"

void Checkbox::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();

  /* Отрисовка контура "флажка" */

  sf::RectangleShape shape(size_);
  shape.setOutlineThickness(kLineWidth);
  shape.setOutlineColor(sf::Color::Black);
  shape.setPosition(position_);

  target.draw(shape, states);

  /* Отрисовка "флажка" */

  sf::Vector2f indent(6, 6); // Отступ от границ контура
  sf::RectangleShape flag(size_ - indent);
  // Выбор цвета в зависимости от состояния
  if (is_checked_)
    flag.setFillColor(sf::Color(230, 130, 60));
  else if (is_hovered_)
    flag.setFillColor(sf::Color(50, 50, 50));

  flag.setPosition(
    sf::Vector2f(position_.x + indent.x / 2, position_.y + indent.y / 2)
  );
  target.draw(flag, states);

  /* Отрисовка текста слева */

  sf::Text text(title_text_, Assets::Instance().font, kDefaultFontSize);
  text.setFillColor(sf::Color::Black);

  // Вычисление позиции для текста
  int title_x = shape.getSize().x + 10,
  title_y = shape.getPosition().y - text.getLocalBounds().top +
        shape.getSize().y / 2 - text.getGlobalBounds().height / 2;

  text.setPosition(title_x, title_y);

  target.draw(text, states);
}

#include "Gui.h"
#include "Game/Game.h"
#include "Config.h"
#include "Control.h"
#include "Button.h"
#include "Label.h"

#include <iostream>

Gui::Gui(Game& g) : game(g) {
  Init();
}

void Gui::Init() {
  auto labelStart = std::make_unique<Label>(L"пип");

  auto btnRestart = std::make_shared<Button>(L"Старт", sf::Vector2f(0, labelStart->getPosition().y + 50));
  btnRestart->onClick = [](Game& game) {
    std::cout << "CLICKED" << std::endl;
    game.board[0][0] = StateCell::O;
    game.resizeBoard();
  };

  auto btnDefaultConfig = std::make_unique<Button>(L"Перезапуск", sf::Vector2f(0, btnRestart->getPosition().y + 50));
  btnDefaultConfig->onClick = [](Game& game) {
    game.setCellsCount(game.getCellsCount() + 1);
  };

  //auto labelStart = std::make_unique<Label>(L"Тест", 0, btnRestart->getPosition().y + 100);

  controls.push_back(std::move(labelStart));
  controls.push_back(std::move(btnRestart));
  controls.push_back(std::move(btnDefaultConfig));

}

void Gui::mouseButtonPressed(sf::Vector2f point) {
  for (auto const& control: controls)
    control->setPressed(isPointInside(control, point));
}

void Gui::mouseButtonReleased(sf::Vector2f point) {
  for (auto const& control: controls) {
    if (isPointInside(control, point) &&
      (control->getPressed() && control->onClick))
        control->onClick(game);

    control->setPressed(false);
  }
}

void Gui::mouseMoved(sf::Vector2f point) {
  for (auto const& control: controls)
    control->setHovered(isPointInside(control, point));
}

bool Gui::isPointInside(const std::shared_ptr<Control>& control, sf::Vector2f point) {
  // Преобразование в координаты внутри GUI
  sf::Vector2f pointInContainer = point - getPosition();
  // Получение границ элемента
  float left = control->getPosition().x;
  float right = left + control->getSize().x;
  float top = control->getPosition().y;
  float bottom = top + control->getSize().y;

  return (left <= pointInContainer.x && pointInContainer.x <= right) &&
    (top <= pointInContainer.y && pointInContainer.y <= bottom);
}

void Gui::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();

  if (game.isFinished()) controls[0]->setTitleText(L"Игра завершена");

  // Отрисовка элементов GUI
  for (auto const& control: controls)
    target.draw(*control, states);
}

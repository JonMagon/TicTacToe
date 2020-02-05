#include "Gui.h"

#include "Game/Game.h"

#include "Control.h"
#include "Button.h"
#include "Label.h"

#include "Config.h"

#include <iostream>

Gui::Gui(Game& g) : game_(g) {
  Initialize();
}

void Gui::Initialize() {
  auto label_start = std::make_unique<Label>(L"пип");

  auto btn_restart = std::make_shared<Button>(L"Старт", sf::Vector2f(0, label_start->GetPosition().y + 50));
  btn_restart->OnClick = [](Game& game) {
    std::cout << "CLICKED" << std::endl;
    game.board[0][0] = StateCell::O;
    game.ResizeBoard();
  };

  auto btn_default_config = std::make_unique<Button>(L"Перезапуск", sf::Vector2f(0, btn_restart->GetPosition().y + 50));
  btn_default_config->OnClick = [](Game& game) {
    game.SetCellsCount(game.GetCellsCount() + 1);
  };

  //auto label_start = std::make_unique<Label>(L"Тест", 0, btn_restart->GetPosition().y + 100);

  controls_.push_back(std::move(label_start));
  controls_.push_back(std::move(btn_restart));
  controls_.push_back(std::move(btn_default_config));

}

void Gui::MouseButtonPressed(sf::Vector2f point) {
  for (auto const& control: controls_)
    control->SetPressed(IsPointInside(control, point));
}

void Gui::MouseButtonReleased(sf::Vector2f point) {
  for (auto const& control: controls_) {
    if (IsPointInside(control, point) &&
      (control->GetPressed() && control->OnClick))
        control->OnClick(game_);

    control->SetPressed(false);
  }
}

void Gui::MouseMoved(sf::Vector2f point) {
  for (auto const& control: controls_)
    control->SetHovered(IsPointInside(control, point));
}

bool Gui::IsPointInside(const std::shared_ptr<Control>& control, sf::Vector2f point) {
  // Преобразование в координаты внутри GUI
  sf::Vector2f point_in_container = point - getPosition();
  // Получение границ элемента
  float left = control->GetPosition().x;
  float right = left + control->GetSize().x;
  float top = control->GetPosition().y;
  float bottom = top + control->GetSize().y;

  return (left <= point_in_container.x && point_in_container.x <= right) &&
    (top <= point_in_container.y && point_in_container.y <= bottom);
}

void Gui::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();

  if (game_.IsFinished()) controls_[0]->SetTitleText(L"Игра завершена");

  // Отрисовка элементов GUI
  for (auto const& control: controls_)
    target.draw(*control, states);
}

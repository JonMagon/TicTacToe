#include "Gui.h"

#include "Game/Game.h"

#include "Control.h"
#include "Button.h"
#include "Label.h"
#include "Config.h"

#include <iostream>
#include <string>

Gui::Gui(Game& g) : game_(g) {
  Initialize();
}

void Gui::Initialize() {
  auto label_start = std::make_shared<Label>(
    std::wstring(L"Первый ход делает игрок, ставящий крестики.\n") +
    std::wstring(L"Размер игрового поля: ")
  );

  auto label_field_size = std::make_shared<Label>(
    L"",
    sf::Vector2f(0, 150)
  );

  auto btn_default_config = std::make_shared<Button>(
    L"Стандартные настройки",
    sf::Vector2f(0, 250)
  );
  btn_default_config->OnClick = [](Game& game) {
    game.SetCellsCount(3);
    game.ResizeBoard();
  };

  auto btn_increase_field = std::make_shared<Button>(
    L"Больше",
    sf::Vector2f(0, 300)
  );
  btn_increase_field->OnClick = [](Game& game) {
    if (game.GetCellsCount() < 10) {
      game.SetCellsCount(game.GetCellsCount() + 1);
      game.ResizeBoard();
    }
  };

  auto btn_reduce_field = std::make_shared<Button>(
    L"Меньше",
    sf::Vector2f(150, 300)
  );
  btn_reduce_field->OnClick = [](Game& game) {
    if (game.GetCellsCount() > 3) {
      game.SetCellsCount(game.GetCellsCount() - 1);
      game.ResizeBoard();
    }
  };

  controls_.push_back(std::move(label_start));
  controls_.push_back(std::move(label_field_size));
  controls_.push_back(std::move(btn_default_config));
  controls_.push_back(std::move(btn_increase_field));
  controls_.push_back(std::move(btn_reduce_field));
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

  //if (game_.IsFinished()) controls_[0]->SetTitleText(L"Игра завершена");
  //else if (!game_.IsFinished()) controls_[0]->SetTitleText(L"Игра не завершена");

  // Отрисовка элементов GUI
  for (auto const& control: controls_)
    target.draw(*control, states);
}

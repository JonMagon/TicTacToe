#include "Gui.h"

#include "Game/Game.h"

#include "Control.h"
#include "Button.h"
#include "Label.h"
#include "Checkbox.h"
#include "Config.h"

#include <iostream>
#include <string>

Gui::Gui(Game& g) : game_(g) {
  Initialize();
}

void Gui::Initialize() {
  auto label_start = std::make_shared<Label>(
    std::wstring(L"Игра \"Крестики-нолики\"\n") +
    std::wstring(L"Первый ход делает игрок, ставящий крестики.")
  );

  /* Кнопка сброса на стандартные настройки (Config.h) */
  auto btn_default_config = std::make_shared<Button>(
    L"Стандартный размер поля", sf::Vector2f(0, 50)
  );
  btn_default_config->SetSize(sf::Vector2f(350, kButtonDefaultHeight));
  btn_default_config->OnClick = [](Control& me, Game& game, Gui& gui) {
    game.SetCellsCount(kDefaultFieldSize);
  };

  /* Лейбл для вывода размерности поля */
  auto label_field_size = std::make_shared<Label>(sf::Vector2f(0, 100));

  /* Кнопка увеличения размерности поля */
  auto btn_reduce_field = std::make_shared<Button>(L"-",
    sf::Vector2f(240, label_field_size->GetPosition().y - 5)
  );
  btn_reduce_field->SetSize(sf::Vector2f(50, kButtonDefaultHeight));
  btn_reduce_field->OnClick = [](Control& me, Game& game, Gui& gui) {
    if (game.GetCellsCount() > 3)
      game.SetCellsCount(game.GetCellsCount() - 1);
  };

  /* Кнопка уменьшения размерности поля */
  auto btn_increase_field = std::make_shared<Button>(L"+",
    sf::Vector2f(300, label_field_size->GetPosition().y - 5)
  );
  btn_increase_field->SetSize(sf::Vector2f(50, kButtonDefaultHeight));
  btn_increase_field->OnClick = [](Control& me, Game& game, Gui& gui) {
    if (game.GetCellsCount() < 10)
      game.SetCellsCount(game.GetCellsCount() + 1);
  };

  /* Лейбл для вывода состояния игры */
  auto label_status_game = std::make_shared<Label>(L"", sf::Vector2f(0, 150));

  /* Чекбокс, передащий право первого хода компьютеру */
  auto checkbox_first_ai = std::make_shared<Checkbox>(
    L"Первый ходит компьютер",
    sf::Vector2f(0, 200)
  );
  checkbox_first_ai->OnClick = [](Control& me, Game& game, Gui& gui) {
    Checkbox& me_casted = static_cast<Checkbox&>(me);
    bool is_checked = me_casted.GetCheckedState();
    me_casted.SetCheckedState(!is_checked);
    game.SetFirstTurnAI(!is_checked);
  };

  /* Кнопка перезапуска игры */
  auto btn_restart_game = std::make_shared<Button>(
    L"Перезапуск игры", sf::Vector2f(0, 250)
  );
  btn_restart_game->SetSize(sf::Vector2f(350, kButtonDefaultHeight));
  btn_restart_game->OnClick = [](Control& me, Game& game, Gui& gui) {
    game.ResizeBoard();
  };

  controls_["label_start"] = label_start;
  controls_["btn_default_config"] = btn_default_config;
  controls_["label_field_size"] = label_field_size;
  controls_["btn_reduce_field"] = btn_reduce_field;
  controls_["btn_increase_field"] = btn_increase_field;
  controls_["label_status_game"] = label_status_game;
  controls_["checkbox_first_ai"] = checkbox_first_ai;
  controls_["btn_restart_game"] = btn_restart_game;
}

void Gui::Refresh() {
  /* Обновление размерности поля */
  controls_.find("label_field_size")->second->SetTitleText(
    L"Размер игрового поля: " +
    std::to_wstring(game_.GetCellsCount()) + L"x" +
    std::to_wstring(game_.GetCellsCount())
  );

  /* Обновление состояния игры */
  std::wstring state_of_game_label;
  if (game_.IsFinished()) {
    if (game_.GetWinner() == StateCell::X)
      state_of_game_label = L"Выиграли крестики";
    else if (game_.GetWinner() == StateCell::O)
      state_of_game_label = L"Выиграли нолики";
    else
      state_of_game_label = L"Ничья";
  } else {
    state_of_game_label = L"Игра продолжается";
  }

  controls_.find("label_status_game")->second->SetTitleText(
    L"Состояние игры: " + state_of_game_label
  );
}

void Gui::MouseButtonPressed(sf::Vector2f point) {
  for (const auto& [name, control]: controls_)
    control->SetPressed(IsPointInside(control, point));
}

void Gui::MouseButtonReleased(sf::Vector2f point) {
  for (auto const& [name, control]: controls_) {
    if (IsPointInside(control, point) &&
      (control->GetPressed() && control->OnClick))
        control->OnClick(*control, game_, *this);

    control->SetPressed(false);
  }
}

void Gui::MouseMoved(sf::Vector2f point) {
  for (auto const& [name, control]: controls_)
    control->SetHovered(IsPointInside(control, point));
}

bool Gui::IsPointInside(const std::shared_ptr<Control>& control,
                        sf::Vector2f point) {
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

  // Отрисовка элементов GUI
  for (auto const& [name, control]: controls_)
    target.draw(*control, states);
}

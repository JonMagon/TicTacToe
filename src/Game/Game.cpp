#include "Game.h"

#include "Lookup.h"
#include "Config.h"

#include <math.h>

Game::Game(Lookup& l) : lookup_(l) {
  SetCellsCount(kDefaultFieldSize);
  ResizeBoard();
}

bool Game::IsFinished() {
  return lookup_.IsGameDone(board);
}

void Game::ResizeBoard() {
  cell_size_ = kFieldSizePx / cells_count_;
  lookup_.InitializeWinningStates(cells_count_);
  win_state_ =
    std::make_pair(StateCell::None, std::vector<std::pair<int, int>>());
  board.clear();
  board.resize(cells_count_);
  for(int i = 0 ; i < cells_count_ ; ++i)
    board[i].resize(cells_count_);

  if (is_first_turn_ai_) {
    lookup_.player_marker = StateCell::O;
    lookup_.ai_marker = StateCell::X;
    board[cells_count_ / 2][cells_count_ / 2] = lookup_.ai_marker;
  } else {
    lookup_.player_marker = StateCell::X;
    lookup_.ai_marker = StateCell::O;
  }
}

void Game::MouseButtonPressed(sf::Vector2f point) {
  if (IsFinished()) return;

  sf::Vector2f point_in_container = point - getPosition();
  // Вычисление текущей ячейки
  unsigned int column = (point_in_container.x) / (kFieldSizePx / board.size());
  unsigned int row = (point_in_container.y) / (kFieldSizePx / board.size());

  // Изменение статуса ячейки
  if ((row < cells_count_ && column < cells_count_) &&
      (board[row][column] == StateCell::None)) {

    board[row][column] = lookup_.player_marker;

    std::pair<int, std::pair<int, int>> ai_move =
      lookup_.MinimaxOptimization(board, lookup_.ai_marker, 0, kLoss, kWin);

    if (ai_move.second.first != -1 && ai_move.second.second != -1)
      board[ai_move.second.first][ai_move.second.second] = lookup_.ai_marker;

    win_state_ = lookup_.GetWinningPosition(board);
  }
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();

  /* Отрисовка игрового поля */

  for (unsigned int i = 1; i < board.size(); i++) {
    // Горизонтальная линия
    sf::Vector2f position(0, cell_size_ * i - kLineWidth / 2);

    sf::RectangleShape line(
      sf::Vector2f(cell_size_ * board.size(), kLineWidth)
    );
    line.setFillColor(sf::Color::Black);
    line.setPosition(position);

    target.draw(line, states);

    // Вертикальная линия
    line.rotate(90);

    position.x = position.y;
    position.y = 0;
    line.setPosition(position);

    target.draw(line, states);
  }

  /* Отрисовка состояний ячеек */

  for (unsigned int i = 0; i < board.size(); i++) {
    for (unsigned int j = 0; j < board.size(); j++) {
      switch (board[i][j]) {
        case StateCell::X: {
          // Вычисление смещений по координатам
          float cathet = cell_size_ / sqrt(2);
          float indent = cell_size_ / 2 - cathet / 2;
          sf::RectangleShape line(sf::Vector2f(cell_size_, kMarkerWidth));
          line.setFillColor(sf::Color::Blue);
          line.rotate(45);
          line.setPosition(
            sf::Vector2f(cell_size_ * j + indent,
                         cell_size_ * i + indent - kMarkerWidth / 2)
          );
          target.draw(line, states);
          line.rotate(90);
          line.setPosition(
            sf::Vector2f(cell_size_ * j + cathet + indent,
                         cell_size_ * i + indent + kMarkerWidth / 2)
          );
          target.draw(line, states);
          break;
        }
        case StateCell::O: {
          float radius = cell_size_ / M_PI;
          sf::CircleShape circle;
          circle.setRadius(radius);
          circle.setOutlineColor(sf::Color::Red);
          circle.setOutlineThickness(kMarkerWidth);
          circle.setPosition(
            sf::Vector2f(cell_size_ * j + radius / 2,
                         cell_size_ * i + radius / 2)
          );
          target.draw(circle, states);
          break;
        }
      }
    }
  }

  /* Отрисовка победных состояний */

  if (win_state_.first != StateCell::None) {
    // Преобразование победного состояния в координаты
    sf::Vector2f position_begin(
      cell_size_ * win_state_.second[0].second,
      cell_size_ * win_state_.second[0].first
    );
    sf::Vector2f position_end(
      cell_size_ * (win_state_.second[board.size() - 1].second + 1),
      cell_size_ * (win_state_.second[board.size() - 1].first + 1)
    );

    if (win_state_.second[0].second ==
        win_state_.second[board.size() - 1].second) {
      position_begin += sf::Vector2f(cell_size_ / 2, 0);
      position_end -= sf::Vector2f(cell_size_ / 2, 0);
    } else if (win_state_.second[0].first ==
        win_state_.second[board.size() - 1].first) {
      position_begin += sf::Vector2f(0, cell_size_ / 2);
      position_end -= sf::Vector2f(0, cell_size_ / 2);
    } else if (win_state_.second[0].first ==
        win_state_.second[board.size() - 1].second) {
        position_begin += sf::Vector2f(0, cell_size_);
        position_end -= sf::Vector2f(0, cell_size_);
    }

    sf::RectangleShape line(
      sf::Vector2f(
        sqrt(
          pow(position_end.x - position_begin.x, 2) +
          pow(position_end.y - position_begin.y, 2)
        ),
        kMarkerWidth)
    );
    line.setFillColor(
      win_state_.first == StateCell::O ? sf::Color::Red : sf::Color::Blue
    );
    line.setPosition(position_begin);

    // Поворот "зачёркивания" на соответствующий угол
    float rad = atan2(position_end.y - position_begin.y,
      position_end.x - position_begin.x);

    line.rotate(180 / M_PI * rad);

    target.draw(line, states);
  }
}

#include "Game.h"

#include "Lookup.h"
#include "Config.h"

#include <math.h>

Game::Game(Lookup& l) : lookup_(l) {
  SetCellsCount(kDefaultFieldSize);
}

void Game::SetCellsCount(unsigned int count) {
  cells_count_ = count;
  cell_size_ = kFieldSizePx / cells_count_;
  ResizeBoard();

  lookup_.InitializeWinningStates(count);
}

unsigned int Game::GetCellsCount() {
  return cells_count_;
}

bool Game::IsFinished() {
  return lookup_.IsGameDone(board);
}

void Game::ResizeBoard() {
  board.clear();
  board.resize(cells_count_);
  for(int i = 0 ; i < cells_count_ ; ++i)
    board[i].resize(cells_count_);
}

void Game::MouseButtonPressed(sf::Vector2f point) {
  if (IsFinished()) return;

  // Вычисление текущей ячейки
  unsigned int column = (point.x - kIndentFieldX) / (kFieldSizePx / cells_count_);
  unsigned int row = (point.y - kIndentFieldY) / (kFieldSizePx / cells_count_);

  // Изменение статуса ячейки
  if ((row < cells_count_ && column < cells_count_) &&
      (board[row][column] == StateCell::None)) {

    board[row][column] = lookup_.player_marker;

    std::pair<int, std::pair<int, int>> ai_move =
      lookup_.MinimaxOptimization(board, lookup_.ai_marker, 0, kLoss, kWin);

    if (ai_move.second.first != -1 && ai_move.second.second != -1)
      board[ai_move.second.first][ai_move.second.second] =
        lookup_.ai_marker;
  }
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();

  /* Отрисовка игрового поля */

  for (unsigned int i = 1; i < cells_count_; i++) {
    // Горизонтальная линия
    sf::Vector2f position(0, cell_size_ * i - kLineWidth / 2);

    sf::RectangleShape line(
      sf::Vector2f(cell_size_ * cells_count_, kLineWidth)
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

  for (unsigned int i = 0; i < cells_count_; i++) {
    for (unsigned int j = 0; j < cells_count_; j++) {
      switch (board[i][j]) {
        case StateCell::X: {
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

  sf::Vector2i win_pos_begin(0, 0), win_pos_end(2, 2);

  sf::Vector2f position_begin(cell_size_ * win_pos_begin.y + cell_size_ / 2,
    cell_size_ * win_pos_begin.x + cell_size_ / 2);
  sf::Vector2f position_end(cell_size_ * win_pos_end.y + cell_size_ / 2,
    cell_size_ * win_pos_end.x + cell_size_ / 2);

  sf::RectangleShape line(
    sf::Vector2f(
      sqrt(
        pow(position_end.x - position_begin.x, 2) +
        pow(position_end.y - position_begin.y, 2)
      ),
      kLineWidth)
  );
  line.setFillColor(sf::Color::Green);
  line.setPosition(position_begin);

  float rad = atan2(position_end.y - position_begin.y,
    position_end.x - position_begin.x);

  line.rotate(180 / M_PI * rad);

  target.draw(line, states);
}

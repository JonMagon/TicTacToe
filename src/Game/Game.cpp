#include "Game.h"
#include "Config.h"

#include <iostream>
#include <math.h>

#include "Lookup.h"

Game::Game(Lookup& l) : lookup(l) {
  setCellsCount(DEFAULT_FIELD_SIZE);
}

void Game::setCellsCount(unsigned int count) {
  cellsCount = count;
  cellSize = FIELD_SIZE_PX / cellsCount;
  resizeBoard();

  lookup.fillWinningStates(count);
}

unsigned int Game::getCellsCount() {
  return cellsCount;
}

bool Game::isFinished() {
  return lookup.isGameDone(board);
}

void Game::resizeBoard() {
  board.resize(cellsCount);
  for(int i = 0 ; i < cellsCount ; ++i)
    board[i].resize(cellsCount);
}

StateCell player = StateCell::X, opponent = StateCell::O;

void Game::mouseButtonPressed(sf::Vector2f point) {
  if (isFinished()) return;

  // Вычисление текущей ячейки
  unsigned int column = (point.x - INDENT_FIELD_X) / (FIELD_SIZE_PX / cellsCount);
  unsigned int row = (point.y - INDENT_FIELD_Y) / (FIELD_SIZE_PX / cellsCount);

  // Изменение статуса ячейки
  if ((row < cellsCount && column < cellsCount) &&
      (board[row][column] == StateCell::None)) {

    board[row][column] = player;

    std::pair<int, std::pair<int, int>> aiMove =
      lookup.minimaxOptimization(board, opponent, 0, LOSS, WIN);

    if (aiMove.second.first != -1 && aiMove.second.second != -1)
      board[aiMove.second.first][aiMove.second.second] = opponent;
  }
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();

  sf::Color color = sf::Color(200, 100, 200);
  sf::Color None = sf::Color(54, 90, 173);
  sf::Color X = sf::Color(200, 100, 200);
  sf::Color O = sf::Color(50, 30, 255);

  /* Отрисовка игрового поля */

  for (unsigned int i = 1; i < cellsCount; i++) {
    // Горизонтальная линия
    sf::Vector2f position(0, cellSize * i - LINE_WIDTH / 2);

    sf::RectangleShape line(sf::Vector2f(cellSize * cellsCount, LINE_WIDTH));
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

  for (unsigned int i = 0; i < cellsCount; i++) {
    for (unsigned int j = 0; j < cellsCount; j++) {
      switch (board[i][j]) {
        case StateCell::X: {
          sf::Vector2f position(FIELD_SIZE_PX / cellsCount * j, FIELD_SIZE_PX / cellsCount * i);

          sf::RectangleShape shape(sf::Vector2f(40, 40));
          shape.setOutlineThickness(2.f);
          shape.setOutlineColor(X);
          //shape.setFillColor(sf::Color::Transparent);
          shape.setPosition(position);
          target.draw(shape, states);
          break;
        }
        case StateCell::O: {
          float radius = cellSize / M_PI;
          sf::Vector2f position(cellSize * j+ radius / 2, cellSize * i+ radius/2);
          sf::CircleShape circle;
          circle.setRadius(radius);
          circle.setOutlineColor(sf::Color::Red);
          circle.setOutlineThickness(5);
          circle.setPosition(position);
          target.draw(circle, states);
          break;
        }
      }
    }
  }
}

#include "Game.h"
#include "Config.h"

#include <iostream>
#include <math.h>

Game::Game() {
  setCellsCount(DEFAULT_FIELD_SIZE);
}

void Game::setCellsCount(unsigned int count) {
  cellsCount = count;
  cellSize = FIELD_SIZE_PX / cellsCount;
  resizeMatrix();
}

unsigned int Game::getCellsCount() {
  return cellsCount;
}

void Game::resizeMatrix() {
  matrix.resize(cellsCount);
  for(int i = 0 ; i < cellsCount ; ++i)
    matrix[i].resize(cellsCount);

  matrix[1][2] = StateCell::X;
}

void Game::mouseButtonPressed(sf::Vector2f point) {
  // Вычисление текущей ячейки
  unsigned int column = (point.x - INDENT_FIELD_X) / (FIELD_SIZE_PX / cellsCount);
  unsigned int row = (point.y - INDENT_FIELD_Y) / (FIELD_SIZE_PX / cellsCount);

  // Изменение статуса ячейки
  if ((row < cellsCount && column < cellsCount) &&
      (matrix[row][column] == StateCell::None))
    matrix[row][column] = StateCell::O;
}

void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform *= getTransform();

  sf::Color color = sf::Color(200, 100, 200);
  sf::Color None = sf::Color(54, 90, 173);
  sf::Color X = sf::Color(200, 100, 200);
  sf::Color O = sf::Color(50, 30, 255);

  // Рамка игрового поля
  /*sf::RectangleShape shape(sf::Vector2f(FIELD_SIZE, FIELD_SIZE));
  shape.setOutlineThickness(2.f);
  shape.setOutlineColor(color);
  shape.setFillColor(sf::Color::Transparent);
  target.draw(shape, states);*/

  // Отрисовка игрового поля
  for (unsigned int i = 1; i < cellsCount; i++) {
    // Горизонтальную линию
    sf::Vector2f position(0, cellSize * i - LINE_WIDTH / 2);

    sf::RectangleShape line(sf::Vector2f(FIELD_SIZE_PX, LINE_WIDTH));
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

  // Отрисовка ячеек
  for (unsigned int i = 0; i < cellsCount; i++) {
    for (unsigned int j = 0; j < cellsCount; j++) {
      switch (matrix[i][j]) {
        case StateCell::X: {
          sf::Vector2f position(FIELD_SIZE_PX / cellsCount * j, FIELD_SIZE_PX / cellsCount * i);

          sf::RectangleShape shape(sf::Vector2f(40, 40));
          shape.setOutlineThickness(2.f);
          shape.setOutlineColor(X);
          shape.setFillColor(sf::Color::Transparent);
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

#include <SFML/Graphics.hpp>

#include "Utils/Assets.h"
#include "Game/Game.h"
#include "Game/Lookup.h"
#include "Gui/Gui.h"
#include "Config.h"

// Приведение "спроектированного" размера к размеру окна
sf::View CalcView(const sf::Vector2u& window_size,
                  const sf::Vector2u& designed_size) {
  sf::FloatRect viewport(0.f, 0.f, 1.f, 1.f);

  float screen_width = window_size.x / static_cast<float>(designed_size.x);
  float screen_height = window_size.y / static_cast<float>(designed_size.y);

  if (screen_width > screen_height) {
    viewport.width = screen_height / screen_width;
    viewport.left = (1.f - viewport.width) / 2.f;
  }
  else if (screen_width < screen_height) {
    viewport.height = screen_width / screen_height;
    viewport.top = (1.f - viewport.height) / 2.f;
  }

  sf::View view(sf::FloatRect(0, 0, designed_size.x, designed_size.y));
  view.setViewport(viewport);

  return view;
}

int main() {
    // Загрузка ресурсов
    Assets::Instance().Load();

    // Спроектированный размер
    const sf::Vector2u designed_size(kWindowWidth, kWindowHeight);

    /* Создание окна игры */

    // Сглаживание
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(kWindowWidth, kWindowHeight),
      "Tic-tac-toe", sf::Style::Default, settings);
    window.setView(CalcView(window.getSize(), designed_size));
    window.setFramerateLimit(60);

    // Создаем объект игры
    Lookup lookup;
    Game game(lookup);
    game.setPosition(kIndentFieldX, kIndentFieldY);

    Gui gui(game);
    gui.setPosition(kFieldSizePx + 2 * kIndentFieldX, kIndentFieldY);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
          sf::Vector2f point =
            window.mapPixelToCoords(sf::Mouse::getPosition(window));
          switch (event.type)
          {
            case sf::Event::Closed:
              window.close();
              break;
            case sf::Event::KeyPressed:
              if (event.key.code == sf::Keyboard::Escape) window.close();
              break;
            case sf::Event::Resized: {
              window.setView(
                CalcView(sf::Vector2u(event.size.width, event.size.height),
                  designed_size)
              );
              break;
            }
            case sf::Event::MouseButtonPressed: {
              if (event.mouseButton.button == sf::Mouse::Left) {
                // Нажатие произошло внутри игрового поля
                if (game.IsPointInGameField(point))
                  game.MouseButtonPressed(point);
                else // В противном случае - в гуи
                  gui.MouseButtonPressed(point);
              }
              break;
            }
            case sf::Event::MouseButtonReleased: {
              if (event.mouseButton.button == sf::Mouse::Left &&
                  !game.IsPointInGameField(point)) {
                  gui.MouseButtonReleased(point);
              }
              break;
            }
            case sf::Event::MouseMoved: {
                // Курсор перемещён внутри игрового поля
                if (game.IsPointInGameField(point))
                  game.MouseMoved(point);
                else // В противном случае - в гуи
                  gui.MouseMoved(point);
                break;
              }
            default:
              break;
          }
        }

        // Обновление состояния
        gui.Refresh();

        // Отрисовка игрового поля и GUI
        window.clear(sf::Color::White);
        window.draw(game);
        window.draw(gui);
        window.draw(Assets::Instance().logo);
        window.display();
    }

    return 0;
}

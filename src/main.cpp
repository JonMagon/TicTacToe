#include <SFML/Graphics.hpp>

#include "Utils/Assets.h"
#include "Game/Game.h"
#include "Game/Lookup.h"
#include "Gui/Gui.h"
#include "Gui/Button.h"
#include "Config.h"

sf::View calcView(const sf::Vector2u& window_size, const sf::Vector2u& designed_size) {
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

bool IsInGameField(sf::Vector2f point) {
  return (point.x >= kIndentFieldX && point.x <= kIndentFieldX + kFieldSizePx) &&
    (point.y >= kIndentFieldY && point.y <= kIndentFieldY + kFieldSizePx);
}


int main() {
    // Загрузка ресурсов
    Assets::Instance().Load();

    const sf::Vector2u designed_size(1000,600);

    // Создаем окно размером 600 на 600 и частотой обновления 60 кадров в секунду

    sf::RenderWindow window(sf::VideoMode(1000, 600), "Tic-tac-toe");
    window.setView(calcView(window.getSize(), designed_size));
    window.setFramerateLimit(60);

    // Создаем объект игры
    Lookup lookup;

    Game game(lookup);
    game.setPosition(50.f, 50.f);

    Gui gui(game);

    gui.setPosition(kFieldSizePx + 100.f, 50.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
          sf::Vector2f point = window.mapPixelToCoords(sf::Mouse::getPosition(window));
          switch (event.type)
          {
            case sf::Event::Closed:
              window.close();
              break;
            case sf::Event::KeyPressed:
              if (event.key.code == sf::Keyboard::Escape) window.close();
              break;
            case sf::Event::Resized: {
              window.setView(calcView(sf::Vector2u(event.size.width, event.size.height), designed_size));
              break;
            }
            case sf::Event::MouseButtonPressed: {
              if (event.mouseButton.button == sf::Mouse::Left) {
                // Нажатие произошло внутри игрового поля
                if (IsInGameField(point))
                  game.MouseButtonPressed(point);
                else // В противном случае - в гуи
                  gui.MouseButtonPressed(point);
              }
              break;
            }
            case sf::Event::MouseButtonReleased: {
              if (event.mouseButton.button == sf::Mouse::Left &&
                  !IsInGameField(point)) {
                  gui.MouseButtonReleased(point);
              }
              break;
            }
            case sf::Event::MouseMoved: {
                // Курсор перемещён внутри игрового поля
                if (IsInGameField(point))
                  game.MouseMoved(point);
                else // В противном случае - в гуи
                  gui.MouseMoved(point);
                break;
              }
            default:
              break;
          }
        }

        // Отрисовка игрового поля и GUI
        window.clear(sf::Color::White);
        window.draw(game);
        window.draw(gui);
        window.display();
    }

    return 0;
}

#include <SFML/Graphics.hpp>

#include "Utils/Assets.h"
#include "Game.h"
#include "Gui/Gui.h"
#include "Gui/Button.h"
#include "Config.h"

sf::View calcView(const sf::Vector2u& windowsize, const sf::Vector2u& designedsize) {
  sf::FloatRect viewport(0.f, 0.f, 1.f, 1.f);

  float screenwidth = windowsize.x / static_cast<float>(designedsize.x);
  float screenheight = windowsize.y / static_cast<float>(designedsize.y);

  if (screenwidth > screenheight) {
    viewport.width = screenheight / screenwidth;
    viewport.left = (1.f - viewport.width) / 2.f;
  }
  else if (screenwidth < screenheight) {
    viewport.height = screenwidth / screenheight;
    viewport.top = (1.f - viewport.height) / 2.f;
  }

  sf::View view(sf::FloatRect(0, 0, designedsize.x, designedsize.y));
  view.setViewport(viewport);

  return view;
}

bool isInGameField(sf::Vector2f point) {
  return (point.x >= INDENT_FIELD_X && point.x <= INDENT_FIELD_X + FIELD_SIZE_PX) &&
    (point.y >= INDENT_FIELD_Y && point.y <= INDENT_FIELD_Y + FIELD_SIZE_PX);
}


int main() {
    // Загрузка ресурсов
    Assets::Instance().Load();

   const sf::Vector2u designedsize(800,600);

    // Создаем окно размером 600 на 600 и частотой обновления 60 кадров в секунду

    sf::RenderWindow window(sf::VideoMode(800, 600), "Tic-tac-toe");
    window.setView(calcView(window.getSize(), designedsize));
    window.setFramerateLimit(60);

    // Создаем объект игры
    Game game;
    game.setPosition(50.f, 50.f);

    Gui gui(game);

    gui.setPosition(FIELD_SIZE_PX + 100.f, 50.f);

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
              window.setView(calcView(sf::Vector2u(event.size.width, event.size.height), designedsize));
              break;
            }
            case sf::Event::MouseButtonPressed: {
                // Нажатие произошло внутри игрового поля
                if (isInGameField(point))
                  game.mouseButtonPressed(point);
                else // В противном случае - в гуи
                  gui.mouseButtonPressed(point);
                break;
              }
            case sf::Event::MouseButtonReleased: {
              if (!isInGameField(point))
                gui.mouseButtonReleased(point);
              break;
            }
            case sf::Event::MouseMoved: {
                // Курсор перемещён внутри игрового поля
                if (isInGameField(point))
                  game.mouseMoved(point);
                else // В противном случае - в гуи
                  gui.mouseMoved(point);
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

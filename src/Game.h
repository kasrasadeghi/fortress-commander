#include <SFML/Graphics.hpp>

class World {
public:
  
};

class Game {
  constexpr static float tile_size = 1.f;
  sf::View _view;
  sf::Clock _clock;
  sf::RenderWindow _window;
  World _world;

public:
  Game();

  void loop();
  void handleEvent(const sf::Event& event);

  void handleViewInput(const sf::Time& dt);
};

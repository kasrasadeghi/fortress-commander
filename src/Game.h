#include <SFML/Graphics.hpp>

enum class Tile { GRASS };

class World : public sf::Drawable {
  std::vector<std::vector<Tile>> _region;

  void _region_draw(sf::RenderTarget &window) const;

public:
  constexpr static float tile_size = 1.f;

  World() : _region(50, std::vector<Tile>(50, Tile::GRASS)) {}

  virtual void draw(sf::RenderTarget &rw, sf::RenderStates states) const {
    _region_draw(rw);
  };
};

class Game {
  sf::View _view;
  sf::Clock _clock;
  sf::RenderWindow _window;
  World _world;

public:
  Game();

  void loop();
  void handleEvent(const sf::Event &event);

  void handleViewInput(const sf::Time &dt);
};

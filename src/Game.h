#include <SFML/Graphics.hpp>

enum class Tile { GRASS };

class World : public sf::Drawable {
  std::vector<std::vector<Tile>> _region; // this should be a square

  void _region_draw(sf::RenderTarget& window) const;

public:
  constexpr static float tile_size = 1.f;

  World(size_t size) : _region(size, std::vector<Tile>(size, Tile::GRASS)) {}
  size_t size() const { return _region.size(); }

  virtual void draw(sf::RenderTarget& rw, sf::RenderStates states) const {
    _region_draw(rw);
  };
};

class Game {
  World _world;
  sf::View _view;
  sf::RenderWindow _window;
  sf::Clock _clock;

public:
  constexpr static int view_size = 10;
  static float widthScalingFactor() {
    return 1.f * sf::VideoMode::getFullscreenModes()[0].width /
           sf::VideoMode::getFullscreenModes()[0].height;
  }
  Game();

  void loop();
  void handleEvent(const sf::Event& event);

  void handleViewInput(const sf::Time& dt);
};

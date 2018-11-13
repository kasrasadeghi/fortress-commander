#include <SFML/Graphics.hpp>

enum class Tile { GRASS, WATER };

class World : public sf::Drawable {
  std::vector<std::vector<Tile>> _region; // this should be a square

  void _region_draw(sf::RenderTarget& window) const;

public:
  constexpr static float tile_size = 1.f;

  World(size_t size) : _region(size, std::vector<Tile>(size, Tile::GRASS)) {}
  size_t size() const { return _region.size(); }

  Tile flipCell(int i, int j) {
    return _region[i][j] = (_region[i][j] == Tile::GRASS) ? Tile::WATER : Tile::GRASS;
  }

  void setCell(int i, int j, Tile t) {
    _region[i][j] = t;
  }

  virtual void draw(sf::RenderTarget& rw, sf::RenderStates states) const {
    _region_draw(rw);
  };
};

class Game {
  World _world;
  sf::View _view;
  sf::RenderWindow _window;
  sf::Clock _clock;
  Tile _paint = Tile::GRASS;

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

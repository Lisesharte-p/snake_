#ifndef SFML_RENDERER_HPP
#define SFML_RENDERER_HPP

#include <SFML/Graphics.hpp>
#include "game.hpp"
#include<SFML/Main.hpp>
class SFMLRenderer {
public:
    SFMLRenderer(unsigned int window_width, unsigned int window_height);
    ~SFMLRenderer();
    
    void render(const game_t* game, bool paused);

    void render_map_preview(const game_t* game, sf::RenderTarget& target, const sf::FloatRect& bounds);

    sf::RenderWindow& get_window() { return window; }
    
private:
    sf::RenderWindow window;
    unsigned int cell_size;
    sf::Font font;
    sf::Text paused_text;
    sf::Text info_text;
    
    void draw_cell(int row, int col, char c,float cell_size);

    sf::Color get_color_for_char(char c);
    
    void initialize_text();
};

#endif
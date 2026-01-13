
#include "render.hpp"
#include <string>
sf::Font font_global;
SFMLRenderer::SFMLRenderer(unsigned int window_width, unsigned int window_height) 
    : window(sf::VideoMode(sf::Vector2(window_width, window_height)), std::string("Snake Game SFML")) ,paused_text(font_global,"PAUSE"),info_text(font_global,"this is info"){
    
    cell_size = std::min(window_width / 40, window_height / 30);
    

        
    font.openFromFile("/usr/share/fonts/gnu-free/FreeMonoBold.otf");

    
    initialize_text();
}

SFMLRenderer::~SFMLRenderer() {
    window.close();
}

void SFMLRenderer::initialize_text() {
    paused_text.setFont(font);
    paused_text.setString("PAUSED - Press SPACE to continue");
    paused_text.setCharacterSize(24);
    paused_text.setFillColor(sf::Color::Red);
    paused_text.setStyle(sf::Text::Bold);
    
    info_text.setFont(font);
    info_text.setCharacterSize(16);
    info_text.setFillColor(sf::Color::White);
}

void SFMLRenderer::draw_cell(int row, int col, char c) {
    sf::RectangleShape cell(sf::Vector2f(cell_size - 1, cell_size - 1));
    sf::Vector2f pos(col * cell_size, row * cell_size);
    cell.setPosition(pos);
    cell.setFillColor(get_color_for_char(c));
    cell.setOutlineThickness(1);
    cell.setOutlineColor(sf::Color(50, 50, 50));
    window.draw(cell);
}

sf::Color SFMLRenderer::get_color_for_char(char c) {
    switch (c) {
        case '#': return sf::Color(100, 100, 100); // 墙
        case '*': return sf::Color(255, 0, 0);     // 食物
        case 'x': return sf::Color(128, 0, 0);     // 死蛇
        
        // 头
        case 'A': case 'W': case 'S': case 'D': 
            return sf::Color(0, 255, 100);
            
        // 身
        case '<': case '^': case 'v': case '>':
            return sf::Color(0, 200, 50);
            
        // 尾巴
        case 'a': case 'w': case 's': case 'd':
            return sf::Color(0, 150, 0);
            
        default: return sf::Color(0, 0, 0);      
    }
}

void SFMLRenderer::render(const game_t* game, bool paused) {
    window.clear(sf::Color(0, 0, 0));
    
    if (!game || !game->board) return;
    
  
    for (unsigned int i = 0; i < game->num_rows; i++) {
        for (unsigned int j = 0; j < game->num_columns; j++) {
            draw_cell(i, j, game->board[i][j]);
        }
    }
    
    sf::FloatRect bounds = paused_text.getLocalBounds();
    sf::Vector2f pos1((window.getSize().x - bounds.size.x) / 2, 50);
    if (paused) {
        paused_text.setPosition(
            pos1
        );
        window.draw(paused_text);
    }
    
   
    int live_snakes = 0;
    for (unsigned int i = 0; i < game->num_snakes; i++) {
        if (game->snakes[i].live) live_snakes++;
    }
    
    info_text.setString("Snakes: " + std::to_string(live_snakes) + 
                       " | Food: * | Walls: #");
    sf::Vector2f pos(10, window.getSize().y - 30);
    info_text.setPosition(pos);
    window.draw(info_text);
    
    window.display();
}

void SFMLRenderer::render_map_preview(const game_t* game, sf::RenderTarget& target, const sf::FloatRect& bounds) {
    if (!game || !game->board) return;
    
    float preview_cell_size = std::min(
        bounds.size.x / game->num_columns,
        bounds.size.y / game->num_rows
    );
    
    sf::RectangleShape background(sf::Vector2f(bounds.size.x, bounds.size.y));
    background.setPosition(sf::Vector2f(bounds.position.x, bounds.position.y));
    background.setFillColor(sf::Color(20, 20, 20));
    target.draw(background);
    
    for (unsigned int i = 0; i < game->num_rows; i++) {
        for (unsigned int j = 0; j < game->num_columns; j++) {
            sf::RectangleShape cell(sf::Vector2f(preview_cell_size - 1, preview_cell_size - 1));
            cell.setPosition(sf::Vector2f(
                bounds.position.x + j * preview_cell_size,
                bounds.position.y + i * preview_cell_size
            ));
            cell.setFillColor(get_color_for_char(game->board[i][j]));
            target.draw(cell);
        }
    }
}
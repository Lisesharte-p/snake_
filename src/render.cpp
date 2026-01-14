#include "render.hpp"
#include <string>
#include <algorithm>

sf::Font font_global;

SFMLRenderer::SFMLRenderer(unsigned int window_width, unsigned int window_height) 
    : window(sf::VideoMode(sf::Vector2(window_width, window_height)), std::string("Snake Game SFML")),
      paused_text(font_global, "PAUSE"), info_text(font_global, "this is info") {
    
    font.openFromFile("/usr/share/fonts/truetype/jetbrains-mono/JetBrainsMono-Bold.ttf");
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

void SFMLRenderer::draw_cell(int row, int col, char c, float cell_size) {
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
    
    // 动态计算格子大小，基于地图规模和窗口大小
    float cell_size_x = static_cast<float>(window.getSize().x) / game->num_columns;
    float cell_size_y = static_cast<float>(window.getSize().y) / game->num_rows;
    float cell_size = std::min(cell_size_x, cell_size_y);
    
    // 计算地图在窗口中的居中位置
    float map_width = cell_size * game->num_columns;
    float map_height = cell_size * game->num_rows;
    float offset_x = (window.getSize().x - map_width) / 2.0f;
    float offset_y = (window.getSize().y - map_height) / 2.0f;
    
    // 绘制地图
    for (unsigned int i = 0; i < game->num_rows; i++) {
        for (unsigned int j = 0; j < game->num_columns; j++) {
            // 创建一个临时函数用于绘制带偏移的格子
            sf::RectangleShape cell(sf::Vector2f(cell_size - 1, cell_size - 1));
            sf::Vector2f pos(offset_x + j * cell_size, offset_y + i * cell_size);
            cell.setPosition(pos);
            cell.setFillColor(get_color_for_char(game->board[i][j]));
            cell.setOutlineThickness(1);
            cell.setOutlineColor(sf::Color(50, 50, 50));
            window.draw(cell);
        }
    }
    
    // 绘制暂停文本
    if (paused) {
        sf::FloatRect bounds = paused_text.getLocalBounds();
        sf::Vector2f pos1((window.getSize().x - bounds.size.x) / 2, 20);
        paused_text.setPosition(pos1);
        window.draw(paused_text);
    }
    
    // 绘制游戏信息
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
    
    // 计算预览地图在预览区域中的居中位置
    float preview_width = preview_cell_size * game->num_columns;
    float preview_height = preview_cell_size * game->num_rows;
    float offset_x = bounds.position.x + (bounds.size.x - preview_width) / 2.0f;
    float offset_y = bounds.position.y + (bounds.size.y - preview_height) / 2.0f;
    
    for (unsigned int i = 0; i < game->num_rows; i++) {
        for (unsigned int j = 0; j < game->num_columns; j++) {
            sf::RectangleShape cell(sf::Vector2f(preview_cell_size - 1, preview_cell_size - 1));
            cell.setPosition(sf::Vector2f(
                offset_x + j * preview_cell_size,
                offset_y + i * preview_cell_size
            ));
            cell.setFillColor(get_color_for_char(game->board[i][j]));
            target.draw(cell);
        }
    }
}

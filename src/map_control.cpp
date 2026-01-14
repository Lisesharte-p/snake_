#include "map_control.hpp"
#include <iostream>

namespace fs = std::filesystem;

MapSelector::MapSelector(GameController& controller, SFMLRenderer& renderer)
    : game_controller(controller), renderer(renderer), selected_index(0), active(true) {
}

MapSelector::~MapSelector() {
    cleanup_previews();
}

void MapSelector::scan_maps(const std::string& directory) {
    maps.clear();
    
    if (!fs::exists(directory)) {
        std::cerr << "Directory " << directory << " does not exist" << std::endl;
        return;
    }
    
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".snk") {
            MapInfo info;
            info.filename = entry.path().string();
            info.display_name = entry.path().stem().string();
            info.preview_game = nullptr;
            maps.push_back(info);
        }
    }
    
    // 加载第一个预览
    if (!maps.empty()) {
        load_preview(0);
    }
}

void MapSelector::load_preview(size_t index) {
    if (index >= maps.size()) return;
    
    // 清理旧的预览
    if (maps[index].preview_game) {
        delete maps[index].preview_game;
    }
    maps[index].preview_game = nullptr;
    
    // 加载新预览
    FILE* fp = fopen(maps[index].filename.c_str(), "r");
    if (fp) {
        maps[index].preview_game = load_board(fp);
        fclose(fp);
    }
}

void MapSelector::cleanup_previews() {
    for (auto& map : maps) {
        if (map.preview_game) {
            delete map.preview_game;
            map.preview_game = nullptr;
        }
    }
}

void MapSelector::handle_event(const sf::Event& event) {
    if (!active) return;

    const auto* key_pressed = event.getIf<sf::Event::KeyPressed>();
    if (!key_pressed) return;

    switch (key_pressed->code) {
        case sf::Keyboard::Key::Up:
            if (selected_index > 0) {
                selected_index--;
                load_preview(selected_index);
            }
            break;
        case sf::Keyboard::Key::Down:
            if (selected_index < maps.size() - 1) {
                selected_index++;
                load_preview(selected_index);
            }
            break;
        case sf::Keyboard::Key::Enter:
            if (!maps.empty()) {
                game_controller.load_map(maps[selected_index].filename);
                active = false;
            }
            break;
        case sf::Keyboard::Key::Escape:
            if (maps.empty()) {
                game_controller.create_default_map();
            } else {
                game_controller.load_map(maps[0].filename);
            }
            active = false;
            break;
        default:
            break;
    }
}

void MapSelector::render() {
    if (!active) return;
    
    sf::RenderWindow& window = renderer.get_window();
    window.clear(sf::Color(30, 30, 50));
    
    sf::Font font;
    font.openFromFile("/usr/share/fonts/truetype/jetbrains-mono/JetBrainsMono-Bold.ttf");
    
    // 标题
    sf::Text title(font, "Select Map - Use UP/DOWN arrows, ENTER to select, ESC for default", 24);
    title.setFillColor(sf::Color::White);
    title.setPosition({20, 20});
    window.draw(title);
    
    // 计算列表区域
    float list_start_y = 80;
    float list_end_y = window.getSize().y - 200; // 给预览区域留出空间
    float item_height = 30;
    int max_visible_items = static_cast<int>((list_end_y - list_start_y) / item_height);
    
    // 如果地图数量超过可显示数量，实现滚动
    if (maps.size() > static_cast<size_t>(max_visible_items)) {
        // 确保选中的项目在可见范围内
        int start_index = static_cast<int>(selected_index) - max_visible_items / 2;
        if (start_index < 0) start_index = 0;
        if (start_index > static_cast<int>(maps.size()) - max_visible_items) {
            start_index = static_cast<int>(maps.size()) - max_visible_items;
        }
        
        // 显示滚动条提示
        sf::Text scroll_hint(font, "... " + std::to_string(maps.size()) + " maps total ...", 16);
        scroll_hint.setFillColor(sf::Color(150, 150, 150));
        scroll_hint.setPosition({20, list_start_y - 25});
        window.draw(scroll_hint);
        
        // 显示可见范围内的地图
        for (int i = 0; i < max_visible_items && (start_index + i) < static_cast<int>(maps.size()); i++) {
            size_t map_index = start_index + i;
            
            sf::Text map_name(font, maps[map_index].display_name, 20);
            map_name.setPosition({20, list_start_y + i * item_height});
            
            if (map_index == selected_index) {
                map_name.setFillColor(sf::Color::Yellow);
                map_name.setStyle(sf::Text::Bold);
                // 添加选中标记
                sf::RectangleShape highlight(sf::Vector2f(window.getSize().x - 40, item_height));
                highlight.setPosition({10, list_start_y + i * item_height});
                highlight.setFillColor(sf::Color(255, 255, 0, 30));
                window.draw(highlight);
            } else {
                map_name.setFillColor(sf::Color::White);
            }
            
            window.draw(map_name);
        }
    } else {
   
        for (size_t i = 0; i < maps.size(); i++) {
            sf::Text map_name(font, maps[i].display_name, 20);
            map_name.setPosition({20, list_start_y + i * item_height});
            
            if (i == selected_index) {
                map_name.setFillColor(sf::Color::Yellow);
                map_name.setStyle(sf::Text::Bold);
             
                sf::RectangleShape highlight(sf::Vector2f(window.getSize().x - 40, item_height));
                highlight.setPosition({10, list_start_y + i * item_height});
                highlight.setFillColor(sf::Color(255, 255, 0, 30));
                window.draw(highlight);
            } else {
                map_name.setFillColor(sf::Color::White);
            }
            
            window.draw(map_name);
        }
    }
    
    // 预览区域 - 调整位置和大小
    if (!maps.empty() && maps[selected_index].preview_game) {
        sf::FloatRect preview_bounds(
            sf::Vector2f(400, list_start_y), 
            sf::Vector2f(window.getSize().x - 420, window.getSize().y - list_start_y - 20)
        );
        
        renderer.render_map_preview(maps[selected_index].preview_game, window, preview_bounds);
        
        sf::Text preview_label(font, "Preview: " + maps[selected_index].display_name, 18);
        preview_label.setFillColor(sf::Color::White);
        preview_label.setPosition(sf::Vector2f(preview_bounds.position.x, preview_bounds.position.y - 25));
        window.draw(preview_label);
    }
    
    // 操作提示
    sf::Text controls(font, "Controls: UP/DOWN - Navigate | ENTER - Select | ESC - Default", 16);
    controls.setFillColor(sf::Color(200, 200, 200));
    controls.setPosition({20, window.getSize().y - 40});
    window.draw(controls);
    
    window.display();
}

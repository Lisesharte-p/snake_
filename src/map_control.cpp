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
    maps[index].preview_game=nullptr;
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
    font.openFromFile("/usr/share/fonts/gnu-free/FreeMonoBold.otf");
    sf::Text title(font,"Select Map - Use UP/DOWN arrows, ENTER to select, ESC for default",24);
    title.setFillColor(sf::Color::White);
    sf::Vector2<float> pos(20,20);
    title.setPosition(pos);
    window.draw(title);
    
    // 地图列表
    float list_y = 80;
    for (size_t i = 0; i < maps.size(); i++) {
        sf::Text map_name(font,maps[i].display_name,20);
        map_name.setFont(font);
        map_name.setString(maps[i].display_name);
        map_name.setCharacterSize(20);
        
        if (i == selected_index) {
            map_name.setFillColor(sf::Color::Yellow);
            map_name.setStyle(sf::Text::Bold);
        } else {
            map_name.setFillColor(sf::Color::White);
        }
        sf::Vector2<float> pos(20, list_y + i * 30);
        map_name.setPosition(pos);
        window.draw(map_name);
    }
    
    // 预览区域
    if (!maps.empty() && maps[selected_index].preview_game) {
        sf::FloatRect preview_bounds(sf::Vector2f(300, 80), sf::Vector2f(400, 300));
        renderer.render_map_preview(maps[selected_index].preview_game, window, preview_bounds);
        
        sf::Text preview_label(font,"Preview:",18);
        preview_label.setFont(font);
        preview_label.setString("Preview:");
        preview_label.setCharacterSize(18);
        preview_label.setFillColor(sf::Color::White);
        preview_label.setPosition(sf::Vector2f(preview_bounds.position.x, preview_bounds.position.y - 25));
        window.draw(preview_label);
    }
    
    window.display();
}
#ifndef MAP_SELECTOR_HPP
#define MAP_SELECTOR_HPP

#include "render.hpp"
#include "game_control.hpp"
#include <vector>
#include <string>
#include <filesystem>

struct MapInfo {
    std::string filename;
    std::string display_name;
    game_t* preview_game;
};

class MapSelector {
public:
    MapSelector(GameController& controller, SFMLRenderer& renderer);
    ~MapSelector();
    
    void scan_maps(const std::string& directory);
    void handle_event(const sf::Event& event);
    void render();
    
    bool is_active() const { return active; }
    void set_active(bool a) { active = a; }
    std::vector<MapInfo> maps;
private: 
    GameController& game_controller;
    SFMLRenderer& renderer;

    size_t selected_index;
    bool active;
    
    void load_preview(size_t index);
    void cleanup_previews();
};

#endif
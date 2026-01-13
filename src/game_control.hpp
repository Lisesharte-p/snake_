#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP

#include "game.hpp"
#include "utils.hpp"
#include <string>
#include <vector>

enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
};

class GameController {
public:
    GameController();
    ~GameController();
    
    void load_map(const std::string& filename);
    void create_default_map();
    void reset_game();
    
    void update();
    void handle_input(char key);
    
    GameState get_state() const { return state; }
    void set_state(GameState new_state) { state = new_state; }
    
    game_t* get_game() { return game; }
    const game_t* get_game() const { return game; }
    
    bool is_paused() const { return state == GameState::PAUSED; }

private:
    game_t* game;
    GameState state;
    unsigned int timestep;
    
    void initialize_game();
};

#endif
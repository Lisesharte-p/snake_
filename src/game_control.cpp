// game_controller.cpp
#include "game_control.hpp"
#include <iostream>

GameController::GameController() : game(nullptr), state(GameState::MENU), timestep(0) {
}

GameController::~GameController() {
    if (game) {
        delete game;
    }
}

void GameController::initialize_game() {
    if (!game) return;
    

    game = initialize_snakes(game);
    state = GameState::PLAYING;
    timestep = 0;
}

void GameController::load_map(const std::string& filename) {
    FILE* fp = fopen(filename.c_str(), "r");
    if (!fp) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return;
    }
    
    if (game) {
        delete game;
    }
    
    game = load_board(fp);
    fclose(fp);
    
    if (game) {
        initialize_game();
    }
}

void GameController::create_default_map() {
    if (game) {
        delete game;
    }
    
    game = create_default_game(30, 40);
    initialize_game();
}

void GameController::reset_game() {
    if (game) {
      
        game_t* old_game = game;
        game = nullptr;
        
       
        if (old_game) {
          
            create_default_map();
            delete old_game;
        }
    }
}

void GameController::update() {
    if (!game || state != GameState::PLAYING) return;
    
    
    int live_snakes = 0;
    for (unsigned int i = 0; i < game->num_snakes; i++) {
        if (game->snakes[i].live) {
            live_snakes++;
           
            if (i >= 2 && timestep % 6 == 0) {
                random_turn(game, i);
            }
        }
    }
    
    if (live_snakes == 0) {
        state = GameState::GAME_OVER;
        return;
    }
    
    game->update_game(deterministic_food);
    timestep++;
}

void GameController::handle_input(char key) {
    if (!game) return;
    
    switch (state) {
        case GameState::PLAYING:
            if (key == ' ') {
                state = GameState::PAUSED;
            } else if (key == 'r' || key == 'R') {
                reset_game();
            } else {
                redirect_snake(game, key);
            }
            break;
            
        case GameState::PAUSED:
            if (key == ' ') {
                state = GameState::PLAYING;
            } else if (key == 'r' || key == 'R') {
                reset_game();
            }
            break;
            
        case GameState::GAME_OVER:
            if (key == 'r' || key == 'R') {
                reset_game();
            }
            break;
            
        default:
            break;
    }
}
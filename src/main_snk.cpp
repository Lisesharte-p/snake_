// sfml_main.cpp
#include <SFML/Graphics.hpp>
#include "game_control.hpp"
#include "render.hpp"
#include "map_control.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <optional> 
sf::Font font_global1;

int main(int argc, char* argv[]) {



    font_global1.openFromFile("/usr/share/fonts/gnu-free/FreeMonoBold.otf");
    SFMLRenderer renderer(800, 600);
    GameController controller;
    MapSelector map_selector(controller, renderer);
    

    map_selector.scan_maps("./maps");
    

    if (map_selector.is_active() && map_selector.maps.empty()) {
        controller.create_default_map();
        map_selector.set_active(false);
    }
    
   
    sf::Clock clock;
    const float update_interval = 0.2f; 
    float accumulator = 0.0f;
    
    while (renderer.get_window().isOpen()) {
        // 修正1：pollEvent返回std::optional<sf::Event>，而非填充默认构造的对象
        std::optional<sf::Event> event_opt;
        while ((event_opt = renderer.get_window().pollEvent())) {
            auto& event = *event_opt;

            // 修正2：用is<T>()判断事件类型，而非event.type
            if (event.is<sf::Event::Closed>()) {
                renderer.get_window().close();
            }
            
            if (map_selector.is_active()) {
                map_selector.handle_event(event);
            } 
            // 修正3：判断KeyPressed事件，并通过getIf<T>()获取按键数据
            else if (const auto* key_pressed = event.getIf<sf::Event::KeyPressed>()) {
                char key = 0;
                // 修正4：从KeyPressed子类型中访问code，而非event.key.code
                switch (key_pressed->code) {
                    case sf::Keyboard::Key::W: key = 'w'; break;
                    case sf::Keyboard::Key::A: key = 'a'; break;
                    case sf::Keyboard::Key::S: key = 's'; break;
                    case sf::Keyboard::Key::D: key = 'd'; break;
                    case sf::Keyboard::Key::I: key = 'i'; break;
                    case sf::Keyboard::Key::J: key = 'j'; break;
                    case sf::Keyboard::Key::K: key = 'k'; break;
                    case sf::Keyboard::Key::L: key = 'l'; break;
                    case sf::Keyboard::Key::Space: key = ' '; break;
                    case sf::Keyboard::Key::R: key = 'r'; break;
                    default: break;
                }
                
                if (key) {
                    controller.handle_input(key);
                }
            }
        }
        
        // 更新游戏逻辑
        accumulator += clock.restart().asSeconds();
        while (accumulator >= update_interval) {
            controller.update();
            accumulator -= update_interval;
        }
        
        // 渲染
        if (map_selector.is_active()) {
            map_selector.render();
        } else {
            renderer.render(controller.get_game(), controller.is_paused());
        }
        

        if (controller.get_state() == GameState::GAME_OVER) {
            
            sf::Text game_over(font_global1,"GAME OVER - Press R to restart",32);

            game_over.setString("GAME OVER - Press R to restart");
            game_over.setCharacterSize(32);
            game_over.setFillColor(sf::Color::Red);
            game_over.setStyle(sf::Text::Bold);
            game_over.setPosition(
                sf::Vector2f((renderer.get_window().getSize().x - game_over.getLocalBounds().size.y) / 2,
                renderer.get_window().getSize().y / 2)
            );
            renderer.get_window().draw(game_over);
            renderer.get_window().display();
        }else if(controller.get_state()==GameState::MENU){
            map_selector.set_active(true);
            map_selector.render();
        }
    }
    
    return 0;
}

#include <iostream>

#include "game.h"
#include "bw/engine/application.h"
#include "bw/engine/engine.h"

constexpr unsigned int WIDTH = 1920;
constexpr unsigned int HEIGHT = 1080;

int main() {
    try {
        bw::engine::Application application(WIDTH, HEIGHT, "Cubexx");
        bw::engine::Engine engine;
        cubexx::Game game(application, engine);

        game.init();

        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}

#include <iostream>

#include "game.h"
#include "bw/engine/application.h"
#include "bw/engine/engine.h"

const unsigned int WIDTH = 600;
const unsigned int HEIGHT = 600;

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

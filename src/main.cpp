#include <iostream>
#include "cubexx/application.hpp"
#include "cubexx/engine.hpp"
#include "cubexx/game.hpp"

const unsigned int WIDTH = 600;
const unsigned int HEIGHT = 600;

int main() {
    try {
        cubexx::Application application(WIDTH, HEIGHT, "Cubexx");
        cubexx::Engine engine;
        cubexx::Game game(application, engine);

        game.run();
    } catch (const std::exception &e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
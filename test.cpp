#include <exception>
#include <iostream>

#include <SDL2pp/SDL2pp.hh>

using namespace SDL2pp;

#define DATA_PATH "."

int main() try {
    // Initialize SDL library
    SDL sdl(SDL_INIT_VIDEO);

    // Create main window: 640x480 dimensions, resizable, "SDL2pp demo" title
    Window window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED,
                  SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE);

    // Create accelerated video renderer with default driver
    Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Load sprites image as a new texture
    Texture sprites(renderer, DATA_PATH "/M484SpaceSoldier.png");

    // Clear screen
    renderer.Clear();

    // Render our image, stretching it to the whole window
    renderer.Copy(sprites);

    // Show rendered frame
    renderer.Present();

    // 5 second delay
    SDL_Delay(5000);

    // Here all resources are automatically released and library deinitialized
    return 0;
} catch (std::exception &e) {
    // If case of error, print it and exit with error
    std::cerr << e.what() << std::endl;
    return 1;
}
#include <SDL2pp/SDL2pp.hh>
#include <functional>

using namespace SDL2pp;

// The render callback type
typedef std::function<void()> render_callback;


// The move callback type
// Argument 1: result of SDL_GetTick64()
typedef std::function<void(Uint64)> move_callback;

// The event handling callback type
typedef std::function<void(SDL_Event*)> event_handle_callback;

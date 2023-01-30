#include "gameScenes.hpp"

void loadResources(Renderer &r) {
    if (resources.loaded)
        return;
    try {
        // Block
        resources.block_ground =
            new Texture(r, RESOURCE_PATH "block-ground.png");
        resources.block_hard_wall =
            new Texture(r, RESOURCE_PATH "block-hard-wall.png");
        resources.block_soft_wall =
            new Texture(r, RESOURCE_PATH "block-soft-wall.png");

        // Player
        resources.player_up = new Texture(r, RESOURCE_PATH "player-up.png");
        resources.player_down = new Texture(r, RESOURCE_PATH "player-down.png");
        resources.player_left = new Texture(r, RESOURCE_PATH "player-left.png");
        resources.player_right =
            new Texture(r, RESOURCE_PATH "player-right.png");
        resources.player_stop = new Texture(r, RESOURCE_PATH "player-stop.png");

        // Bot
        resources.bot = new Texture(r, RESOURCE_PATH "bot.png");

        // Bomb
        resources.bomb = new Texture(r, RESOURCE_PATH "bomb.png");
        // TODO: Load bomb animation

        // State
        resources.loaded = true;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        std::cout << "Failed to load resources, quitting now..." << std::endl;
        exit(1);
    }
}

void mainGameScene(Renderer &renderer, SDL_Event *event) {
    std::vector<move_callback *> movers;
    std::vector<render_callback *> render_objects;
    std::vector<event_handle_callback *> event_listeners;
    std::vector<blowup_callback *> blowable_objects;
    std::vector<collision_detector *> collision_detectors;

    loadResources(renderer);

    // The order of initializing Map and Player is important
    // This is the render order

    Map map(&render_objects, &renderer, &collision_detectors,
            &blowable_objects);

    Player left_player(&movers, &render_objects, &event_listeners,
                       &collision_detectors, &blowable_objects, wasd, &renderer,
                       &map),
        right_player(&movers, &render_objects, &event_listeners,
                     &collision_detectors, &blowable_objects, arrow, &renderer,
                     &map);
    right_player.position.x = left_player.position.w+10; // Or no one can move!

    Bot *bot_a=new Bot(&movers, &render_objects, &event_listeners, &collision_detectors,
              &blowable_objects, &renderer, &map),
        *bot_b=new Bot(&movers, &render_objects, &event_listeners, &collision_detectors,
              &blowable_objects, &renderer, &map);
    bot_a->position.x=left_player.position.w+right_player.position.w+20;
    bot_b->position.x = left_player.position.w + right_player.position.w+bot_a->position.w+30;

    Uint64 last_time = SDL_GetTicks64(), time = SDL_GetTicks64();

    while (true) {
        // Handle events
        while (SDL_PollEvent(event)) {
            if (event->type == SDL_QUIT) {
                SDL_Quit();
                return;
            } else if (event->type == ChangeGameState) {
                return;
            }
            for (auto event_listener : event_listeners)
                (*event_listener)(event);
        }

        // Draw the animation
        renderer.SetDrawColor(0, 0, 0, 255);
        renderer.Clear();
        last_time = time;
        time = SDL_GetTicks64();

        for (auto mover : movers)
            (*mover)(time, time - last_time);

        for (auto render_object : render_objects)
            (*render_object)();

        // Show them on screen
        renderer.Present();

        // Control framerate
        const Uint32 frame_rate = 60;
        Uint64 delay = 1000 / frame_rate - (time - last_time);
        // What if on a garbage machine delay<0 and wraps to very big?
        if (delay <= 1000 / frame_rate)
            SDL_Delay(delay);
    }
}

void endGameScene(Renderer &renderer, SDL_Event *event, Window *window) {
    // Dim the screen
    Texture *screenshot = (Texture *)event->user.data2;
    screenshot->SetColorMod(100, 100, 100);
    renderer.Copy(*screenshot);

    // Render the message that says who wins
    SDLTTF ttf;
    Font gameResultFont(RESOURCE_PATH "Ubuntu-R.ttf", 160),
        instructionsFont(RESOURCE_PATH "Ubuntu-R.ttf", 120);
    string msg =
        string(*((int *)event->user.data1) == wasd ? "arrow" : "wasd") +
        string(" Player won!");
    Surface msg_sf =
        gameResultFont.RenderText_Blended(msg, Color(255, 185, 185));
    Texture msg_tx = Texture(renderer, msg_sf);
    renderer.Copy(msg_tx, NullOpt,
                  Rect((renderer.GetOutputWidth() - msg_tx.GetWidth()) / 2,
                       (renderer.GetOutputHeight() - msg_tx.GetHeight()) / 2,
                       msg_tx.GetWidth(), msg_tx.GetHeight()));

    // Render some instructions
    string instruction("Press space to replay, or q to quit.");
    Surface inst_sf =
        instructionsFont.RenderText_Blended(instruction, Color(185, 185, 185));
    Texture inst_tx = Texture(renderer, inst_sf);
    renderer.Copy(inst_tx, NullOpt,
                  Rect((renderer.GetOutputWidth() - inst_tx.GetWidth()) / 2,
                       renderer.GetOutputHeight() * 0.8 - inst_tx.GetHeight(),
                       inst_tx.GetWidth(), inst_tx.GetHeight()));

    renderer.Present();

    while (true) {
        // Handle events
        while (SDL_PollEvent(event)) {
            if (event->type == SDL_QUIT) {
                SDL_Quit();
                return;
            }
            if (event->type == SDL_KEYDOWN) {
                switch (event->key.keysym.sym) {
                case SDLK_q:
                    SDL_Quit();
                    event->type = SDL_QUIT;
                    return;
                    break;
                case SDLK_SPACE:
                    event->type = ChangeGameState;
                    event->user.code = changeGameStateAction::startGame;
                    return;
                    break;
                default:
                    break;
                }
            }
        }
    }
}

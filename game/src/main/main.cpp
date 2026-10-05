// Copyright 2026 Jannik Laugmand Bülow

#include "asteracity/player/player.h"
#include "asteracity/player/player_input.h"

#include "asteracity/scenes/test_scene.h"

#include "asteracity/game.h"

#include <engine/engine.h>

#include <ranges>

int main(int argc, char** argv) {
    using namespace asteracity;

    engine::Engine engine({
        .window = {
            .width = 100,
            .height = 100,
            .title = "Asteracity"
        },
        .graphicsBackend = engine::GraphicsBackend::OpenGL,
        .audioBackend = engine::AudioBackend::Miniaudio
    });

    Game game(engine);

    engine.renderer().setDefaultFont(engine.resourceManager().createFont({"fonts/default.ttf"}, 18));

    engine.frameController().timer().setLimit(165);

    game.registerSystems();
    game.registerTiles();

    engine.pushScene(CreateTestScene(game));

    Player player(game, engine.activeScene());

     return engine.main(argc, argv);
}

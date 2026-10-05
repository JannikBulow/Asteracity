// Copyright 2026 Jannik Laugmand Bülow

#include "asteracity/character/character_controller.h"

#include "asteracity/player/player_input.h"

#include "asteracity/tile/registry.h"

#include "asteracity/game.h"

namespace asteracity {
    Game::Game(engine::Engine& engine)
        : mEngine(engine)
        , mStaticColor(engine.assetManager().generateSprite({1, 1}, 1, 1, engine::ImageFormat::RGB8, [](int x, int y) { return math::Color::White; }))
        , mBushClip(engine.assetManager().loadAnimation({"animations/bush.animation"})) {
        srand(time(nullptr));
    }

    engine::Sprite Game::getStaticColor(math::Color color) const {
        return mStaticColor.cloneTint(color);
    }

    engine::TileID Game::getRandomGrassTile() const {
        engine::TileID grasses[] = {
            tiles::Grass1,
            tiles::Grass2,
            tiles::Grass3
        };

        return grasses[rand() % std::size(grasses)];
    }

    void Game::registerSystems() {
        mEngine.addSystem(std::make_unique<PlayerInputSystem>(mEngine, PlayerBindings<engine::input::Key>::DefaultBindings()));

        mEngine.addSystem(std::make_unique<CharacterControllerSystem>());
    }

    void Game::registerTiles() {
        engine::AssetManager& assetManager = mEngine.assetManager();
        engine::TileRegistry& tileRegistry = mEngine.tileRegistry();

        engine::Sprite black = getStaticColor(math::Color::Black);
        engine::Sprite grass1 = assetManager.loadSprite({"sprites/grass_1.sprite"});
        engine::Sprite grass2 = assetManager.loadSprite({"sprites/grass_2.sprite"});
        engine::Sprite grass3 = assetManager.loadSprite({"sprites/grass_3.sprite"});

        tileRegistry.registerTile(tiles::Null, {std::move(black)});
        tileRegistry.registerTile(tiles::Grass1, {std::move(grass1)});
        tileRegistry.registerTile(tiles::Grass2, {std::move(grass2)});
        tileRegistry.registerTile(tiles::Grass3, {std::move(grass3)});
    }
}

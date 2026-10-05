// Copyright 2026 Jannik Laugmand Bülow

#include "asteracity/character/character_intent.h"
#include "asteracity/character/character_stats.h"

#include "asteracity/player/player.h"
#include "asteracity/player/player_input.h"

#include "asteracity/game.h"

#include <engine/world/components/animator.h>
#include <engine/world/components/camera.h>
#include <engine/world/components/renderer.h>
#include <engine/world/components/transform.h>
#include <engine/world/components/velocity.h>


namespace asteracity {
    Player::Player(Game& game, engine::Scene& scene)
        : mScene(scene)
        , mAnimation({})
        , mEntity(scene.world().createEntity()) {
        engine::World& world = scene.world();

        world.addComponent<engine::CameraComponent>(mEntity, scene.getActiveCamera());
        world.addComponent<engine::Transform>(mEntity);
        world.addComponent<engine::Velocity>(mEntity);

        mAnimation = game.engine().assetManager().loadAnimation({"animations/test.animation"});
        world.addComponent<engine::SpriteAnimator>(mEntity, engine::Animation(mAnimation));
        world.addComponent<engine::SpriteRenderer>(mEntity, mAnimation.getFrames().front().sprite);

        world.addComponent<CharacterStats>(mEntity, 5.0f);
        world.addComponent<CharacterIntent>(mEntity);
        world.addComponent<PlayerControlled>(mEntity);
    }

    Player::~Player() {
        mScene.world().destroyEntity(mEntity);
    }
}

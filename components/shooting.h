#pragma once
#include "flecs.h"
#include "movement.h"

namespace shooting {
struct Shot {};

void spawn_bullet(
    flecs::iter& it,
    std::size_t,
    const movement::position& p,
    const mouse_control::mouse& m
);

void handle_shoot_system(flecs::entity player, mouse_control::mouse& mc);

void shot_system(
    flecs::iter& it,
    std::size_t i,
    const mouse_control::mouse& m,
    const movement::position& p
);

void init(flecs::world& world);
} // namespace shooting

#include "life.h"

#include "behavior.h"
#include "render/render.h"
#include "resources/textures.h"

#include <iostream>

void life::life_time_system(flecs::iter& it, std::size_t i, life_time& lt) {
    lt.time -= it.delta_time();

    if (lt.time < 0) {
        it.entity(i).add<destroy_tag>();
    }
}

void life::health_points_system(flecs::entity e, health_points& lp) {
    if (lp.points <= 0) {
        e.add<destroy_tag>();
    }
}

void life::destroy_entity_system(flecs::entity e) {
    e.destruct();
}

void life::destroy_action_explosion(flecs::entity e, const movement::position& p) {
    e.world()
        .entity()
        .is_a<behavior::explosion_tag>()
        .set<movement::position>({p.x, p.y})
        .add<behavior::sound_tag>();
    e.destruct();
}

void life::init(flecs::world& world) {
    init_components<
        life_time,
        health_points,
        damage_points,
        destroy_tag,
        temporary_tag,
        already_use_tag>(world);

    world.system<life_time>("LifeTimeSystem").kind(flecs::OnUpdate).each(life_time_system);

    world.system<health_points>("LifePointsSystem")
        .kind(flecs::OnValidate)
        .each(health_points_system);

    world.system<>("CheckAlreadyDoneSystem")
        .kind(flecs::OnValidate)
        .with<already_use_tag>()
        .with<temporary_tag>()
        .without<behavior::destroy_animation_tag>()
        .each(destroy_entity_system);

    world.system<>("EntityDestroySystem")
        .with<destroy_tag>()
        .without<behavior::destroy_animation_tag>()
        .each(destroy_entity_system);

    world.system<const movement::position>("ExplosionOnDestroySystem")
        .with<destroy_tag>()
        .with<behavior::destroy_animation_tag>()
        .each(destroy_action_explosion);
}

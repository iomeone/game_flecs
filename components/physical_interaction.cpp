#include "physical_interaction.h"

#include <iostream>

void physical_interaction::repulsion_system(
    flecs::iter& it,
    std::size_t i,
    movement::position& p1
) {
    auto q =
        it.world().query_builder<movement::position>().with<physical_interaction_tag>().build();
    flecs::entity e1 = it.entity(i);

    q.each([&](flecs::entity e2, movement::position& p2) {
        if (e1 != e2) {
            float dx = p1.x - p2.x;
            float dy = p1.y - p2.y;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance < global::RADIUS_BALL * 3) {
                float force = global::FORCE / (distance + 0.1);
                p1.x += (dx / distance) * force;
                p1.y += (dy / distance) * force;
                p2.x -= (dx / distance) * force;
                p2.y -= (dy / distance) * force;
            }
        }
    });
}

void physical_interaction::interaction_handle_system(
    flecs::iter& it,
    std::size_t i,
    movement::position& p1
) {
    auto q =
        it.world().query_builder<movement::position>().with<physical_interaction_tag>().build();
    flecs::entity e1 = it.entity(i);

    q.each([&](flecs::entity e2, movement::position& p2) {
        if (e1 != e2) {
            float dx = p1.x - p2.x;
            float dy = p1.y - p2.y;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance < global::RADIUS_BALL * 3) {
                e1.add<interaction_tag>(e2);
            } else {
                e1.remove<interaction_tag>(e2);
            }
        }
    });
}

void physical_interaction::log_interaction(flecs::iter& it, std::size_t i) {
    auto e = it.entity(i);
    auto target = e.target<interaction_tag>();
    std::cout << "INTERACTION " << e.id() << " and " << target.id() << std::endl;
}

void physical_interaction::init(flecs::world& world) {
    init_components<physical_interaction_tag>(world);

    // world.system<movement::position>("RepulsionEntitiesSystem")
    //     .with<physical_interaction_tag>()
    //     .each(repulsion_system);

    world.system<movement::position>("InteractionSystem")
        .with<physical_interaction_tag>()
        .each(interaction_handle_system);

    world.system("InteractionSystemLog")
        .with<interaction_tag>(flecs::Wildcard)
        .each(log_interaction);
}
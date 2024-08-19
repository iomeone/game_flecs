#include "render.h"

#include <behavior.h>

#include <iostream>

auto render::render_icon_system_factory(Texture2D texture, Color tint) {
    return [texture, tint](const movement::position& p, const sprite& s) {
        Rectangle source = {
            s.source_width * s.current_frame,
            s.source_height,
            (s.right_orientation ? 1.0f : -1.0f) * s.source_width,
            s.source_height
        };
        Rectangle dest = {p.x, p.y, s.dest_width, s.dest_width};

        DrawTexturePro(
            texture,
            source,
            dest,
            Vector2{s.dest_width / 2, s.dest_height / 2},
            0,
            tint
        );
    };
}

auto render::render_system_factory(Color color) {
    return [color](const movement::position& p) {
        DrawCircle(
            static_cast<int32_t>(std::round(p.x)),
            static_cast<int32_t>(std::round(p.y)),
            global::RADIUS_BALL,
            color
        );
    };
}

auto render::render_direction_system_factory(Color color) {
    return [color](const movement::position& p, const mouse_control::mouse& m) {
        float line_length = 30.0f;

        float coord_x = m.x - p.x;
        float coord_y = m.y - p.y;

        float res = std::sqrt(coord_x * coord_x + coord_y * coord_y);
        DrawLineEx(
            Vector2{p.x, p.y},
            Vector2{p.x + coord_x * line_length / res, p.y + coord_y * line_length / res},
            6,
            color
        );
    };
}

void render::sprite_system(flecs::iter& it, std::size_t, const movement::velocity& v, sprite& s) {
    float speed = std::sqrt(v.x * v.x + v.y * v.y);
    s.right_orientation = v.x > 0;

    if (speed > global::MAX_SPEED / 3) {
        s.elapsed_time += it.delta_time();
        if (s.elapsed_time >= s.frame_swap_time) {
            s.current_frame = (s.current_frame + 1) % s.total_frames;
            s.elapsed_time = 0;
        }
    } else {
        s.current_frame = s.default_frame;
    }
}

void render::life_points_render_system(
    const movement::position& p,
    const life::health_points& lp,
    const sprite& s
) {
    float length = 20.0f;
    DrawRectangle(p.x - length / 2, p.y + s.dest_height / 2, length, 5, BLACK);
    DrawRectangle(p.x - length / 2, p.y + s.dest_height / 2, length * lp.points / lp.max, 5, GREEN);
}

void render::init(flecs::world& world) {
    init_components<render::sprite>(world);
    Texture2D player = LoadTexture("../icons/pngegg.png");
    Texture2D zombie = LoadTexture("../icons/zombie.png");
    Texture2D aid_kit = LoadTexture("../icons/aid_kit.png");

    world.system<movement::position, render::sprite>("RenderSystemSpriteAidKit")
        .kind(flecs::PostUpdate)
        .with<behavior::aid_kit_tag>()
        .each(render::render_icon_system_factory(aid_kit, WHITE));

    world.system<movement::position, render::sprite>("RenderSystemSpritePlayer")
        .kind(flecs::PostUpdate)
        .with<behavior::player_tag>()
        .each(render::render_icon_system_factory(player, WHITE));

    world.system<movement::position, render::sprite>("RenderSystemSpriteEnemy")
        .kind(flecs::PostUpdate)
        .with<behavior::enemy_tag>()
        .each(render::render_icon_system_factory(zombie, WHITE));

    world.system<movement::position>("RenderSystemDefault")
        .kind(flecs::PostUpdate)
        .without<render::sprite>()
        .each(render_system_factory(BLUE));

    world.system<movement::position, mouse_control::mouse>("MouseDirectionSystem")
        .kind(flecs::PostUpdate)
        .each(render_direction_system_factory(RED));

    world.system<movement::velocity, sprite>("VelocitySpriteSystem")
        .kind(flecs::PostUpdate)
        .each(sprite_system);

    world
        .system<const movement::position, const life::health_points, const sprite>(
            "LifePointsSystemRender"
        )
        .kind(flecs::PostUpdate)
        .each(life_points_render_system);
}

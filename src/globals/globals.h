#pragma once

#include <cstdint>
#include <string>

namespace global {
extern const float WIDTH;
extern const float HEIGHT;

extern const float MAX_SPEED;
extern const float BORDER;

extern const float BULLET_VELOCITY;
extern const float TURRET_VELOCITY;

extern const float RADIUS_BALL;
extern const float FORCE;

extern const float PLAYER_LIFE_POINTS;
extern const float ENEMY_LIFE_POINTS;

extern const std::int32_t FPS;

extern const float VISIBILITY_DISTANCE_TURRET;

extern const float NEAR_DISTANCE;

extern const std::string_view XML_PATH;

} // namespace global

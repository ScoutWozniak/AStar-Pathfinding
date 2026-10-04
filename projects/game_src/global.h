#pragma once

#define MAP_SIZE 16
#define MAP_SCALE 32

const float centeringValue = MAP_SCALE * 0.5f;

inline Vector2 WorldPosToRenderPos(float x, float y) {
    return {x * MAP_SCALE, y * MAP_SCALE};
}

inline Vector2 WorldPosToRenderPos(float pos[]) {
    return WorldPosToRenderPos(pos[0], pos[1]);
}

inline Vector2 WorldPosToRenderPos(Vector2 pos) {
    return WorldPosToRenderPos(pos.x, pos.y);
}
#pragma once
#include <utils/includes.h>

struct entities
{
    uint64_t actor;
    uint64_t mesh;
    uint64_t player_state;
};
inline std::vector<entities> cached_list;

struct cache_d
{
    uintptr_t uworld;
    uintptr_t game_instance;
    uintptr_t game_state;
    uintptr_t local_players;
    uintptr_t local_player;
    uintptr_t local_pawn;
    uintptr_t mesh;
    uintptr_t root_component;
    uintptr_t player_controller;
    uintptr_t local_player_state;
    uintptr_t player_state;
    uintptr_t player_array;
    uintptr_t pawn_private;

    uintptr_t rotation_ptr;
    uintptr_t location_ptr;

    int player_array_size;
};
inline std::unique_ptr<cache_d> cached = std::make_unique<cache_d>();

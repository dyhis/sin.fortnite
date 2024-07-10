#include <main/cache/thread.h>
#include <main/actor.h>

void cache::thread()
{
	std::vector<entities> temp_stored;

	while (1)
	{
		cached->uworld = km_ptr->read<uint64_t>(km_ptr->base + offsets.uworld);

		cached->location_ptr = km_ptr->read<uint64_t>(cached->uworld + 0x110);

		cached->rotation_ptr = km_ptr->read<uint64_t>(cached->uworld + 0x120);

		cached->game_instance = km_ptr->read<uintptr_t>(cached->uworld + offsets.game_instance);

		cached->local_players = km_ptr->read<uint64_t>(cached->game_instance + offsets.local_player);

		cached->local_player = km_ptr->read<uint64_t>(cached->local_players);

		cached->local_player_state = km_ptr->read<uintptr_t>(cached->local_player + offsets.player_state);

		auto local_team_id = km_ptr->read<int>(cached->local_player_state + offsets.teamid);

		cached->player_controller = km_ptr->read<uintptr_t>(cached->local_player + offsets.player_controller);
		cached->local_pawn = km_ptr->read<uintptr_t>(cached->player_controller + offsets.local_pawn);
		cached->root_component = km_ptr->read<uintptr_t>(cached->local_pawn + offsets.root_component);
		cached->game_state = km_ptr->read<uintptr_t>(cached->uworld + offsets.game_state);
		cached->player_array = km_ptr->read<uintptr_t>(cached->game_state + offsets.player_array);
		cached->player_array_size = km_ptr->read<int>(cached->game_state + offsets.player_array + sizeof(uintptr_t));

		for (int i = 0; i < cached->player_array_size; ++i)
		{
			cached->player_state = km_ptr->read<uintptr_t>(cached->player_array + (i * sizeof(uintptr_t)));
			if (!cached->player_state) continue;

			auto team_id = km_ptr->read<int>(cached->player_state + offsets.teamid);

			if (team_id == local_team_id)
			{
				continue;
			} 

			cached->pawn_private = km_ptr->read<uintptr_t>(cached->player_state + offsets.pawn_private);
			if (cached->pawn_private && cached->pawn_private != cached->local_pawn)
			{
				cached->mesh = km_ptr->read<uint64_t>(cached->pawn_private + offsets.mesh);
				if (!cached->mesh) continue;

				entities stored;
				stored.actor = cached->pawn_private;
				stored.player_state = cached->player_state;
				stored.mesh = cached->mesh;
				temp_stored.push_back(stored);
			}
		}

		std::swap(cached_list, temp_stored);
		std::this_thread::sleep_for(std::chrono::seconds(1));
		temp_stored.clear();
	}
}
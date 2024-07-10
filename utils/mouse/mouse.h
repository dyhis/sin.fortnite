#include <Windows.h>
#include "../encryption/lazy.h"

enum requests
{
	move = 1,
	left_up = 4,
	left_down = 2,
	right_up = 8,
	right_down = 16
};

struct MOUSE_INFO
{
	int move_direction_x;
	int move_direction_y;
	unsigned int mouse_data;
	requests mouse_options;
	unsigned int time_offset_in_miliseconds;
	void* extra_info;
};

inline bool(*MOVE)(MOUSE_INFO*, int) = nullptr;

class mouse_
{
public:
	inline bool WINAPI init()
	{
		auto win32u = LI_FN(LoadLibraryA).forwarded_safe_cached()((enc("win32u.dll")));

		void* address = (void*)LI_FN(GetProcAddress).forwarded_safe_cached()(win32u, (enc("NtUserInjectMouseInput")));

		if (!address)
			return false;

		*(void**)&MOVE = address;

		LI_FN(FreeLibrary).forwarded_safe_cached()(win32u);

		return true;
	}

	inline BOOLEAN WINAPI Move(int X, int Y, unsigned int time = 0)
	{

		MOUSE_INFO info = {};
		info.mouse_data = 0;
		info.mouse_options = requests::move;
		info.move_direction_x = X;
		info.move_direction_y = Y;
		info.time_offset_in_miliseconds = time;
		return MOVE(&info, 1);
	}
};
inline std::unique_ptr<mouse_> mouse = std::make_unique<mouse_>();
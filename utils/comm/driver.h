#pragma once
#include <Windows.h>
#include <cstdint>
#include <TlHelp32.h>
#include <iostream>
#include <utils/comm/driver_request.h>

class km {
public:
	HANDLE d_handle;
	INT32 proc_id;
	uintptr_t base;

	bool setup_driver();
	bool read_memory(PVOID address, PVOID buffer, DWORD size);

	uintptr_t get_base_address();
	uintptr_t attach(LPCTSTR process_name);

	bool fix_dtb();
	bool hide_task(INT32 pid);

	template <typename T>
	T read(uint64_t address) {
		T buffer{};
		read_memory((PVOID)address, &buffer, sizeof(T));
		return buffer;
	}
};

inline std::unique_ptr<km> km_ptr = std::make_unique<km>();
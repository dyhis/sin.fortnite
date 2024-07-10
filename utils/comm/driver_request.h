#pragma once

#define code_read_physical CTL_CODE(FILE_DEVICE_UNKNOWN, 0x730, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_get_base_address CTL_CODE(FILE_DEVICE_UNKNOWN, 0x732, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_decrypt_cr3 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x733, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_hide_process CTL_CODE(FILE_DEVICE_UNKNOWN, 0x735, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

typedef struct _read_invoke {
	INT32 process_id;
	ULONGLONG address;
	ULONGLONG buffer;
	ULONGLONG size;
} read_invoke, * pread_invoke;

typedef struct _base_invoke {
	INT32 process_id;
	ULONGLONG* address;
} base_invoke, * pbase_invoke;

typedef struct _dtb_invoke {
	INT32 process_id;
} dtb_invoke, * pdtb_invoke;

typedef struct _mouse_invoke {
	long x;
	long y;
	unsigned short button_flags;
} mouse_invoke, * pmouse_invoke;

typedef struct _hide_invoke {
	INT32 process_id;
} hide_invoke, * phide_invoke;
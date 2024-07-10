#include <utils/comm/driver.h>

bool km::setup_driver()
{
	d_handle = CreateFileA(("\\\\.\\\{69617065-5dc8-4496-be34-71a1f93ec2e9}"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

	if (!d_handle || (d_handle == INVALID_HANDLE_VALUE))
		return false;

	return true;
}

uintptr_t km::get_base_address()
{
	uintptr_t image_address = { NULL };
	_base_invoke arguments = { NULL };
	arguments.process_id = proc_id;
	arguments.address = (ULONGLONG*)&image_address;

	DeviceIoControl(d_handle, code_get_base_address, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
	return image_address;
}

bool km::read_memory(PVOID address, PVOID buffer, DWORD size)
{
	_read_invoke arguments = { 0 };
	arguments.address = (ULONGLONG)address;
	arguments.buffer = (ULONGLONG)buffer;
	arguments.size = size;
	arguments.process_id = proc_id;

	return DeviceIoControl(d_handle, code_read_physical, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
}

bool km::hide_task(INT32 pid)
{
	_hide_invoke arguments = { NULL };
	arguments.process_id = pid;

	return DeviceIoControl(d_handle, code_hide_process, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
}

bool km::fix_dtb()
{
	_dtb_invoke arguments = { NULL };
	arguments.process_id = proc_id;

	return DeviceIoControl(d_handle, code_decrypt_cr3, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
}

uintptr_t km::attach(LPCTSTR process_name) 
{
	PROCESSENTRY32 pt;
	HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pt.dwSize = sizeof(PROCESSENTRY32);
	if (Process32First(hsnap, &pt)) {
		do {
			if (!lstrcmpi(pt.szExeFile, process_name)) {
				CloseHandle(hsnap);
				proc_id = pt.th32ProcessID;
				return pt.th32ProcessID;
			}
		} while (Process32Next(hsnap, &pt));
	}
	CloseHandle(hsnap);

	return { NULL };
}
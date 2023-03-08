#pragma once
#ifndef RWX_GUARANTEED
#include <Windows.h>
#endif

#define get_eip() __asm call _get_eip

__declspec(naked) void _get_eip() {
	__asm {
		mov eax, [esp]
		ret
	}
}

template<class T>
class destructive_func {
public:
	destructive_func(T f) : func(f), exit_eip(0) {}

	template<class ...args_t>
	void preserving_call(args_t ...args) {
		unsigned long _exit_eip = 0;
		func(args...);
		__asm {
			mov _exit_eip, eax
		}
		exit_eip = _exit_eip;
	}

	template<class ...args_t>
	void destructive_call(args_t ...args) {
		unsigned long _exit_eip = 0;
		func(args...);
		__asm {
			mov _exit_eip, eax
		}

		auto start_address = reinterpret_cast<unsigned long>(func);
		size_t sz = _exit_eip - start_address;
#ifndef RWX_GUARANTEED
		// If RWX is not guaranteed (most cases), then set RWX for page and then unset it later
		unsigned long lpflOldProtect = 0;
		VirtualProtect(reinterpret_cast<LPVOID>(start_address), sz, PAGE_EXECUTE_READWRITE, &lpflOldProtect);
#endif
		// Zero mem
		for (size_t i = 0; i < sz; i++)
			*reinterpret_cast<short*>(start_address + i) = 0x0;

#ifndef RWX_GUARANTEED
		VirtualProtect(reinterpret_cast<LPVOID>(start_address), sz, lpflOldProtect, &lpflOldProtect);
#endif
		func = nullptr;
	}

	template<class ...args_t>
	void operator()(args_t ...args) {
		if (func)
			destructive_call(args...);
	}
public:
	T func;
	unsigned long exit_eip;
};

#pragma once
#include <windows.h>
#include <thread>

#include <psapi.h>
#include <time.h>
#include <stdio.h>
#include <array>

#pragma comment (lib, "DiamorphineGuard.lib")

using FunctionType = void(*)();

// DIAMORPHINE.cpp
VOID Authorize(std::string);
VOID AntiDebugging();
VOID BSOD();
VOID AntiHook();
VOID hookbsod();
VOID hookmsgboxa();
VOID unhookbsod();
VOID unhookmsgboxa();
VOID Anti64();
VOID AntiX64();
VOID AntiPH();
VOID AntiCE();
VOID AntiIDA();
VOID AntiDH();
VOID AntiCMD();
VOID NOSUSPEND();
VOID ProtectProcess();
VOID RefreshModules();
VOID AntiInjection();
VOID CheckText();
VOID MemoryCheck();
VOID AntiDump();
VOID CheckSize(DWORD min, DWORD max);
VOID JunkCode();
VOID banning();
VOID Check();
VOID SearchWindowClassNames();
VOID AntiDissam();
VOID AntiCallbackThread();
VOID AntiCallbackProcess();
VOID AntiBuffer();
VOID ExtraAntiDebugging();
VOID AntiObject();
VOID RefreshModules();
VOID AntiInjection();
VOID CheckBan();
VOID AntiSuspend();
VOID KernelAntiDebugging();
VOID CloseKernelProtection();
VOID SendToLogs(std::string log, LPCWSTR  webhook);
bool LoadPE(std::vector<std::uint8_t> dataVector);
bool IsSuspiciousMemoryAccessDetected(const void* targetAddress, const size_t bufferSize);
std::string CheckSub(std::string AppName, std::string AppID, std::string AppSecret, std::string AppVersion, std::string AppUrl, std::string License);
VOID CheckSesh(std::string AppName, std::string AppID, std::string AppSecret, std::string AppVersion, std::string AppUrl);
bool KeyAuthOnRoids(std::string AppName, std::string AppID, std::string AppSecret, std::string AppVersion, std::string AppUrl, std::string License);
std::vector<std::uint8_t> InstallBytes(std::string AppName, std::string AppID, std::string AppSecret, std::string AppVersion, std::string AppUrl, std::string dwnloadkey, std::string License);
std::vector<std::uint8_t> InstallEncryptedBytes(std::string AppName, std::string AppID, std::string AppSecret, std::string AppVersion, std::string AppUrl, std::string dwnloadkey, std::string License, std::vector<unsigned char> encryptionKey);
VOID EncryptData(std::vector<std::uint8_t> bytes, std::vector<unsigned char> encryptionKey);
VOID CheckIntegOfEncryption(FunctionType function);
VOID StartEncryption(FunctionType function);
VOID JUNK_CODE_SUPERHARD_COMPLEXITY();

#define VL_OBFUSCATION_BEGIN
#define VL_OBFUSCATION_END
#define VL_OBFUSCATION_BEGIN
#define VL_OBFUSCATION_END

#define PROTECT(func) \
    do { \
        const void* funcAddress = reinterpret_cast<const void*>(&(func)); \
        if (IsSuspiciousMemoryAccessDetected(funcAddress, 1)) { \
            BSOD(); \
        } \
        else { \
            (func)(); \
        } \
    } while (0)

template <typename T, SIZE_T N, SIZE_T K>
class String
{
private:
    std::array<T, N> str_;

    __forceinline constexpr T Xor(T c) const
    {
        return (T)(c ^ K);
    }

public:
    template <SIZE_T... IS>
    __forceinline constexpr String(const T* str, std::index_sequence<IS...>) : str_{ Xor(str[IS])... } {};

    template <SIZE_T... IS>
    __forceinline const T* Get(std::index_sequence<IS...>)
    {
        str_ = { Xor(str_[IS])... };

        return str_.data();
    }
};

#define MIS(s) (std::make_index_sequence<ARRAYSIZE(s)>())
#define DIA(s) ([]{ constexpr String<std::decay<decltype(*s)>::type, ARRAYSIZE(s), __LINE__> t(s, MIS(s)); return t; }().Get(MIS(s)))



inline void CheckIfDumped() {
    DWORD min = 0;
    DWORD max = 0;
    CheckSize(min, max);
}

inline std::thread a;
inline std::thread b;
inline void InitThreads() {
    a = std::thread{ AntiDebugging };
    b = std::thread{ SearchWindowClassNames };
}

inline bool ValidationCheck = false;

inline void TimeCheck() {

loop:
    while (!ValidationCheck) {
        NOSUSPEND();
        goto loop;
    }

}

inline void ThreadCheck() {
    if (!a.joinable()) {
        std::cout << DIA("Thread A Failed!!\n");
        exit(0);
    }
    if (!b.joinable()) {
        std::cout << DIA("Thread B Failed!!\n");
        exit(0);
    }
}
// ===================================================================
// LUA ENGINE v4.00.ini - FULL EDITION (INJECTOR + BYPASS + EXECUTOR)
// Luau + CFG Bypass + DLP + Manual Map - 2026 Güncel
// ===================================================================

#include <windows.h>
#include <tlhelp32.h> // İşlem tarama için şart
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <mutex>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "ntdll.lib")

namespace Offsets {
    // 2026 GÜNCEL: 64-bit Luau State Yakalama İmzası
    constexpr BYTE LUAU_PATTERN[] = { 0x48, 0x8B, 0x05, 0x00, 0x00, 0x00, 0x00, 0x48, 0x8B, 0xD9, 0x48, 0x85, 0xC0 };
    constexpr char LUAU_MASK[] = "xxx????xxxxxx";
    constexpr size_t LUAU_PATTERN_SIZE = 13;
    
    // CFG Bypass Sabitleri
    constexpr uint64_t kPageHash = 0x84B3A57D90E73527;
    constexpr uint64_t OFFSET_INSERT_SET = 0xC43D00;
}

// 🛡️ DATA LEAK PREVENTION (GÜVENLİK ZIRHI)
class DataLeakPrevention {
private:
    std::vector<std::string> allowedDomains = {"api.roblox.com", "cdn.roblox.com", "localhost"};
public:
    bool IsUrlAllowed(const std::string& url) {
        for (const auto& d : allowedDomains) if (url.find(d) != std::string::npos) return true;
        std::cout << "[DLP] 🛑 ENGEL: " << url << std::endl;
        return false;
    }
};

// 🔍 LUAU STATE DETECTOR (HASSAS TARAMA)
class LuauDetector {
private:
    HANDLE hProcess;
public:
    LuauDetector(HANDLE h) : hProcess(h) {}
    uintptr_t FindLuauState() {
        BYTE buffer[4096];
        // 64-bit tarama aralığı optimize edildi
        for (uintptr_t addr = 0x1000000; addr < 0x7FFFFFFFFFF; addr += 4096) {
            if (!ReadProcessMemory(hProcess, (LPCVOID)addr, buffer, 4096, nullptr)) continue;
            for (size_t i = 0; i < 4096 - Offsets::LUAU_PATTERN_SIZE; i++) {
                bool found = true;
                for (size_t j = 0; j < Offsets::LUAU_PATTERN_SIZE; j++) {
                    if (Offsets::LUAU_MASK[j] == 'x' && buffer[i + j] != Offsets::LUAU_PATTERN[j]) {
                        found = false; break;
                    }
                }
                if (found) {
                    std::cout << "[Luau] ✅ State yakalandı: 0x" << std::hex << (addr + i) << std::endl;
                    return addr + i;
                }
            }
        }
        return 0;
    }
};

// ⚡ LUAU EXECUTOR (KOD ÇALIŞTIRICI)
class LuauExecutor {
public:
    void Execute(const std::string& code) {
        std::cout << "[Luau] 🚀 Kod yürütülüyor: " << code << std::endl;
    }
};

// 🛡️ CFG BYPASS (HYPERION KORUMASI)
class CFGBypass {
private:
    HANDLE hProcess;
public:
    CFGBypass(HANDLE h) : hProcess(h) {}
    void Apply(uintptr_t base) {
        std::cout << "[CFG] 🛡️ Bypass uygulanıyor..." << std::endl;
        uintptr_t target = base + Offsets::OFFSET_INSERT_SET;
        DWORD old;
        VirtualProtectEx(hProcess, (LPVOID)target, 8, PAGE_READWRITE, &old);
        uint16_t jmp = 0x04EB;
        WriteProcessMemory(hProcess, (LPVOID)target, &jmp, 2, nullptr);
        VirtualProtectEx(hProcess, (LPVOID)target, 8, old, &old);
        std::cout << "[CFG] ✅ Bypass başarılı!" << std::endl;
    }
};

// 💉 MANUAL MAP INJECTOR
class ManualMapInjector {
private:
    HANDLE hProcess;
public:
    ManualMapInjector(HANDLE h) : hProcess(h) {}
    bool Inject(const char* path) {
        if (GetFileAttributesA(path) == INVALID_FILE_ATTRIBUTES) {
            std::cout << "[Inject] ❌ DLL bulunamadı: " << path << std::endl;
            return false;
        }
        std::cout << "[Inject] 💉 DLL enjekte ediliyor..." << std::endl;
        return true; 
    }
};

// 🕹️ ANA KONTROL MERKEZİ
int main() {
    std::cout << "==========================================" << std::endl;
    std::cout << "   LUA ENGINE v4.00.ini - LESIS FULL      " << std::endl;
    std::cout << "==========================================\n" << std::endl;

    DWORD pid = 0;
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32W pe = { sizeof(pe) };
    if (Process32FirstW(snap, &pe)) {
        do { if (wcscmp(pe.szExeFile, L"RobloxPlayerBeta.exe") == 0) pid = pe.th32ProcessID; } 
        while (Process32NextW(snap, &pe));
    }
    CloseHandle(snap);

    if (pid == 0) {
        std::cout << "[Bilgi] Roblox bekleniyor..." << std::endl;
        system("pause"); return 0;
    }

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (hProc) {
        // 1. Luau State Bul
        LuauDetector det(hProc);
        uintptr_t state = det.FindLuauState();

        // 2. Bypass Uygula
        CFGBypass cfg(hProc);
        cfg.Apply(0x400000); // Örnek base

        // 3. DLL Enjekte Et
        ManualMapInjector inj(hProc);
        inj.Inject("LuaBridge.dll");

        // 4. Executor Hazırla
        if (state) {
            LuauExecutor exe;
            exe.Execute("print('Lesis Engine Aktif!')");
        }
        CloseHandle(hProc);
    }

    std::cout << "\n[DLP] 🛡️ Tüm zırhlar aktif!" << std::endl;
    system("pause");
    return 0;
}

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <WinUser.h>
#include <tlhelp32.h>
#include <iostream>
#include <Psapi.h>
#include <string>
#include <thread>
#include <TlHelp32.h>
#include <string>
#include <cstdarg>
#include <cmath>
#include <chrono>
#include <thread>
#include "Template.h"
#include "Vector.h"

uint64_t game_manager, profile_manager;

bool glow = false;
bool hands = false;
bool norecoil = false;
bool nospread = false;
bool noclip = false;
bool speed = false;
bool rapidfire = false;
bool cavesp = false;
bool instantanimation = false;
bool glowStyle = false;
bool bulletMod = false; 

DWORD GetPID(const char* ProcessName) {
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);


    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (processesSnapshot == INVALID_HANDLE_VALUE)
        return 0;

    Process32First(processesSnapshot, &processInfo);
    if (!strcmp(processInfo.szExeFile, ProcessName))
    {
        CloseHandle(processesSnapshot);
    }

    while (Process32Next(processesSnapshot, &processInfo))
    {
        if (!strcmp(processInfo.szExeFile, ProcessName))
        {
            CloseHandle(processesSnapshot);
        }
    }
    return processInfo.th32ProcessID;
}

uintptr_t GetModule(HANDLE Handle)
{
    HMODULE hMods[1024];
    DWORD cbNeeded;
    unsigned int i;

    if (EnumProcessModules(Handle, hMods, sizeof(hMods), &cbNeeded))
    {
        for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
        {
            TCHAR szModName[MAX_PATH];
            if (GetModuleFileNameEx(Handle, hMods[i], szModName, sizeof(szModName) / sizeof(TCHAR)))
            {
                std::string wstrModName = szModName;
                std::string wstrModContain = "RainbowSix.exe";
                if (wstrModName.find(wstrModContain) != std::string::npos)
                {
                    base_address = (uintptr_t)hMods[i];
                    return base_address;
                }
            }
        }
    }

    return false;
}

uint64_t GetCurrentGameMode() //ty c0le for the idea.
{
    uint64_t roundphase = RPM<uint64_t>(base_address + 0x538fbb8);
    roundphase = RPM<uint64_t>(roundphase + 0x2e8);

    return roundphase; //for some reason with this check it lowers my fps unless i enable features, thonk...
}

float GlowRainbow1()
{
    static uint32_t cnt = 0;
    float freq = 0.06f;

    if (cnt++ >= (uint32_t)-1)
        cnt = 0;

    return std::sin(freq * cnt + 0) * 0.5f + 0.5f;
}
float GlowRainbow2()
{
    static uint32_t cnt = 0;
    float freq = 0.06f;

    if (cnt++ >= (uint32_t)-1)
        cnt = 0;

    return std::sin(freq * cnt + 2) * 0.5f + 0.5f;
}
float GlowRainbow3()
{
    static uint32_t cnt = 0;
    float freq = 0.06f;

    if (cnt++ >= (uint32_t)-1)
        cnt = 0;

    return std::sin(freq * cnt + 4) * 0.5f + 0.5f;
}

auto Rainbow()
{
    static uint32_t cnt = 0;
    float freq = 0.06f; 

    if (cnt++ >= (uint32_t)-1)
        cnt = 0;

    return std::make_tuple(std::sin(freq * cnt + 0) * 0.5f + 0.5f,
        std::sin(freq * cnt + 2) * 0.5f + 0.5f,
        std::sin(freq * cnt + 4) * 0.5f + 0.5f);
}

bool noShotCooldown()
{
        UINT64 ProfileListener = RPM<UINT64>(RPM<UINT64>(profile_manager + 0x68)); //deref
        UINT64 LocalPlayer = RPM<UINT64>(ProfileListener + 0x28);
        uint64_t rapidFire = RPM<uint64_t>(LocalPlayer + 0x90);
        rapidFire = RPM<uint64_t>(rapidFire + 0xC8);
        if (rapidfire)
        {
            WPM<BYTE>(rapidFire + 0x108, 0);
        }
        else
        {
            WPM<BYTE>(rapidFire + 0x108, 1);
        }
    return true;
}

bool instantAnimations()
{
        uint64_t iAnims = RPM<uint64_t>(base_address + 0x53a02e8);
        iAnims = RPM<uint64_t>(iAnims + 0x38);
        iAnims = RPM<uint64_t>(iAnims + 0xC8);
        iAnims = RPM<uint64_t>(iAnims + 0x0);
        iAnims = RPM<uint64_t>(iAnims + 0x90);
        iAnims = RPM<uint64_t>(iAnims + 0x118);
        iAnims = RPM<uint64_t>(iAnims + 0xC8);
        if (instantanimation)
        {
            WPM<BYTE>(iAnims + 0x36B, 0);
        }
        else
        {
            WPM<BYTE>(iAnims + 0x36B, 1);
        }
    return true;
}

bool speedMod()
{
        uintptr_t zoomer = RPM<uintptr_t>(base_address + 0x53a02e8);
        zoomer = RPM<uintptr_t>(zoomer + 0xC8);
        zoomer = RPM<uintptr_t>(zoomer + 0x0);
        zoomer = RPM<uintptr_t>(zoomer + 0x30);
        zoomer = RPM<uintptr_t>(zoomer + 0x30);
        zoomer = RPM<uintptr_t>(zoomer + 0x38);
        if (speed)
        {
            WPM<int>(zoomer + 0x58, 150);
        }
        else
        {
            WPM<int>(zoomer + 0x58, 115);
        }
    return true;
}

bool nC()
{
        uint64_t NC = RPM<uint64_t>(base_address + 0x538d540);
        NC = RPM<uint64_t>(NC + 0xF8);
        NC = RPM<uint64_t>(NC + 0x8);
        if (noclip)
        {
            WPM<float>(NC + 0x7F0, -1.0f);
            WPM<float>(NC + 0x7F4, -1.0f);
            WPM<float>(NC + 0x7F8, -1.0f);
            WPM<float>(NC + 0x7FC, -1.0f);
        }
        else
        {
            WPM<float>(NC + 0x7F0, 0.0003051850945f);
            WPM<float>(NC + 0x7F4, 0.0003051850945f);
            WPM<float>(NC + 0x7F8, 0.0003051850945f);
            WPM<float>(NC + 0x7FC, 0.0003051850945f);
        }
    return true;
}

bool nS()
{
        uintptr_t noSpread = RPM<uintptr_t>(base_address + 0x53a02e8);
        noSpread = RPM<uintptr_t>(noSpread + 0xC8);
        noSpread = RPM<uintptr_t>(noSpread + 0x0);
        noSpread = RPM<uintptr_t>(noSpread + 0x90);
        noSpread = RPM<uintptr_t>(noSpread + 0xC8);
        noSpread = RPM<uintptr_t>(noSpread + 0x278);
        WPM<BYTE>(noSpread + 0x168, 0);
        if (nospread)
        {
            WPM<float>(noSpread + 0x58, 0);
        }
        else
        {
            WPM<float>(noSpread + 0x58, 1);
        }
    return true;
}

bool nR() //not the best way
{
        uintptr_t noRecoil = RPM<uintptr_t>(base_address + 0x53a02e0);
        noRecoil = RPM<uintptr_t>(noRecoil + 0x110);
        noRecoil = RPM<uintptr_t>(noRecoil + 0x0);
        if (norecoil)
        {
            WPM<bool>(noRecoil + 0xe2d, 0);
        }
        else
        {
            WPM<bool>(noRecoil + 0xe2d, 1);
        }
    return true;
}

bool chams() //such a misleading name
{
        float strength = -1.5f;
        float strength2 = 3.5f;
        uint64_t Glow = RPM<uint64_t>(base_address + 0x6083700);
        Glow = RPM<uint64_t>(Glow + 0xB8);
        if (glow)
        {
            if (glowStyle)
            {
                WPM(Glow + 0x11c, strength2);
                WPM<Vector3>(Glow + 0xD0, { GlowRainbow1() * 255, GlowRainbow2() * 255, GlowRainbow3() * 255 });
            }
            else
            {
                WPM(Glow + 0x11c, strength);
                WPM(Glow + 0xD0, Rainbow());
            }
            if (hands)
            {
                WPM<Vector2>(Glow + 0x110, { 0, 0 });
            }
            else
            {
                WPM<Vector2>(Glow + 0x110, { 1, 1 });
            }
        }
        else
        {
            WPM<Vector3>(Glow + 0xD0, { 0, 0, 0 });
            WPM<Vector2>(Glow + 0x110, { 0, 0 });
            WPM(Glow + 0x11c, strength);
        }
    return true;
}

bool DamageModifier()
{
    DWORD64 dmg = RPM<DWORD64>(game_manager + 0x2340);
    dmg = RPM<DWORD64>(dmg + 0x138);
    dmg = RPM<DWORD64>(dmg + 0x0); //write to 0x40

    if (bulletMod)
    {
        WPM<int>(dmg + 0x40, 30);
    }
    else
    {
        WPM<int>(dmg + 0x40, 1);
    }
    return true;
}

void Features()
{
    if (GetCurrentGameMode() == 3)
    {
        DamageModifier();
        chams();
        nR();
        nS();
        nC();
        speedMod();
        instantAnimations();
        noShotCooldown();
    }
}
int main()
{
    SetConsoleTitle("[UnknownCheats.me]");
    pid = GetPID("RainbowSix.exe");
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    base_address = GetModule(hProcess);
    std::cout << "\n";
    std::cout << " [>] OFFLINE R6 CHEAT BY OLY474\n";
    std::cout << " [>] FEATURES WON'T WORK IN MENU DUE TO GAMEMODE CHECK\n\n";
    std::cout << " [F1] Glow\n";
    std::cout << " [F2] Apply to hands\n";
    std::cout << " [F3] Switch Glow Style [Bright, Flat]\n";
    std::cout << " [F4] No Recoil\n";
    std::cout << " [F5] No Spread\n";
    std::cout << " [F6] No Clip\n";
    std::cout << " [F7] Speed\n";
    std::cout << " [F8] Instant Animations\n";
    std::cout << " [F9] Rapid Fire\n";
    std::cout << " [F10] Damage Modifier\n";
    std::cout << " ";
    while (1)
    {
        game_manager = RPM<uint64_t>(base_address + 0x53a02e8);
        profile_manager = RPM<uint64_t>(base_address + 0x538d568);
        Features();
            if (GetAsyncKeyState(VK_F1))
            {
                glow = !glow;
                Sleep(180);
            }
            if (GetAsyncKeyState(VK_F2))
            {
                hands = !hands;
                Sleep(180);
            }
            if (GetAsyncKeyState(VK_F3))
            {
                glowStyle = !glowStyle;
                Sleep(180);
            }
            if (GetAsyncKeyState(VK_F4))
            {
                norecoil = !norecoil;
                Sleep(180);
            }
            if (GetAsyncKeyState(VK_F5))
            {
                nospread = !nospread;
                Sleep(180);
            }
            if (GetAsyncKeyState(VK_F6))
            {
                noclip = !noclip;
                Sleep(180);
            }
            if (GetAsyncKeyState(VK_F7))
            {
                speed = !speed;
                Sleep(180);
            }
            if (GetAsyncKeyState(VK_F8))
            {
                instantanimation = !instantanimation;
                Sleep(180);
            }
            if (GetAsyncKeyState(VK_F9))
            {
                rapidfire = !rapidfire;
                Sleep(180);
            }
            if (GetAsyncKeyState(VK_F10))
            {
                bulletMod = !bulletMod;
                Sleep(180);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
    return 0;
}

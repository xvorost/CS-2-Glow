#include <d3d9.h>
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3d9.lib")
#include <thread>

#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui.h"
#include "ProcessManager.hpp"
#include "Game.h"
#include "Entity.h"

#define RGBA_TO_FLOAT(r,g,b,a) (float)r/255.0f, (float)g/255.0f, (float)b/255.0f, (float)a/255.0f

// Data
static IDirect3D9* g_pD3D = NULL;
static IDirect3DDevice9* g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND hwnd;
int WIDTH = 300;
int HEIGHT = 65;
POINTS position = { };

int prelogs_count = 0;
float on = 86400.f;
float off = 0.f;

bool EnableGlow = false;

void Glow() {
    while (true) {
        // Update EntityList Entry
        gGame.UpdateEntityListEntry();

        DWORD64 LocalControllerAddress = 0;
        DWORD64 LocalPawnAddress = 0;

        while (LocalControllerAddress == 0) {
            ProcessMgr.ReadMemory(gGame.GetLocalControllerAddress(), LocalControllerAddress);
            if (LocalControllerAddress == 0) Sleep(100);
        }
        ProcessMgr.ReadMemory(gGame.GetLocalPawnAddress(), LocalPawnAddress);

        // LocalEntity
        CEntity LocalEntity;
        static int LocalPlayerControllerIndex = 1;
        LocalEntity.UpdateController(LocalControllerAddress);
        LocalEntity.UpdatePawn(LocalPawnAddress);

        for (int i = 0; i < 64; i++)
        {
            CEntity Entity;
            DWORD64 EntityAddress = 0;
            ProcessMgr.ReadMemory<DWORD64>(gGame.GetEntityListEntry() + (i + 1) * 0x78, EntityAddress);
            if (EntityAddress == LocalEntity.Controller.Address)
            {
                LocalPlayerControllerIndex = i;
            }
            Entity.UpdateController(EntityAddress);
            Entity.UpdatePawn(Entity.Pawn.Address);

            if (!EnableGlow)
            {
                ProcessMgr.WriteMemory(Entity.Pawn.Address + Offset::Entity.EnemySensor, off);
            }
            else {
                ProcessMgr.WriteMemory(Entity.Pawn.Address + Offset::Entity.EnemySensor, on);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main(HINSTANCE hinst, DWORD fdwReason, LPVOID lpvReserved)
{
    FreeConsole();
    DisableThreadLibraryCalls(hinst);

    auto ProcessStatus = ProcessMgr.Attach("cs2.exe");
    if (ProcessStatus != StatusCode::SUCCEED)
    {
        MessageBox(NULL, L"Failed to attach \"cs2.exe\" process.", L"Error", MB_ICONERROR);
        return 0;
    }

    if (!Offset::UpdateOffsets())
    {
        MessageBox(NULL, L"Failed to update offsets.", L"Error", MB_ICONERROR);
        return 0;
    }

    if (!gGame.InitAddress())
    {
        MessageBox(NULL, L"Failed to call InitAddress().", L"Error", MB_ICONERROR);
        return 0;
    }

    std::thread run(Glow);

    RECT desktop;
    GetWindowRect(GetDesktopWindow(), &desktop);

    // Create application window
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L" ", NULL };
    ::RegisterClassEx(&wc);
    hwnd = ::CreateWindow(wc.lpszClassName, L" ", WS_POPUP, desktop.right / 2 - WIDTH / 2, desktop.bottom / 2 - HEIGHT / 2, WIDTH, HEIGHT, NULL, NULL, wc.hInstance, NULL);

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClass(wc.lpszClassName, wc.hInstance);
        exit(0);
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX9_Init(g_pd3dDevice);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGuiStyle& Style = ImGui::GetStyle();

    ImGui::GetStyle().WindowPadding = ImVec2(10.000f, 8.000f);
    Style.WindowTitleAlign = ImVec2(0.500f, 0.500f);

    Style.Colors[ImGuiCol_FrameBg] = ImVec4(0.141f, 0.141f, 0.141f, 0.667f);
    Style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.141f, 0.141f, 0.141f, 0.667f);
    Style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.141f, 0.141f, 0.141f, 0.667f);
    Style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.039f, 0.039f, 0.037f, 1.000f);
    Style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.141f, 0.141f, 0.141f, 0.667f);
    Style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.580f, 0.294f, 0.282f, 1.000f);
    Style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.643f, 0.329f, 0.318f, 1.000f);
    Style.Colors[ImGuiCol_CheckMark] = ImVec4(0.769f, 0.349f, 0.333f, 1.000f);
    Style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.580f, 0.294f, 0.282f, 1.000f);
    Style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.769f, 0.349f, 0.333f, 1.000f);
    Style.Colors[ImGuiCol_Button] = ImVec4(0.137f, 0.137f, 0.141f, 0.549f);
    Style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.580f, 0.294f, 0.282f, 1.000f);
    Style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.769f, 0.349f, 0.333f, 1.000f);
    Style.Colors[ImGuiCol_Header] = ImVec4(0.141f, 0.141f, 0.141f, 0.667f);
    Style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.580f, 0.294f, 0.282f, 1.000f);
    Style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.769f, 0.349f, 0.333f, 1.000f);
    Style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.580f, 0.294f, 0.282f, 0.549f);
    Style.Colors[ImGuiCol_Separator] = ImVec4(0.780f, 0.349f, 0.333f, 1.000f);
    Style.Colors[ImGuiCol_Tab] = ImVec4(0.137f, 0.137f, 0.141f, 0.549f);
    Style.Colors[ImGuiCol_TabHovered] = ImVec4(0.580f, 0.294f, 0.282f, 1.000f);
    Style.Colors[ImGuiCol_TabActive] = ImVec4(0.769f, 0.349f, 0.333f, 1.000f);
    Style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.137f, 0.137f, 0.141f, 0.549f);
    Style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.580f, 0.294f, 0.282f, 0.549f);

    // Main loop
    bool done = false;
    int cwr = 0;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        cwr++;

        // Start the Dear ImGui frame
        ImGui_ImplDX9_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT), ImGuiCond_Once);

        {
            ImGui::Begin("xvisual - CS 2 by xvorost", NULL, 18470);

            ImGui::Spacing();
            ImGui::Text(" ");
            ImGui::SameLine();
            ImGui::BulletText("Enable Glow:"); ImGui::SameLine();
            ImGui::Checkbox("##EnableGlow", &EnableGlow);
            ImGui::SameLine();
            ImGui::Text("        ");
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.769f, 0.349f, 0.333f, 1.000f));
            if (ImGui::Button("_")) ::ShowWindow(hwnd, SW_MINIMIZE);
            ImGui::PopStyleColor(1);
            ImGui::SameLine();
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.769f, 0.349f, 0.333f, 1.000f));
            if (ImGui::Button("X")) {
                ShellExecute(NULL, L"open", L"https://github.com/xvorost/", NULL, NULL, SW_SHOWNORMAL);
                exit(0);
            }
            ImGui::PopStyleColor(1);

            ImGui::End();
        }

        // Rendering
        if (cwr >= 1000) { ::UpdateWindow(hwnd); cwr = 0; }
        ImGui::EndFrame();
        g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
        g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
        D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * clear_color.w * 255.0f), (int)(clear_color.y * clear_color.w * 255.0f), (int)(clear_color.z * clear_color.w * 255.0f), (int)(clear_color.w * 255.0f));
        g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
        if (g_pd3dDevice->BeginScene() >= 0)
        {
            ImGui::Render();
            ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
            g_pd3dDevice->EndScene();
        }
        HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

        // Handle loss of D3D9 device
        if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
            ResetDevice();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    // Cleanup
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClass(wc.lpszClassName, wc.hInstance);
    //}

    return 0;
}

bool CreateDeviceD3D(HWND hWnd)
{
    if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
        return false;

    // Create the D3DDevice
    g_d3dpp.Windowed = true;
    g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    g_d3dpp.hDeviceWindow = hWnd;

    HRESULT dummyDeviceCreated = g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice);
    if (dummyDeviceCreated != S_OK)
    {
        g_pD3D->Release();
        Sleep(5000);
        return false;
    }

    return true;
}

void CleanupDeviceD3D()
{
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
    if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
    if (hr == D3DERR_INVALIDCALL)
        IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
        {
            g_d3dpp.BackBufferWidth = LOWORD(lParam);
            g_d3dpp.BackBufferHeight = HIWORD(lParam);
            ResetDevice();
        }
        return 0;
        break;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ShellExecute(NULL, L"open", L"https://github.com/xvorost/", NULL, NULL, SW_SHOWNORMAL);
        ::PostQuitMessage(0);
        return 0;
    case WM_LBUTTONDOWN:
        position = MAKEPOINTS(lParam); // set click points
        return 0;
    case WM_MOUSEMOVE:
        if (wParam == MK_LBUTTON)
        {
            const auto points = MAKEPOINTS(lParam);
            auto rect = ::RECT{ };

            GetWindowRect(hwnd, &rect);

            rect.left += points.x - position.x;
            rect.top += points.y - position.y;

            if (position.x >= 0 &&
                position.x <= WIDTH &&
                position.y >= 0 && position.y <= 19)
                SetWindowPos(
                    hwnd,
                    HWND_TOPMOST,
                    rect.left,
                    rect.top,
                    0, 0,
                    SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
                );
        }
        return 0;
    }

    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
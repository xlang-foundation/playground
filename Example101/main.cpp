#include <windows.h>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include "xload.h"
#include "xpackage.h"
#include "xlang.h"

int DrawObj(int x, int y, int w, int h, const char* objName);
void UpdateObj(int id, int x, int y);

//Begin XLang API Exports
class DrawSystem
{
    HWND m_hwnd;
public:
    BEGIN_PACKAGE(DrawSystem)
        APISET().AddEvent("OnTimer");
        APISET().AddFunc<5>("DrawObj", &DrawSystem::DrawObjAPI);
        APISET().AddFunc<3>("UpdateObj", &DrawSystem::UpdateObjAPI);
        APISET().AddFunc<2>("CreateTimer", &DrawSystem::CreateTimerAPI);
    END_PACKAGE

    inline int DrawObjAPI(int x, int y, int w, int h, std::string name)
    {
        return DrawObj(x, y, w, h, name.c_str());
    }
    inline bool UpdateObjAPI(int id, int x, int y)
    {
        UpdateObj(id, x, y);
        return true;
    }
    inline int CreateTimerAPI(int id, int interval)
    {
		return (int)SetTimer(m_hwnd, id, interval, NULL);
	}
    inline void SetWnd(HWND hwnd) { m_hwnd = hwnd; }
};

DrawSystem drawSystem;

//End

// Structure to hold drawing information
struct DrawItem {
    int x, y, w, h;
    std::string objName;
};

// Global variables
const char g_szClassName[] = "myWindowClass";
HWND hwnd; // Global variable for the window handle
std::map<int, DrawItem> drawMap; // Map to store drawing items with IDs
int nextId = 1; // Global ID counter

// Forward declarations of functions
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
int DrawObj(int x, int y, int w, int h, const char* objName);
void UpdateObj(int id, int x, int y);

// WinMain: Entry point for a Windows application
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    X::XLoad g_xload;

    X::Config config;
    char exePath[MAX_PATH];
    GetModuleFileName(NULL, exePath, MAX_PATH);

    std::string progName(exePath);
    auto pos = progName.rfind('\\');
    if (pos != progName.npos)
    {
        std::string strAppPath = progName.substr(0, pos);
        config.appPath = new char[strAppPath.length() + 1];
        memcpy((char*)config.appPath, strAppPath.data(), strAppPath.length() + 1);
    }
    config.appFullName = new char[progName.length() + 1];
    memcpy((char*)config.appFullName, progName.data(), progName.length() + 1);

	config.dbg = true;
    config.runEventLoopInThread = true;
    config.enterEventLoop = true;
    int retCode = g_xload.Load(&config);
    if (retCode == 0) {
        g_xload.Run();
    }
    X::RegisterPackage<DrawSystem>(config.appFullName,"DrawSystem", &drawSystem);

    //if you need to enalbe remoting, add line below
    //then can use import DrawSystem thru 'lrpc:101' to import DrawSystem in another process
    //X::g_pXHost->Lrpc_Listen(101, false);

    WNDCLASSEX wc;
    MSG Msg;

    // Registering the Window Class
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "A Simple Win32 App with XLang",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    drawSystem.SetWnd(hwnd);
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    //UINT_PTR timerID = SetTimer(hwnd, 0, 1000, NULL);

    // Adding test draw objects
    //int id1 = DrawObj(50, 50, 100, 20, "First Object");
    //int id2 = DrawObj(70, 80, 150, 20, "Second Object");

    // Optionally, update an object
    //UpdateObj(id1, 60, 60);

    // The Message Loop
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    g_xload.Unload();
    return Msg.wParam;
}

// WndProc: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        // Create and select font
        HFONT hFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
            OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"));
        HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

        // Draw all items in the map
        for (const auto& pair : drawMap) {
            const auto& item = pair.second;
            RECT rect;
            SetRect(&rect, item.x, item.y, item.x + item.w, item.y + item.h);
            DrawText(hdc, item.objName.c_str(), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        }
        // Clean up
        SelectObject(hdc, oldFont);
        DeleteObject(hFont);
        EndPaint(hwnd, &ps);
    }
    break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_TIMER:
    {
        int timerID = (int)wParam;
        X::ARGS args(1);
        args.push_back(timerID);
        X::KWARGS kwargs;
        //0 is the index of OnTimer event
        drawSystem.Fire(0, args, kwargs);
    }
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// DrawObj: Adds a draw item to the map and invalidates the window
int DrawObj(int x, int y, int w, int h, const char* objName)
{
    int id = nextId++;
    DrawItem item = { x, y, w, h, objName };
    drawMap[id] = item;
    InvalidateRect(hwnd, NULL, TRUE); // Invalidate the window to trigger a redraw
    return id;
}

// UpdateObj: Updates the position of an existing object
void UpdateObj(int id, int x, int y)
{
    if (drawMap.find(id) != drawMap.end()) {
        drawMap[id].x = x;
        drawMap[id].y = y;
        InvalidateRect(hwnd, NULL, TRUE); // Invalidate to redraw with updated positions
    }
}

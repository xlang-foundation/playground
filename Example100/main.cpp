#include <windows.h>
#include <map>
#include <string>
#include <vector>
#include <iostream>

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
    char exePath[MAX_PATH];
    GetModuleFileName(NULL, exePath, MAX_PATH);

    std::string progName(exePath);
    auto pos = progName.rfind('\\');
    if (pos != progName.npos)
    {
        std::string strAppPath = progName.substr(0, pos);
        char* appPath = new char[strAppPath.length() + 1];
        memcpy((char*)appPath, strAppPath.data(), strAppPath.length() + 1);
        std::cout << "AppPath:" << appPath << std::endl;
        delete [] appPath;
    }
    char* appFullName = new char[progName.length() + 1];
    memcpy((char*)appFullName, progName.data(), progName.length() + 1);
    std::cout << "AppFullName:" << appFullName << std::endl;
    delete [] appFullName;

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
        "A Simple Win32 App",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 640,480,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    UINT_PTR timerID = SetTimer(hwnd, 0, 1000, NULL);

    // Adding test draw objects
    int id1 = DrawObj(50, 50, 200, 50, "First Object");
    int id2 = DrawObj(200, 80, 250, 50, "Second Object");

    // Optionally, update an object
    UpdateObj(id1, 60, 60);

    // The Message Loop
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

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

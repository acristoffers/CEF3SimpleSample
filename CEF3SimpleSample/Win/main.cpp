/************************************************************************************************
*   Copyright (c) 2013 Álan Crí­stoffer
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
*   of the Software, and to permit persons to whom the Software is furnished to do so,
*   subject to the following conditions:
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
*   INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
*   PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
*   FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
*   OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
*   DEALINGS IN THE SOFTWARE.
************************************************************************************************/

#include <string>
#include <algorithm>
#include <windows.h>

#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "ClientApp.h"
#include "ClientHandler.h"

ClientHandler *g_handler = 0;

std::string GetApplicationDir()
{
    HMODULE hModule = GetModuleHandleW(NULL);
    WCHAR   wpath[MAX_PATH];

    GetModuleFileNameW(hModule, wpath, MAX_PATH);
    std::wstring wide(wpath);

    std::string path = CefString(wide);
    path = path.substr( 0, path.find_last_of("\\/") );
    return path;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch ( uMsg ) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_SIZE:
            if ( g_handler ) {
                // Resize the browser window and address bar to match the new frame
                // window size
                RECT rect;
                GetClientRect(hwnd, &rect);
                HDWP hdwp = BeginDeferWindowPos(1);
                hdwp = DeferWindowPos(hdwp, g_handler->GetBrowserHwnd(),
                                      NULL, rect.left, rect.top,
                                      rect.right - rect.left,
                                      rect.bottom - rect.top,
                                      SWP_NOZORDER);
                EndDeferWindowPos(hdwp);
            }

            break;

        case WM_ERASEBKGND:
            if ( g_handler ) {
                // Dont erase the background if the browser window has been loaded
                // (this avoids flashing)
                return 0;
            }

            break;

       case WM_PAINT:
            PAINTSTRUCT ps;
            HDC         hdc = BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HWND RegisterWindow(HINSTANCE hInstance, int nCmdShow)
{
    WNDCLASS wc = {};

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = "TestsDatabaseWindow";
    RegisterClass(&wc);
    HWND hwnd = CreateWindowEx(0,                                     // Optional window styles.
                               "TestsDatabaseWindow",                 // Window class
                               "CEF3 Simple Sample",                  // Window text
                               WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN, // Window style
                                                                      // Size and position
                               CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                               CW_USEDEFAULT,
                               NULL,      // Parent window
                               NULL,      // Menu
                               hInstance, // Instance handle
                               NULL       // Additional application data
                              );

    if ( hwnd == NULL ) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    return hwnd;
}

LRESULT CALLBACK MessageWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hWnd, message, wParam, lParam);
}

HWND CreateMessageWindow(HINSTANCE hInstance)
{
    WNDCLASSEX wc = {
        0
    };

    wc.cbSize        = sizeof(wc);
    wc.lpfnWndProc   = MessageWndProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = "ClientMessageWindow";
    RegisterClassEx(&wc);
    return CreateWindow("ClientMessageWindow", 0, 0, 0, 0, 0, 0, HWND_MESSAGE, 0, hInstance, 0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    CefMainArgs main_args(hInstance);

    CefRefPtr<ClientApp> app(new ClientApp);

    // Execute the secondary process, if any.
    int exit_code = CefExecuteProcess( main_args, app.get(), NULL );
    if ( exit_code >= 0 ) {
        exit(exit_code);
    }

    // Register the window class.
    HWND hwnd = RegisterWindow(hInstance, nCmdShow);
    if ( hwnd == 0 ) {
        return 0;
    }

    RECT rect;
    GetClientRect(hwnd, &rect);

    CefSettings settings;
    CefInitialize( main_args, settings, app.get(), NULL );
    CefWindowInfo        info;
    CefBrowserSettings   b_settings;
    CefRefPtr<CefClient> client(new ClientHandler);
    g_handler = (ClientHandler *) client.get();
    std::string               path         = "file://" + GetApplicationDir() + "/html/index.html";
    CefRefPtr<CefCommandLine> command_line = CefCommandLine::GetGlobalCommandLine();

    if ( command_line->HasSwitch("url") ) {
        path = command_line->GetSwitchValue("url");
    }

    info.SetAsChild(hwnd, rect);
    CefBrowserHost::CreateBrowser(info, client.get(), path, b_settings, NULL);
    int result = 0;

    if ( !settings.multi_threaded_message_loop ) {
        // Run the CEF message loop. This function will block until the application
        // recieves a WM_QUIT message.
        CefRunMessageLoop();
    } else {
        // Create a hidden window for message processing.
        HWND hMessageWnd = CreateMessageWindow(hInstance);
        MSG  msg;

        // Run the application message loop.
        while ( GetMessage(&msg, NULL, 0, 0) ) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        DestroyWindow(hMessageWnd);
        hMessageWnd = NULL;
        result      = static_cast<int> (msg.wParam);
    }

    CefShutdown();
    return result;
}

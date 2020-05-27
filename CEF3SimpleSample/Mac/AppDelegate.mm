/*******************************************************************************
 *   Copyright (c) 2013 Álan Crístoffer
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a
 *   copy of this software and associated documentation files (the "Software"),
 *   to deal in the Software without restriction, including without limitation
 *   the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *   and/or sell copies of the Software, and to permit persons to whom the
 *   Software is furnished to do so, subject to the following conditions:
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *   DEALINGS IN THE SOFTWARE.
 ******************************************************************************/

#import "AppDelegate.h"

#include "include/cef_app.h"
#include "include/cef_browser.h"

#include "ClientApp.h"
#include "ClientHandler.h"

@implementation AppDelegate

@synthesize window;

-(void)dealloc
{
    [super dealloc];
}

-(BOOL)applicationShouldTerminateAfterLastWindowClosed: (NSApplication *)sender
{
    return YES;
}

-(void)applicationDidFinishLaunching: (NSNotification *)aNotification
{
    CefRefPtr<ClientApp> app(new ClientApp);

    CefMainArgs main_args;
    CefSettings settings;

    int ret = CefExecuteProcess(main_args, app.get(), NULL);
    if (ret >= 0) exit(ret);

    CefInitialize(main_args, settings, app.get(), NULL);

    CefRefPtr<CefClient> client(new ClientHandler);

    CefWindowInfo        info;
    CefBrowserSettings   b_settings;

    std::string path;

    CefRefPtr<CefCommandLine> command_line = CefCommandLine::GetGlobalCommandLine();
    if ( command_line->HasSwitch("url") ) {
        path = command_line->GetSwitchValue("url");
    }

    if ( path.empty() ) {
        path = [[[NSBundle mainBundle] resourcePath] cStringUsingEncoding:NSUTF8StringEncoding];
        path = "file://" + path + "/html/index.html";
    }

    info.SetAsChild([window contentView], 0, 0, [[window contentView] frame].size.width, [[window contentView] frame].size.height);
    CefBrowserHost::CreateBrowser(info, client.get(), path, b_settings, NULL, NULL);

    CefRunMessageLoop();
    CefShutdown();
}

@end

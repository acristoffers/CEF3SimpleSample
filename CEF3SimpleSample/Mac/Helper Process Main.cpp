/************************************************************************************************
*   Copyright (c) 2013 Álan Crístoffer
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

#include "ClientApp.h"

#include "include/cef_app.h"
#include "include/wrapper/cef_library_loader.h"
#include "include/cef_sandbox_mac.h"

int main(int argc, char *argv[])
{
    CefScopedSandboxContext sandbox_context;
    if (!sandbox_context.Initialize(argc, argv))
      return 1;

    CefScopedLibraryLoader library_loader;
    if (!library_loader.LoadInHelper()) {
      return 1;
    }

    // Provide CEF with command-line arguments.
    CefMainArgs main_args(argc, argv);

    CefRefPtr<ClientApp> app(new ClientApp);

    // Execute the sub-process.
    return CefExecuteProcess(main_args, app, NULL);
}

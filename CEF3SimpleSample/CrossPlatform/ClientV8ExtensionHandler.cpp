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

#include "ClientV8ExtensionHandler.h"

ClientV8ExtensionHandler::ClientV8ExtensionHandler(CefRefPtr<CefApp> app)
{
    this->app = app;
}

bool ClientV8ExtensionHandler::Execute(const CefString &name, CefRefPtr<CefV8Value> object, const CefV8ValueList &arguments, CefRefPtr<CefV8Value> &retval, CefString &exception)
{
    if ( name == "ChangeTextInJS" ) {
        if ( (arguments.size() == 1) && arguments[0]->IsString() ) {
            CefString           text   = arguments[0]->GetStringValue();
            CefRefPtr<CefFrame> frame  = CefV8Context::GetCurrentContext()->GetBrowser()->GetMainFrame();
            std::string         jscall = "ChangeText('";
            jscall += text;
            jscall += "');";
            frame->ExecuteJavaScript(jscall, frame->GetURL(), 0);
            /*
             * If you want your method to return a value, just use retval, like this:
             * retval = CefV8Value::CreateString("Hello World!");
             * you can use any CefV8Value, what means you can return arrays, objects or whatever you can create with CefV8Value::Create* methods
             */
            return true;
        }
    }

    return false;
}

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

#include <fstream>
#include <stdlib.h>
#include <string>
#include <unistd.h>

#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <X11/Xlib.h>
#undef Status   // Definition conflicts with cef_urlrequest.h
#undef Success  // Definition conflicts with cef_message_router.h

#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_frame.h"
#include "include/cef_runnable.h"

#include "ClientHandler.h"
#include "ClientApp.h"

void TerminationSignalHandler(int signatl)
{
    CefQuitMessageLoop();
}

static gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    CefShutdown();
    return TRUE;
}

GdkPixbuf *create_pixbuf(const gchar *filename)
{
    GdkPixbuf *pixbuf;
    GError    *error = NULL;

    pixbuf = gdk_pixbuf_new_from_file(filename, &error);
    if ( !pixbuf ) {
        fprintf(stderr, "%s\n", error->message);
        g_error_free(error);
    }

    return pixbuf;
}

std::string getExecutableBaseFolder()
{
    std::string path    = "";
    pid_t       pid     = getpid();
    char        buf[20] = {
        0
    };

    sprintf(buf, "%d", pid);
    std::string _link = "/proc/";
    _link.append(buf);
    _link.append("/exe");
    char proc[512];
    int  ch = readlink(_link.c_str(), proc, 512);
    if ( ch != -1 ) {
        proc[ch] = 0;
        path     = proc;
        std::string::size_type t = path.find_last_of("/");
        path = path.substr(0, t);
    }

    return path;
}

int main(int argc, char **argv)
{
    CefMainArgs main_args(argc, argv);

    CefRefPtr<ClientApp> app(new ClientApp);

    // Execute the secondary process, if any.
    int exit_code = CefExecuteProcess( main_args, app.get(), NULL );
    if ( exit_code >= 0 ) {
        exit(exit_code);
    }

    GtkWidget *window;

    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    g_signal_connect(window, "delete-event", G_CALLBACK(delete_event), NULL);

    GtkWidget *vbox = gtk_vbox_new(FALSE, 0);

    CefSettings settings;
    settings.no_sandbox = true;

    CefInitialize( main_args, settings, app.get(), NULL );

    CefWindowInfo        info;
    CefBrowserSettings   b_settings;
    CefRefPtr<CefClient> client(new ClientHandler);

    std::string path     = getExecutableBaseFolder();
    std::string app_icon = path + "/html/img/icon.png";
    path = std::string("file://") + path + std::string("/html/index.html");

    CefRefPtr<CefCommandLine> command_line = CefCommandLine::GetGlobalCommandLine();
    if ( command_line->HasSwitch("url") ) {
        path = command_line->GetSwitchValue("url");
    }

    gtk_window_set_icon( GTK_WINDOW(window), create_pixbuf( app_icon.c_str() ) );
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_show_all( GTK_WIDGET(window) );

    ::Window xwindow = GDK_WINDOW_XID(gtk_widget_get_window(window));

    CefRect rect(0, 0, 800, 600);
    info.SetAsChild(xwindow, rect);

    CefBrowserHost::CreateBrowser(info, client.get(), path, b_settings, NULL);

    signal(SIGINT,  TerminationSignalHandler);
    signal(SIGTERM, TerminationSignalHandler);

    CefRunMessageLoop();

    return 0;
}

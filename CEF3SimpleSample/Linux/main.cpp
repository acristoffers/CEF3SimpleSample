#include <fstream>
#include <stdlib.h>
#include <string>
#include <unistd.h>

#include <gtk/gtk.h>

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
    int exit_code = CefExecuteProcess( main_args, app.get() );
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

    CefInitialize( main_args, settings, app.get() );

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

    info.SetAsChild(vbox);

    CefBrowserHost::CreateBrowser(info, client.get(), path, b_settings);

    gtk_window_set_icon( GTK_WINDOW(window), create_pixbuf( app_icon.c_str() ) );
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_widget_show_all( GTK_WIDGET(window) );

    signal(SIGINT,  TerminationSignalHandler);
    signal(SIGTERM, TerminationSignalHandler);

    CefRunMessageLoop();

    return 0;
}

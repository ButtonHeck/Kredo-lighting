#include "core/application.h"
#include "core/main_window.h"

namespace Kredo
{

bool Application::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    MainWindow* mainWindow = new MainWindow();
    mainWindow->Show(true);

    return true;
}

}

wxIMPLEMENT_APP(Kredo::Application);

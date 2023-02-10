#include "application.h"
#include "main_window.h"

#include <wx/fileconf.h>

namespace Kredo
{

bool Application::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    wxMkDir(KREDO_CONFIG_DIR, wxS_DIR_DEFAULT);

    wxFileConfig* config = new wxFileConfig(wxEmptyString, wxEmptyString, wxString::Format("%s/%s", KREDO_CONFIG_DIR, "config.ini"));
    wxConfigBase::Set(config);

    MainWindow* mainWindow = new MainWindow();
    mainWindow->Show(true);

    return true;
}

}

wxIMPLEMENT_APP(Kredo::Application);

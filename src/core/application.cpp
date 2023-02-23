#include "application.h"
#include "main_window.h"

#include <wx/fileconf.h>
#include "wx/uilocale.h"
#include <wx/image.h>

namespace Kredo
{

bool Application::OnInit()
{
    if (!wxApp::OnInit())
        return false;

    wxMkDir(KREDO_CONFIG_DIR, wxS_DIR_DEFAULT);
    wxInitAllImageHandlers();

    if (!LoadTranslations())
        wxLogError("Could not load translation");

    wxFileConfig* config = new wxFileConfig(wxEmptyString, wxEmptyString, wxString::Format("%s/%s", KREDO_CONFIG_DIR, "config.ini"));
    wxConfigBase::Set(config);

    MainWindow* mainWindow = new MainWindow();
    mainWindow->Show(true);

    return true;
}

bool Application::LoadTranslations()
{
    if (!wxUILocale::UseDefault())
        wxLogWarning("Failed to initialize the default system locale.");

    wxFileTranslationsLoader::AddCatalogLookupPathPrefix("locale");
    wxTranslations* const translation = new wxTranslations();
    wxTranslations::Set(translation);

    if (!translation->AddCatalog("Kredo"))
    {
        wxLogError("Couldn't find/load 'Kredo' catalog");
        return false;
    }

    return true;
}

}

wxIMPLEMENT_APP(Kredo::Application);

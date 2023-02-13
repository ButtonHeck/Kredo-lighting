#pragma once

#include <wx/app.h>

namespace Kredo
{

class Application : public wxApp
{
public:
    virtual bool OnInit() override;

private:
    bool LoadTranslations();
};

}

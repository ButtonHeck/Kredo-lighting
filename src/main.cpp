#include <wx/wx.h>
#include <wx/app.h>

class TestFrame : public wxFrame
{
public:
    TestFrame(const wxString& title)
        : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(1024, 768))
    {
        SetMinSize(wxSize(250, 200));
    }
};

class TestApp : public wxApp
{
public:
    virtual bool OnInit() override
    {
        if (!wxApp::OnInit())
            return false;

        TestFrame* frame = new TestFrame("Test");
        frame->Show(true);

        return true;
    }
};

wxIMPLEMENT_APP(TestApp);

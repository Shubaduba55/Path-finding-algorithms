#include "cApp.h"

wxIMPLEMENT_APP(cApp);

cApp::cApp()
{
    main_window = new cMain();

}

cApp::~cApp()
{
}

bool cApp::OnInit()
{
    main_window->Show();
    return true;
}

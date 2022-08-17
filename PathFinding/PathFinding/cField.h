#pragma once

#include "wx/wx.h"
#include <string>

using std::string;

class cField : public wxFrame
{
private:
	int nFieldWidth = 26;
	int nFieldHeight = 36;
	wxButton*** btn;

public:
	cField();
	~cField();

	void OnButtonClicked(wxCommandEvent&);

	wxDECLARE_EVENT_TABLE();

};


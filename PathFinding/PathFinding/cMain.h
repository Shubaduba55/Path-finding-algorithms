#pragma once
#include <wx\wx.h>
#include <wx/wfstream.h> //for wxFileInputStream
#include "BinaryFile.h"
#include "Graph.h"



class cMain: public wxFrame
{
private:
	//m_txt1 = new wxTextCtrl(this, wxID_ANY, "", wxPoint(10, 70), wxSize(300, 30));
	wxTextCtrl* rows;//10001
	wxTextCtrl* clms;//10002
	wxTextCtrl* obst;//10003

	wxButton* btn_create_graph;//11001
	wxButton* btn_load;//11002
	wxButton* btn_save;//11003

	wxArrayString choices;
	wxChoice* choice_algorithm;//12001
	
	wxTextCtrl* start_n;//10004
	wxTextCtrl* end_n;//10005

	wxButton* btn_find_path;//11004

	wxFrame* field = nullptr; wxButton*** btn = nullptr;
	Graph graph;

	

	void OnClickCreateGraph(wxCommandEvent&);
	void OnClickFindPath(wxCommandEvent&);
	void OnClickLoad(wxCommandEvent&);
	void OnClickSave(wxCommandEvent&);

	void OnButtonField(wxCommandEvent&);

	wxDECLARE_EVENT_TABLE();
public:
	cMain();
	~cMain();

	void delete_field();
	void create_field(int, int);
	void to_colour_field();
	void set_cell_colour(int, wxColor&);
};


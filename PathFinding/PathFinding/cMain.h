#pragma once
#include <wx\wx.h>
#include "BinaryFile.h"
#include "Graph.h"



class cMain: public wxFrame
{
private:
	
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

	void delete_field();
	void create_field(int, int);
	void to_colour_field();
	void set_cell_colour(int, wxColor&);
public:
	cMain();
	~cMain();
};


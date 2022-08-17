#include "cMain.h"
//#include <wx/listctrl.h>

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
EVT_BUTTON(1, cMain::OnButtonField)
EVT_BUTTON(11001, cMain::OnClickCreateGraph)
EVT_BUTTON(11004, cMain::OnClickFindPath)
EVT_BUTTON(11002, cMain::OnClickLoad)
EVT_BUTTON(11003, cMain::OnClickSave)
wxEND_EVENT_TABLE()




cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Path Finding", wxPoint(300, 150), wxSize(800, 600))
{
	wxPanel* panel = new wxPanel(this, -1, wxDefaultPosition, wxSize(610, 300));

	wxStaticText* t_rows = new wxStaticText(panel, wxID_ANY, "rows:", wxPoint(15, 30));
	rows = new wxTextCtrl(panel, 10001, "", wxPoint(50, 30), wxSize(20, 20));

	wxStaticText* t_clms = new wxStaticText(panel, wxID_ANY, "clms:", wxPoint(80, 30));
	clms = new wxTextCtrl(panel, 10002, "", wxPoint(115, 30), wxSize(20, 20));

	wxStaticText* t_obst = new wxStaticText(panel, wxID_ANY, "obstacles: ", wxPoint(145, 30));
	obst = new wxTextCtrl(panel, 10003, "0", wxPoint(205, 30), wxSize(20, 20));


	btn_create_graph = new wxButton(panel, 11001, "Create Graph", wxPoint(15, 60), wxSize(210, 50));
	btn_load = new wxButton(panel, 11002, "Load", wxPoint(15, 120), wxSize(210, 50));
	btn_save = new wxButton(panel, 11003, "Save", wxPoint(15, 180), wxSize(210, 50));


	choices.Add("Dijkstra"); choices.Add("A*(euclidian heuristic)"); choices.Add("A*(manhattan heuristic)");
	choice_algorithm = new wxChoice(panel, 12001, wxPoint(400, 30), wxSize(200, 25), choices);
	

	wxStaticText* t_start_n = new wxStaticText(panel, wxID_ANY, "start node:", wxPoint(400, 65));
	start_n = new wxTextCtrl(panel, 10004, "", wxPoint(465, 60), wxSize(25, 20));


	wxStaticText* t_end_n = new wxStaticText(panel, wxID_ANY, "end node:", wxPoint(500, 65));
	end_n = new wxTextCtrl(panel, 10005, "", wxPoint(565, 60), wxSize(25, 20));

	btn_find_path = new wxButton(panel, 11004, "Find Path", wxPoint(400, 85), wxSize(200, 50));

	wxBoxSizer* vsizer = new wxBoxSizer(wxVERTICAL);
	vsizer->Add(panel, 1, wxEXPAND);
	
	this->SetSizerAndFit(vsizer);
}

void cMain::OnClickCreateGraph(wxCommandEvent& evt)
{
	int int_rows = wxAtoi(rows->GetValue());
	int int_columns = wxAtoi(clms->GetValue());
	int int_obstacles = wxAtoi(obst->GetValue());
	if (int_rows < 2 || int_columns < 2) throw "The number of rows and columns must be more than 2";

	//if (&graph != nullptr) delete &graph;
	//graph.create_graph(int_rows, int_columns, int_obstacles);
	//graph.visualize();

	wxString text;
	//text << int_rows << int_columns << int_obstacles;
	//wxFrame* tmp = new wxFrame(nullptr, wxID_ANY, text, wxPoint(300, 150), wxSize(800, 600));
	//tmp->Show(true);

	if (btn != nullptr && field != nullptr) delete_field();
	graph.create_graph(int_rows, int_columns, int_obstacles);
	create_field(int_rows, int_columns);
	graph.visualize();
	evt.Skip();
}

void cMain::OnClickFindPath(wxCommandEvent& evt)
{
	int start_node = wxAtoi(start_n->GetValue());
	int end_node = wxAtoi(end_n->GetValue());
	int size = graph.get_rows() * graph.get_columns();
	int algorithm_option = choice_algorithm->GetSelection() + 1;

	if (0 > start_node || start_node > size || 0 > end_node || end_node > size) throw "id is out of range";
	if (!graph.get_node(start_node).is_node_walkable() ||
		!graph.get_node(end_node).is_node_walkable()) throw "nodes must not be obstacles";

	graph.find_path(start_node, end_node, algorithm_option);
	to_colour_field();
	graph.visualize();
	evt.Skip();
}

void cMain::OnClickLoad(wxCommandEvent& WXUNUSED(event))
{
	if (false)
	{
		if (wxMessageBox(_("Current content has not been saved! Proceed?"), _("Please confirm"),
			wxICON_QUESTION | wxYES_NO, this) == wxNO)
			return;
		
	}

	wxFileDialog 
		openFileDialog(this, _("Open graph file"), "C:\Path-finding-algorithms\PathFinding\Saved/files", "",
			"GPH files (*.gph)|*.gph", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL) return;     

	
	wxFileInputStream input_stream(openFileDialog.GetPath());
	if (!input_stream.IsOk())
	{
		wxLogError("Cannot open file '%s'.", openFileDialog.GetPath());
		return;
	}
	
	else {
		BinaryFile file(openFileDialog.GetPath().ToStdString());
		
		int a = 0;
		if (btn != nullptr && field != nullptr) delete_field();
		try {
			graph = file.read_graph();
		}
		catch (const char* text) {
			a++;
			return;
		}
		create_field(graph.get_rows(), graph.get_columns());
		graph.visualize();

	}

}

void cMain::OnClickSave(wxCommandEvent& WXUNUSED(event))
{
	if (graph.get_rows() * graph.get_columns() == 0) throw "graph is empty";
	if (wxWindow::FindWindowByName("Field") == nullptr) throw "there is no graph";

	wxFileDialog
		saveFileDialog(this, _("Save GPH file"), "", "",
			"GPH files (*.gph)|*.gph", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL)
		return;    

	
	wxFileOutputStream output_stream(saveFileDialog.GetPath());
	if (!output_stream.IsOk())
	{
		wxLogError("Cannot save current contents in file '%s'.", saveFileDialog.GetPath());
		return;
	}
	else {
		BinaryFile file(saveFileDialog.GetPath().ToStdString());
		file.write_graph(graph);

	}
}

void cMain::OnButtonField(wxCommandEvent& evt)
{
	/*
	m_list1->AppendString(m_txt1->GetValue());
	m_txt1->Clear();
	evt.Skip();//Event handled
	*/
	int cell_id = evt.GetId() - 1000;
	int row = cell_id / graph.get_columns();
	int col = cell_id % graph.get_columns();

	if (btn[row][col]->GetBackgroundColour() != wxT("#5b5b5b")) {
		btn[row][col]->SetBackgroundColour(wxT("#5b5b5b"));
		//btn[row][col]->SetForegroundColour(*wxWHITE);
	}
	else {
		btn[row][col]->SetBackgroundColour(*wxWHITE);
		//btn[row][col]->SetForegroundColour(*wxBLACK);
	};
	graph.get_node(cell_id).change_walkable_status();


	//btn[row][col]->SetBackgroundColour(*wxBLACK);
	//btn[row][col]->SetForegroundColour(*wxWHITE);
	graph.visualize();
	evt.Skip();
	return;
}

cMain::~cMain()
{
}

void cMain::delete_field()
{
	if (wxFrame::FindWindowByName("Field") != nullptr) {
		if (!field->IsBeingDeleted()) {
			field->Destroy();
			field->DestroyChildren();
			/*
				int rows = graph.get_rows();
				int clms = graph.get_columns();
				for (int i = 0; i < rows; i++)
				{
					for (int j = 0; j < clms; j++)
					{
						delete btn[i][j];
					}
					delete[] btn[i];
				}
				delete[] btn; delete field;
				*/
		}
	}
}

void cMain::create_field(int nFieldHeight, int nFieldWidth)
{
	
	field = new wxFrame(this, wxID_ANY, "Field", wxPoint(0, 0), wxSize(2000, 2000));
	
	btn = new wxButton** [nFieldHeight];

	wxGridSizer* grid = new wxGridSizer(nFieldHeight, nFieldWidth, 0, 0);
	int id = 1000;
	for (int x = 0; x < nFieldHeight; x++)
	{
		btn[x] = new wxButton * [nFieldWidth];
		for (int y = 0; y < nFieldWidth; y++)
		{


			//btn[x][y] = new wxButton(this, (x * nFieldWidth + y), std::to_string((x * nFieldWidth + y)));
			btn[x][y] = new wxButton(field, id, std::to_string(id - 1000), wxDefaultPosition, wxSize(50, 50));
			//btn[x][y]->SetSize(wxSize(0, 0));

			Node& current_cell = graph.get_node(id - 1000);
			if(current_cell.is_node_walkable())	btn[x][y]->SetBackgroundColour(*wxWHITE);
			else {
				//btn[x][y]->SetBackgroundColour(*wxBLACK);
				//btn[x][y]->SetForegroundColour(*wxWHITE);
				btn[x][y]->SetBackgroundColour(wxT("#5b5b5b"));
				//btn[x][y]->SetForegroundColour(*wxWHITE);
			}
			id++;
			
			grid->Add(btn[x][y], 1, wxEXPAND | wxALL);
			btn[x][y]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cMain::OnButtonField, this);
		}
	}

	//field->SetSizer(grid);
	field->SetSizerAndFit(grid);
	field->Show(true);
}

void cMain::to_colour_field()
{
	int size = graph.get_rows() * graph.get_columns();
	for (int i = 0; i < size; i++)
	{
		Node& node = graph.get_node(i);
		wxColour path, visited, walkable, obstacle;
		path.Set(wxT("#81BEF7"));
		visited.Set(wxT("#58FAAC"));
		walkable.Set(wxT("#ffffff"));
		obstacle.Set(wxT("#5b5b5b"));//#9999

		if (node.is_node_path()) set_cell_colour(i, path);
		else if (node.is_node_visited()) set_cell_colour(i, visited);
		else {
			if (node.is_node_walkable()) set_cell_colour(i, walkable);
			else set_cell_colour(i, obstacle);
		};
		
	}

}

void cMain::set_cell_colour(int cell_id, wxColor& colour)
{
	int row = cell_id / graph.get_columns();
	int col = cell_id % graph.get_columns();
	btn[row][col]->SetBackgroundColour(colour);
}


/*
wxPanel* panel_top = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
	panel_top->SetBackgroundColour(wxColor(100, 100, 200));

	wxPanel* panel_bottom = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
	panel_bottom->SetBackgroundColour(wxColor(200, 100, 100));

	wxPanel* panel_bottom_right = new wxPanel(panel_bottom, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
	panel_bottom_right->SetBackgroundColour(wxColor(100, 200, 100));


	wxBoxSizer* vsizer = new wxBoxSizer(wxVERTICAL);
	vsizer->Add(panel_top, 1, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
	vsizer->Add(panel_bottom, 1, wxEXPAND | wxALL, 10);
	wxBoxSizer* hsizer = new wxBoxSizer(wxVERTICAL);
	//hsizer->Add(panel_bottom, 1, wxEXPAND | wxRIGHT, 10);
	hsizer->Add(panel_bottom_right, 1, wxALIGN_RIGHT | wxALL, 10);

	panel_bottom->SetSizerAndFit(hsizer);

	this->SetSizerAndFit(vsizer);

*/

/*

wxListView* list = new wxListView(this, wxID_ANY, wxDefaultPosition, wxSize(300, 200));
	list->InsertColumn(0, "Name");
	list->InsertItem(0, "Item");

	wxButton* ok_button = new wxButton(this, wxID_ANY, "Ok");
	wxButton* cancel_button = new wxButton(this, wxID_ANY, "Cancel");

	wxBoxSizer* s1 = new wxBoxSizer(wxVERTICAL);
	s1->Add(list, 1, wxEXPAND | wxALL, 5);


	wxBoxSizer* s2 = new wxBoxSizer(wxHORIZONTAL);
	s2->Add(ok_button, 0, wxRIGHT, 5);
	s2->Add(cancel_button, 0);

	s1->Add(s2, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 5);

	this->SetSizerAndFit(s1);

*/

/*

wxColour col1, col2;
	col1.Set(wxT("#4f2000"));
	col2.Set(wxT("#ededed"));

	wxPanel* panel = new wxPanel(this, -1);
	panel->SetBackgroundColour(col1);
	wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

	wxPanel* midPan = new wxPanel(panel, wxID_ANY);
	midPan->SetBackgroundColour(col2);

	vbox->Add(midPan, 2, wxEXPAND | wxALL, 30);
	panel->SetSizer(vbox);

	Centre();
*/
#include "cMain.h"


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
	obst = new wxTextCtrl(panel, 10003, "0", wxPoint(200, 30), wxSize(35, 20));


	btn_create_graph = new wxButton(panel, 11001, "Create Graph", wxPoint(15, 60), wxSize(210, 50));
	btn_load = new wxButton(panel, 11002, "Load", wxPoint(15, 120), wxSize(210, 50));
	btn_save = new wxButton(panel, 11003, "Save", wxPoint(15, 180), wxSize(210, 50));


	choices.Add("Dijkstra"); choices.Add("A*(euclidian heuristic)"); choices.Add("A*(manhattan heuristic)");
	choice_algorithm = new wxChoice(panel, 12001, wxPoint(400, 30), wxSize(200, 25), choices);
	

	wxStaticText* t_start_n = new wxStaticText(panel, wxID_ANY, "start node:", wxPoint(400, 65));
	start_n = new wxTextCtrl(panel, 10004, "", wxPoint(460, 60), wxSize(35, 20));


	wxStaticText* t_end_n = new wxStaticText(panel, wxID_ANY, "end node:", wxPoint(500, 65));
	end_n = new wxTextCtrl(panel, 10005, "", wxPoint(560, 60), wxSize(35, 20));

	btn_find_path = new wxButton(panel, 11004, "Find Path", wxPoint(400, 85), wxSize(200, 50));

	wxBoxSizer* vsizer = new wxBoxSizer(wxVERTICAL);
	vsizer->Add(panel, 1, wxEXPAND);
	
	this->SetSizerAndFit(vsizer);
}

void cMain::OnClickCreateGraph(wxCommandEvent& evt)
{
	wxString str_rows = rows->GetValue(), 
		str_columns = clms->GetValue(), 
		str_obstacles = obst->GetValue();

	if (str_rows.IsEmpty() || str_columns.IsEmpty() || str_obstacles.IsEmpty()) {
		wxMessageBox(_("Error: Not enough info given"), _(""), wxICON_ERROR | wxOK, this);
		return;
	}

	if (!(str_rows.IsNumber() && str_columns.IsNumber() && str_obstacles.IsNumber())) {
		wxMessageBox(_("Error: Given info is not numbers"), _(""), wxICON_ERROR | wxOK, this);
		return;
	}


	int int_rows = wxAtoi(str_rows);
	int int_columns = wxAtoi(str_columns);
	int int_obstacles = wxAtoi(str_obstacles);
	

	if (btn != nullptr && field != nullptr) delete_field();

	try { graph.create_graph(int_rows, int_columns, int_obstacles);}
	catch(const char* text){
		wxMessageBox(_(text), _(""), wxICON_ERROR | wxOK, this);
		return;
	}

	try { create_field(int_rows, int_columns); }
	catch (const char* text) {
		wxMessageBox(_(text), _(""), wxICON_ERROR | wxOK, this);
		return;
	}

	
	graph.visualize();
	evt.Skip();
}

void cMain::OnClickFindPath(wxCommandEvent& evt)
{
	if (wxWindow::FindWindowByName("Field") == nullptr) {
		wxMessageBox(_("Error: Field doesn't exist"), _(""), wxICON_ERROR | wxOK, this);
		return;
	}


	wxString str_start = start_n->GetValue(),
		str_end = end_n->GetValue();

	if (str_start.IsEmpty() || str_end.IsEmpty()) {
		wxMessageBox(_("Error: Not enough info given"), _(""), wxICON_ERROR | wxOK, this);
		return;
	}

	if (!(str_start.IsNumber() && str_end.IsNumber())) {
		wxMessageBox(_("Error: Given info is not numbers"), _(""), wxICON_ERROR | wxOK, this);
		return;
	}

	int algorithm_option = choice_algorithm->GetSelection() + 1;
	if (algorithm_option == 0){
		wxMessageBox(_("Error: The algorithm has not been chosen"), _(""), wxICON_ERROR | wxOK, this);
		return;
	}
	
	int start_node = wxAtoi(str_start);
	int end_node = wxAtoi(str_end);
	int size = graph.get_rows() * graph.get_columns();
	
	if (size == 0) {
		wxMessageBox(_("Error: Graph doesn't exist"), _(""), wxICON_ERROR | wxOK, this);
		return;
	}
	
	//std::thread th1(&cMain::to_colour_field);
	wxString info;
	try {
		info = graph.find_path(start_node, end_node, algorithm_option);
	}
	catch (const char* text) {
		wxMessageBox(_(text), _(""), wxICON_ERROR | wxOK, this);
		return;
	}
	//th1.join();

	try {
		to_colour_field();
	}
	catch (const char* text) {
		wxMessageBox(_(text), _(""), wxICON_ERROR | wxOK, this);
		return;
	}
	

	wxMessageBox(_(info), _(""), wxICON_INFORMATION | wxOK, this);
	graph.visualize();
	evt.Skip();
}

void cMain::OnClickLoad(wxCommandEvent& WXUNUSED(event))
{
	//if (wxMessageBox(_("Current content has not been saved! Proceed?"), _("Please confirm"), wxICON_QUESTION | wxYES_NO, this) == wxNO)
			
	wxFileDialog openFileDialog(this, _("Open graph file"), "C:\Path-finding-algorithms\PathFinding\Saved/files", "",
			"GPH files (*.gph)|*.gph", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL) return;     

		
	BinaryFile file(openFileDialog.GetPath().ToStdString());	
		
	if (btn != nullptr && field != nullptr) delete_field();

	try {graph = file.read_graph();}
	catch (const char* text) {
		wxMessageBox(_(text), _(""), wxICON_ERROR | wxOK, this);
		return;
	}

	create_field(graph.get_rows(), graph.get_columns());
}

void cMain::OnClickSave(wxCommandEvent& WXUNUSED(event))
{
	
	if (wxWindow::FindWindowByName("Field") == nullptr) {
		wxMessageBox(_("Error: Graph doesn't exist"), _(""), wxICON_ERROR | wxOK, this);
		return;
	}

	wxFileDialog saveFileDialog(this, _("Save GPH file"), "", "",
			"GPH files (*.gph)|*.gph", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

	if (saveFileDialog.ShowModal() == wxID_CANCEL) return;    

	BinaryFile file(saveFileDialog.GetPath().ToStdString());
	try { file.write_graph(graph); }
	catch (const char* text) {
		wxMessageBox(_(text), _(""), wxICON_ERROR | wxOK, this); 
		return;
	}
	
}

void cMain::OnButtonField(wxCommandEvent& evt)
{
	int cell_id = evt.GetId() - 1000;
	int row = cell_id / graph.get_columns();
	int col = cell_id % graph.get_columns();

	if (btn[row][col]->GetBackgroundColour() != wxT("#5b5b5b")) 

		btn[row][col]->SetBackgroundColour(wxT("#5b5b5b"));

	else
		btn[row][col]->SetBackgroundColour(*wxWHITE);

	graph.get_node(cell_id).change_walkable_status();
	//graph.visualize();
	evt.Skip();
	return;
}

cMain::~cMain(){}

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
	if (nFieldHeight <= 0 || nFieldWidth <= 0) throw "Error: Field size cannot be zero or less";

	field = new wxFrame(this, wxID_ANY, "Field", wxPoint(0, 0), wxSize(2000, 2000));
	
	btn = new wxButton** [nFieldHeight];

	wxGridSizer* grid = new wxGridSizer(nFieldHeight, nFieldWidth, 0, 0);
	int id = 1000;
	for (int x = 0; x < nFieldHeight; x++)
	{
		btn[x] = new wxButton * [nFieldWidth];
		for (int y = 0; y < nFieldWidth; y++)
		{
			btn[x][y] = new wxButton(field, id, std::to_string(id - 1000), wxDefaultPosition, wxSize(50, 50));

			Node& current_cell = graph.get_node(id - 1000);
			if(current_cell.is_node_walkable())	btn[x][y]->SetBackgroundColour(*wxWHITE);
			else btn[x][y]->SetBackgroundColour(wxT("#5b5b5b"));
				
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
	if (wxWindow::FindWindowByName("Field") == nullptr) throw "Error: Field doesn't exist";

	int size = graph.get_rows() * graph.get_columns();


	wxColour path, visited, walkable, obstacle;
	path.Set(wxT("#81BEF7"));
	visited.Set(wxT("#58FAAC"));
	walkable.Set(wxT("#ffffff"));
	obstacle.Set(wxT("#5b5b5b"));//#9999


	for (int i = 0; i < size; i++)
	{
		Node& node = graph.get_node(i);

		if (node.is_node_path()) set_cell_colour(i, path);
		else if (node.is_node_visited()) set_cell_colour(i, visited);
		else {
			if (node.is_node_walkable()) set_cell_colour(i, walkable);
			else set_cell_colour(i, obstacle);
		};
		//std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

}

void cMain::set_cell_colour(int cell_id, wxColor& colour)
{

	//if (wxWindow::FindWindowByName("Field") == nullptr) throw "Error: Field doesn't exist";
	//This exception takes too much time to draw field, besides it is not necessary as we check for this mistake
	//in to_colour_field() 

	if (graph.get_columns() * graph.get_rows() == 0) throw "Error: Graph doesn't exist";
	
	int row = cell_id / graph.get_columns();
	int col = cell_id % graph.get_columns();
	btn[row][col]->SetBackgroundColour(colour);
}



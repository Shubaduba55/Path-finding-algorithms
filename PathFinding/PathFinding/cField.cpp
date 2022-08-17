#include "cField.h"

wxBEGIN_EVENT_TABLE(cField, wxFrame)
EVT_BUTTON(1, OnButtonClicked)
wxEND_EVENT_TABLE()



cField::cField() : wxFrame(nullptr, wxID_ANY, "Field", wxPoint(300, 150), wxSize(800, 600))
{
	//m_btn1 = new wxButton(this, 1, "Button1", wxPoint(10,10), wxSize(150, 50));
	//m_txt1 = new wxTextCtrl(this, wxID_ANY,"", wxPoint(10, 70), wxSize(300, 30));
	//m_list1 = new wxListBox(this, wxID_ANY, wxPoint(10, 110), wxSize(300, 300));;
	btn = new wxButton * *[nFieldHeight];

	wxGridSizer* grid = new wxGridSizer(nFieldHeight, nFieldWidth, 0, 0);
	int id = 1000;
	for (int x = 0; x < nFieldHeight; x++)
	{
		btn[x] = new wxButton * [nFieldWidth];
		for (int y = 0; y < nFieldWidth; y++)
		{


			//btn[x][y] = new wxButton(this, (x * nFieldWidth + y), std::to_string((x * nFieldWidth + y)));
			btn[x][y] = new wxButton(this, id, std::to_string(id - 1000));
			btn[x][y]->SetSize(wxSize(10, 10));
			id++;
			btn[x][y]->SetBackgroundColour(*wxWHITE);
			//btn[x][y]->SetForegroundColour(*wxRED);

			//btn[x][y].SetParent(this);
			//btn[x][y].SetId(id++);
			//btn[x][y].SetLabelText(std::to_string(id));

			grid->Add(btn[x][y], 1, wxEXPAND | wxALL);

			btn[x][y]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &cField::OnButtonClicked, this);
		}
	}

	this->SetSizer(grid);//tell parent window which sizer to use
	//grid->Layout();
}

cField::~cField()
{
	for (int i = 0; i < nFieldHeight; i++)
	{


		delete[] btn[i];
	}
	delete[] btn;
}

void cField::OnButtonClicked(wxCommandEvent& evt)
{
	/*
	m_list1->AppendString(m_txt1->GetValue());
	m_txt1->Clear();
	evt.Skip();//Event handled
	*/

	int row = (evt.GetId() - 1000) / nFieldWidth;
	int col = (evt.GetId() - 1000) % nFieldWidth;

	if (btn[row][col]->GetBackgroundColour() == *wxWHITE) {
		btn[row][col]->SetBackgroundColour(*wxBLACK);
		btn[row][col]->SetForegroundColour(*wxWHITE);
	}
	else {
		btn[row][col]->SetBackgroundColour(*wxWHITE);
		btn[row][col]->SetForegroundColour(*wxBLACK);
	};



	//btn[row][col]->SetBackgroundColour(*wxBLACK);
	//btn[row][col]->SetForegroundColour(*wxWHITE);

	evt.Skip();
	return;
}
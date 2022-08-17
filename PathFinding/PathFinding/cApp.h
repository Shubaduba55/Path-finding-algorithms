#pragma once
#include "cMain.h"

int Node::counter = 0;

class cApp : public wxApp
{
private:
	cMain* main_window;

public:
	cApp();
	~cApp();


	virtual bool OnInit();
};


#pragma once
#include <wx/wxprec.h>
#include "wx/setup.h"
#include <wx/grid.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// Main Class that controls the app, always need, 
// implement onInit to create new frame, show it, return


//The main frame class, where GUI is actually created
//Along with all the methods for event handling
class MyFrame : public wxFrame
{
    wxGrid* grid;
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
    void OnHello(wxCommandEvent& event);
    void GenBoard(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void Solve(wxCommandEvent& event);
    void setStartingNumber(wxCommandEvent& event);
    bool Solver();
    void setSeed(int puzzleSeed[9][9]);
    void genPuzzle();
    bool CheckMoveLegal(int ROW, int COL, int NUM);
    std::pair<int, int> getEmptyCell();
    std::pair<int, int> getEmptyCell(int puzzle[9][9]);
    wxDECLARE_EVENT_TABLE();

    int startingNumber = 17;
};
enum //Identifiers for custom event methods not found in wxID standards
{
    ID_HELLO = 2,
    ID_GENERATE_BOARD = 3,
    ID_SOLVE = 4,
    ID_SET_STARTING_NUMBER = 5
};

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};
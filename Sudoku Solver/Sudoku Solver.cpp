// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#include <Windows.h>
#include "wx/setup.h"
#include <wx/grid.h>
#include <chrono>
#include <random>
#include <algorithm>
#include "Sudoku Solver.h"
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame();
    frame->Show(true);
    return true;
}

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MyFrame::OnExit)
    EVT_MENU(ID_GENERATE_BOARD, MyFrame::GenBoard)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
    EVT_MENU(ID_HELLO, MyFrame::OnHello)
    EVT_MENU(ID_SOLVE, MyFrame::Solve)
wxEND_EVENT_TABLE()

MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "Sudoku Solver")
{
    int const_size = 400 / 9;
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_HELLO, "&Hello...\tCtrl-H",
        "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(ID_GENERATE_BOARD, "Create Board", "Creates a new board");
    menuFile->AppendSeparator();
    menuFile->Append(ID_SOLVE, "Solve", "Solves the puzzle");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    
    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);
    
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");

    wxBoxSizer* framesizer = new wxBoxSizer(wxHORIZONTAL);
    
    grid = new wxGrid(this, 1, wxPoint(0,0), wxSize(400,400));
    grid->CreateGrid(9, 9, wxGrid::wxGridSelectCells);
    grid->ClearSelection();
    //grid->SetGridCursor(0, 0);
    
    grid->SetDefaultRowSize(const_size, false);
    grid->SetDefaultColSize(const_size, false);
    grid->HideRowLabels();
    grid->HideColLabels();
    grid->EnableEditing(false);
    grid->SetDefaultCellBackgroundColour(*wxWHITE);
    grid->SetGridLineColour(*wxBLACK);

    framesizer->Add(grid);

    SetSizer(framesizer);
    framesizer->SetSizeHints(this);

}
void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}
void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
        "About Hello World", wxOK | wxICON_INFORMATION);
}
void MyFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}
void MyFrame::setSeed(int puzzleSeed[9][9]) {
    int s1[9][9] = { 5,6,7,8,1,2,9,3,4
                    ,1,2,9,4,5,3,6,7,8
                    ,8,3,4,6,9,7,2,1,5
                    ,6,9,3,2,7,5,4,8,1
                    ,4,1,2,3,6,8,7,5,9
                    ,7,8,5,9,4,1,3,6,2
                    ,3,4,6,1,8,9,5,2,7
                    ,2,5,8,7,3,4,1,9,6
                    ,9,7,1,5,2,6,8,4,3 };
    std::copy(&s1[0][0], &s1[0][0] + 81, &puzzleSeed[0][0]);
}

std::pair<int, int> MyFrame::getEmptyCell() {
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            if (grid->GetCellValue(r,c) == " ") {
                return std::make_pair(r, c);
            }
        }
    }

    return std::make_pair(9, 9);
}

std::pair<int, int> MyFrame::getEmptyCell(int puzzle[9][9]) {
    for (int r = 0; r < 9; r++) {
        for (int c = 0; c < 9; c++) {
            if (puzzle[r][c] == 0) {
                return std::make_pair(r, c);
            }
        }
    }
    return std::make_pair(9, 9);
}

void MyFrame::genPuzzle()
{
    
    int puzzleSeed[9][9] = { 0 };
    setSeed(puzzleSeed);

    char letterGrid[9][9] = {' '};
    //int newGrid[N][N] = {0};

    //step two, transform all numbers to letters a-i
    //Create array[8], randomly fill with a-i
    //When converting, say we grab a 4 from the puzzle,
    //place whatever letter is in cell 4 of our key
    //array gets recreated each call to generate grid.
    char key[9] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i' };
    int x, y, t;
    
    for (int k = 0; k < 64; k++) {
        x = rand() % 9;
        y = rand() % 9;

        if (puzzleSeed[x][y] == 0) {
            k = k - 1;
        }

        else {
            t = puzzleSeed[x][y];
            puzzleSeed[x][y] = 0;

            //if (!Solver(puzzleSeed)) {
                //puzzleSeed[x][y] = t;
                //k = k -1;
            //}
        }
    }
    

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (puzzleSeed[i][j] == 0) {
                letterGrid[i][j] = ' ';
            }
            else {
                letterGrid[i][j] = key[(puzzleSeed[i][j]) - 1];
            }
        }
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(key, key + 8, std::default_random_engine(seed));

    //Transforms letterGrid back into numbers
    char temp;
    for (int i = 0; i < 9; i++) {

        for (int j = 0; j < 9; j++) {

            //gets the letter in letterGrid
            temp = letterGrid[i][j];

            //check to see if it is empty i.e it was removed
            if (letterGrid[i][j] == ' ') {
                grid->SetCellValue(i, j, " ");
                grid->SetReadOnly(i, j, false);
            }

            //else get the positiion of temp in key,
            //position then becomes number in newGrid
            else {
                int pos = (std::find(key, key + 8, temp) - key);
                if (pos == 0) {
                    grid->SetCellValue(i, j, "9");
                }
                else {
                    grid->SetCellValue(i, j, std::to_string(pos));
                }
                grid->SetReadOnly(i, j, true);
            }
        }
    }
    return;
}

bool MyFrame::CheckMoveLegal(int ROW, int COL, int NUM) {
    //Checking that it is not in the same row
    for (int i = 0; i < 9; i++) {
        if (atoi(grid->GetCellValue(ROW, i)) == NUM) {
            return false;
        }
    }

    //Checking that it is not in the same col
    for (int j = 0; j < 9; j++) {
        if (atoi(grid->GetCellValue(j, COL)) == NUM) {
            return false;
        }
    }

    int srow, scol;
    srow = ROW - ROW % 3;
    scol = COL - COL % 3;
    //Checks to see if it is in the same 3 x 3 box
    for (int r = 0; r < 3; r++) {
        for (int c = 0; c < 3; c++) {
            if (atoi(grid->GetCellValue(srow + r, scol + c)) == NUM) {
                return false;
            }
        }
    }

    return true;
}


void MyFrame::Solve(wxCommandEvent& event) {
    Solver();
}

void MyFrame::GenBoard(wxCommandEvent& event) {
    genPuzzle();
}

bool MyFrame::Solver() {
    if (getEmptyCell() == std::make_pair(9, 9)) {
        return true;
    }

    std::pair<int, int> rowcol = getEmptyCell();
    int r = rowcol.first;
    int c = rowcol.second;
    grid->SetCellBackgroundColour(r, c, *wxCYAN);
    for (int num = 1; num <= 9; num++) {
        if (CheckMoveLegal(r, c, num)) {
            grid->SetCellValue(r, c, std::to_string(num));
            grid->SetCellBackgroundColour(r, c, *wxWHITE);
            wxWindow::Refresh(); wxWindow::Update();

            if (Solver()) {
                return true;
            }

            grid->SetCellValue(r, c, " ");
            grid->SetCellBackgroundColour(r, c, *wxWHITE);
            wxWindow::Refresh(); wxWindow::Update();
        }
    }
    return false;
}


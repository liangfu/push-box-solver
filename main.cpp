/**
 * @file   main.cpp
 * @author Liangfu Chen <chenclf@gmail.com>
 * @date   Wed Mar 23 02:40:17 2011
 * 
 * @brief  main entry of push box project
 * 
 * 
 */

#include "utilities.hpp"

#include "pmsolver.hpp"
#include "pmwidget.hpp"

#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Double_Window.H>

// This callback is invoked whenever the user clicks an item in the menu bar
static void cbMainMenu(Fl_Widget *w, void *);

/** 
 * main entry of push box project
 * 
 * @param argc : the number of arguments
 * @param argv : argument list
 * 
 * @return : exit code
 */
int main(int argc, char* argv[])
{
	int ** map;
	int map_width = 0;
	int map_height = 0;

	FILE * fp;
	if (argc == 1)
		fp = fopen("map-002.pgm", "r");
	else if (argc == 2)
		fp = fopen(argv[1], "r");
	else if (argc > 2)
		fprintf(stderr, "ERROR: too many arguments.\n");
	
	char filetype[16];
	u_char largest;

	assert(fp);
	fscanf(fp, "%s\n", filetype);
	printf("input date type: %s\n", filetype);
	fscanf(fp, "%d %d\n", &map_width, &map_height);
	fscanf(fp, "%c\n", &largest); //assert(largest==8);

	new_array<int>(map, map_height, map_width);
	
	for (int j = 0; j < map_height; j++) {
		for (int i = 0; i < map_width; i++) {
			fscanf(fp, "%d\t", &map[map_height-1-j][i]);
		}
		fscanf(fp, "\n");
	}

	if (verbose) {
		if (map_width<12 && map_height<12) {
			for (int j = map_height-1; j > -1; j--) {
				for (int i = 0; i < map_width; i++) {
					fprintf(stderr, "%d\t", map[j][i]);
				}
				fprintf(stderr, "\n");
			}
			fprintf(stderr, "\n");
		}
	}


	//------------------------------------------------------------
	PMSolver* solver = new PMSolver(map, map_width, map_height);
	assert(solver);

	//------------------------------------------------------------

	//------------------------------------------------------------
	// 
	//------------------------------------------------------------
	Fl_Double_Window * win = new Fl_Double_Window(300, 420, "Push-box Puzzle");
	
    Fl_Menu_Bar *menu = new Fl_Menu_Bar(0,0,300,25);		// Create menubar, items..
    menu->add("&File/&Open",  "^o", cbMainMenu);
    menu->add("&File/&Save",  "^s", cbMainMenu, 0, FL_MENU_DIVIDER);
    menu->add("&File/&Quit",  "^q", cbMainMenu);
    menu->add("&Help/Google",    0, cbMainMenu);

	PMWidget *tim = new PMWidget(0, 25, win->w(), win->h());
	tim->setMap(map, map_width, map_height);
	solver->getMoveList(tim->movelist);

    win->end();
    win->show();

	//close file and release memory
	// fclose(fp);
	// free_array<int>(map, map_width, map_height);

	return(Fl::run());
}

static void cbMainMenu(Fl_Widget *w, void *)
{
    Fl_Menu_Bar *bar = (Fl_Menu_Bar*)w;				// Get the menubar widget
    const Fl_Menu_Item *item = bar->mvalue();			// Get the menu item that was picked

    char ipath[256];
    bar->item_pathname(ipath, sizeof(ipath));	// Get full pathname of picked item

    if ( strcmp(item->label(), "Google") == 0 ) {
        fl_open_uri("http://google.com/");
    }
    if ( strcmp(item->label(), "&Quit") == 0 ) {
        exit(0);
    }
}

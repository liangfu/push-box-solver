#ifndef __PUSH_BOX_WIDGET__
#define __PUSH_BOX_WIDGET__

#include "utilities.hpp"

#include <FL/Fl.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/filename.H>		// fl_open_uri()

#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>

#include "pmsolver.hpp"

#define BG_COLOR   45
#define TICK_COLOR 50
#define CIRC_COLOR 0

class PMWidget : public Fl_Box
{
    void draw();	
    static void cbTimer(void *userdata);
	
public:
    // CONSTRUCTOR
    PMWidget(int X,int Y,int W,int H,const char*L=0) : Fl_Box(X,Y,W,H,L) {
        box(FL_FLAT_BOX);
        color(BG_COLOR);
        Fl::add_timeout(0.25, cbTimer, (void*)this);
    }

	u_int ** map;
	int map_width;
	int map_height;
	void setMap(int ** _map, int _w, int _h);

	list<Position> movelist;

	u_int getGrid(int _x, int _y);

	void draw_wall(int _x, int _y);

	void draw_space(int _x, int _y);
	
	void draw_dest(int _x, int _y);
	
	void draw_worker(int _x, int _y);
};

#endif //__PUSH_BOX_WIDGET__

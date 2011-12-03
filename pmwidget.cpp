/**
 * @file   pmwidget.cpp
 * @author Liangfu Chen <chenclf@gmail.com>
 * @date   Tue Apr  5 13:36:53 2011
 * 
 * @brief  
 * 
 * 
 */

#include "pmwidget.hpp"



void PMWidget::draw() {
	// COMPUTE NEW COORDS OF LINE
	//static int start = time(NULL);
	//int tick = time(NULL) - start;
	
	// TELL BASE WIDGET TO DRAW ITS BACKGROUND
	Fl_Box::draw();
	
	// DRAW 'SECOND HAND' OVER WIDGET'S BACKGROUND
	for (int j = 0; j < 8; j++){
		for (int i = 0; i < 6; i++){
			fl_color(TICK_COLOR);
			fl_rectf(0+i*50, 25+j*50, 50, 50);
			fl_color(CIRC_COLOR);
			fl_rect(0+i*50, 25+j*50, 50, 50);
		}
	}
	
	fl_color(0);
	// fl_rectf(10, 35, 30, 30);
	// fl_pie(10, 35, 30, 30, 0.0, 360.0);
	
	// DRAW TIMER TEXT STRING
	// fl_font(FL_HELVETICA, 30);
	//fl_draw("★", 10, 60);

	for (int j = map_height-1; j > -1; j--){
		for(int i = 0; i < map_width; i++){
			if (getGrid(i, j) & WALL){
				// assert(getGrid(i, j)==WALL);
				draw_wall(i, j);
			}else if (getGrid(i, j) & SPACE){
				draw_space(i, j);
			}else if (getGrid(i, j) & DEST){
				draw_dest(i, j);
			}else if (getGrid(i, j) & WORKER){
				draw_worker(i, j);
			}
		}
	}
}

u_int PMWidget::getGrid(int _x, int _y)
{
	return map[_y][_x];
}

void PMWidget::draw_wall(int _x, int _y)
{
	// fl_color(0);
	// fl_rectf(10+50*_x, 35+50*_y, 30, 30);
}

void PMWidget::draw_space(int _x, int _y)
{
	
}

void PMWidget::draw_dest(int _x, int _y)
{
	fl_color(0);
	fl_font(FL_HELVETICA, 30);
	fl_draw("★", 10+50*_x, 60+50*_y);
}

void PMWidget::draw_worker(int _x, int _y)
{
	fl_color(0);
	fl_pie(10+50*_x, 35+50*_y, 30, 30, 0.0, 360.0);
}


void PMWidget::cbTimer(void *userdata) {
	PMWidget *o = (PMWidget*)userdata;
	o->redraw();
	Fl::repeat_timeout(0.25, cbTimer, userdata);
}

void PMWidget::setMap(int ** _map, int _w, int _h)
{
	map = (u_int**)_map;
	map_width = _w;
	map_height = _h;
}

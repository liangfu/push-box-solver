/**
 * @file   pmsolver.hpp
 * @author Liangfu Chen <chenclf@gmail.com>
 * @date   Wed Mar 23 14:46:45 2011
 * 
 * @brief  push-box puzzle solver
 * 
 * 
 */

#ifndef __PM_SOLVER__
#define __PM_SOLVER__

#include "utilities.hpp"

#include <list>

using std::list;

typedef int2 Position;

enum Direction {UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3};

// static const int2 nbr[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
static const int2 nbr[4] = {
	*(new Position(0, 1)),
	*(new Position(1, 0)),
	*(new Position(0, -1)),
	*(new Position(-1, 0))
};

enum Grid {
	WALL = 0x00, SPACE = 0x01, DEST = 0x02, BOX = 0x04, WORKER = 0x08
};

struct Action
{
	// Position pre_pos;
	Position cur_pos;

	Direction dir;
	bool is_push;
};

struct ActionBranch
{
	Position pos;
	list<Position> box;
	ActionBranch * prev;
	
	list<ActionBranch> action;
	ActionBranch * pointer() {return this;}
};

void set_box_position(list<Position>& _box,
					  const int _nbr_x, const int _nbr_y,
					  const int _x, const int _y);

class PMSolver
{
	u_int ** map;
	int map_width;
	int map_height;

	inline u_int getGrid(int _x, int _y);
	inline Grid getGrid(const ActionBranch * _a, int _x, int _y);
	bool finished(ActionBranch * _a, list<Position>& _dest);
	void print_map_info(const Position& worker,
						const list<Position>& box,
						const list<Position>& dest);
	void get_map_info(Position& _worker,
					  list<Position>& _box,
					  list<Position>& _dest);
	void init_worker_nbrs(ActionBranch * _a);

	Grid ** map_layer;
	void init_layer();
	void print_layer();
	void setMapLayer(int _x, int _y, Grid _g);
	Grid getMapLayer(int _x, int _y);

	list<Position> movelist;
	
	void levelOrderSearch(list<Position>& dest, list<Position>& box, Position& worker);
	
public:
	PMSolver(int** _map, int _w, int _h);
	~PMSolver(){}

	void getMoveList(list<Position>& _movelist);
};

#endif //__PM_SOLVER__

/**
 * @file   pmsolver.cpp
 * @author Liangfu Chen <chenclf@gmail.com>
 * @date   Wed Mar 23 20:21:23 2011
 * 
 * @brief  
 * 
 * 
 */

#include "pmsolver.hpp"

#include <list>
#include <queue>
#include <algorithm>

#include "stopwatch.h"

static const int PRINT_LIMIT = 0xffffff;

static int counter = 0;

static stopwatch timer_01;

void set_box_position(list<Position>& _box,
					  const int _nbr_x, const int _nbr_y,
					  const int _x, const int _y)
{
	list<Position>::iterator it;
	for (it = _box.begin(); it != _box.end(); it++){
		// if ((it->x==_nbr_x)&&(it->x==_nbr_y)){
		if (*it == *(new Position(_nbr_x, _nbr_y))){
			it->x = _nbr_x+_x;
			it->y = _nbr_y+_y;
		}
	}
}

PMSolver::PMSolver(int** _map, int _w, int _h)
{
	map = (u_int**)_map;
	map_width = _w;
	map_height = _h;

	//map_layer = (Grid**)malloc(sizeof(Grid)*map_width*map_height);
	new_array<Grid>(map_layer, map_width, map_height);

	list<Position> dest;
	list<Position> box;

	Position worker;

	get_map_info(worker, box, dest);
	
	print_map_info(worker, box, dest);

	init_layer();
	if (verbose) print_layer();

	timer_01.start();

	levelOrderSearch(dest, box, worker);
}

u_int PMSolver::getGrid(int _x, int _y)
{
	return map[_y][_x];
}

Grid PMSolver::getGrid(const ActionBranch * _a, int _x, int _y)
{
	Position _worker = _a->pos;
	list<Position> _box = _a->box;
	
	if (getMapLayer(_x, _y) == WALL){
		return WALL;
	}else if (getMapLayer(_x, _y) == DEST){
		list<Position>::iterator it;
		for (it = _box.begin(); it != _box.end(); it++){
			// if ((it->x==_x)&&(it->x==_y)){
			if (*it == *(new Position(_x, _y))){
				return BOX;
			}
		}
		return DEST;
	}else if (getMapLayer(_x, _y) == SPACE){
		list<Position>::iterator it;
		for (it = _box.begin(); it != _box.end(); it++){
			if (*it == *(new Position(_x, _y))){
			// if ((it->x==_x)&&(it->x==_y)){
				return BOX;
			}
		}
		return SPACE;
	}else{
		assert(false);
	}
}

/** 
 * to check out whether all boxes are pushed on place.
 * 
 * @return : return true if all boxes in position.
 *			 return false if not all boxes in position.
 */
bool PMSolver::finished(ActionBranch * _a, list<Position>& _dest)
{
	bool ok = true;
	// if (equal<list<Position>::iterator>(_a->box.begin(), _a->box.end(), _dest.begin())) exit(0);
	list<Position>::iterator bit;
	list<Position>::iterator dit;
	for (bit = _a->box.begin(); bit != _a->box.end(); bit++){
		for (dit = _dest.begin();;dit++){
			if (*dit == *bit){break;}
			if (dit == _dest.end()){return false;}
		}
	}
	fprintf(stdout, "\n");
	for (ActionBranch* d = _a; d->prev!=NULL; d=d->prev){
		fprintf(stdout, "[%d, %d]", d->pos.x, d->pos.y);
	}fprintf(stdout, "\n");
	return ok;
}

void PMSolver::print_map_info(const Position& _worker,
							  const list<Position>& _box,
							  const list<Position>& _dest)
{
	printf("worker's position: %d, %d\n", _worker.x, _worker.y);
	printf("num of boxes: %d\n", _box.size());
	printf("num of dests: %d\n", _dest.size());
	if (_box.size() != _dest.size()){
		fprintf(stderr, "ERROR: input map error\n");
		exit(-1);
	}
}

void PMSolver::get_map_info(Position& _worker,
							list<Position>& _box,
							list<Position>& _dest)
{
	for (int i = 0; i<map_height*map_width; i++){
		// worker maybe standing on a destination
		int _x = i%map_width;
		int _y = i/map_width;
		Position _pos(_x, _y);
		
		switch (getGrid(_x, _y)){
		case WORKER:
			_worker.x = i%map_width;
			_worker.y = i/map_width;
			break;
		case BOX:
			_box.push_back(_pos);
			break;
		case DEST:
			_dest.push_back(_pos);
			break;
		default:
			break;
		}
	}
}

void PMSolver::init_worker_nbrs(ActionBranch * _a)
{
	// visit four neighbors
	for (int i = 0; i < 4; i++){
		const int nbr_x = _a->pos.x + nbr[i].x;
		const int nbr_y = _a->pos.y + nbr[i].y;
		
		ActionBranch * c = new ActionBranch;
		c->prev = _a;
		c->box.assign(_a->box.begin(), _a->box.end());
		
		switch (getGrid(nbr_x, nbr_y)){
		case WALL:
			break;
		case SPACE:
			c->pos = *(new Position(nbr_x, nbr_y));
			_a->action.push_back(*c);
			break;
		case DEST:
			c->pos = *(new Position(nbr_x, nbr_y));
			_a->action.push_back(*c);
			break;
		case BOX:
			{
				if ((getGrid(_a, nbr_x+nbr[i].x, nbr_y+nbr[i].y) == SPACE)||
					(getGrid(_a, nbr_x+nbr[i].x, nbr_y+nbr[i].y) == DEST)) {
					c->pos = *(new Position(nbr_x, nbr_y));
					set_box_position(c->box, nbr_x, nbr_y, nbr[i].x, nbr[i].y);
					_a->action.push_back(*c);
				}
			}
			break;
		default:
			assert(false);
			break;
		}
	} // for each neighbor
}

void PMSolver::init_layer()
{
	for (int i = 0; i < map_width*map_height; i++){
		int _x = i%map_width;
		int _y = i/map_width;
		
		if (getGrid(_x, _y) & WALL){
			assert(getGrid(_x, _y) == (u_int)WALL);
			setMapLayer(_x, _y, WALL);
		}else if (getGrid(_x, _y) & SPACE){
			assert(getGrid(_x, _y) == (u_int)SPACE);
			setMapLayer(_x, _y, SPACE);
		}else if (getGrid(_x, _y) & DEST){
			setMapLayer(_x, _y, DEST);
		}else
			// set other grid types as empty space
			// EVEN IF it maybe on DEST position
			if (getGrid(_x, _y) & BOX){
			setMapLayer(_x, _y, SPACE);
		}else if (getGrid(_x, _y) & WORKER){
			setMapLayer(_x, _y, SPACE);
		}
	}
}

void PMSolver::setMapLayer(int _x, int _y, Grid _g)
{
	map_layer[_x][_y] = _g;
}

Grid PMSolver::getMapLayer(int _x, int _y)
{
	return map_layer[_x][_y];
}

void PMSolver::print_layer()
{
	if (map_width<25 && map_height<25) {
		for (int j = map_height-1; j > -1; j--) {
			for (int i = 0; i < map_width; i++) {
				fprintf(stderr, "%d\t", getMapLayer(i, j));
			}
			fprintf(stderr, "\n");
		}
		fprintf(stderr, "\n");
	}
}

void PMSolver::levelOrderSearch(list<Position>& dest, list<Position>& box, Position& worker)
{
	//------------------------------------------------------------
	// initialize tree traversal variables
	//------------------------------------------------------------
	ActionBranch * a = new ActionBranch;
	ActionBranch * b = new ActionBranch;
	a->prev = NULL;
	a->pos = worker;
	a->box.assign(box.begin(), box.end());
	
	init_worker_nbrs(a);

	for (;;){
		printf("-");fflush(stdout);
		list<ActionBranch> tmp;
		list<ActionBranch>::iterator tmp_iterator = tmp.begin();
		
		//------------------------------------------------------------
		// check whether finished pushing all boxes
		//------------------------------------------------------------
		if (finished(a, dest)) {break;}

		//---------------------------------------------------------------
		// visit all subnode in branch
		//---------------------------------------------------------------
		list<ActionBranch>::iterator it;
		for (it = a->action.begin(); it != a->action.end(); it++) {
			b = it->pointer();
			
			// visit four neighbors
			for (int i = 0; i < 4; i++){
				const int nbr_x = b->pos.x + nbr[i].x;
				const int nbr_y = b->pos.y + nbr[i].y;

				ActionBranch * c = new ActionBranch;
				c->prev = b;
				c->box.assign(b->box.begin(), b->box.end());

				if (counter!=PRINT_LIMIT){
					counter++;
					/// PRINT OUT STEPS OF PROCESS RIGHT HERE
					// fprintf(stderr, "{%d, %d}, {%d, %d}", b->pos.x, b->pos.y, nbr_x, nbr_y);
				}else{exit(0);}
				
				if (getGrid(b, nbr_x, nbr_y) == WALL){
					// do nothing here
				}else if (getGrid(b, nbr_x, nbr_y) == SPACE){
					c->pos = *(new Position(nbr_x, nbr_y));
					tmp.push_back(*c);
				}else if (getGrid(b, nbr_x, nbr_y) == DEST){
					c->pos = *(new Position(nbr_x, nbr_y));
					tmp.push_back(*c);
				}else if (getGrid(b, nbr_x, nbr_y) == BOX){
					if ((getGrid(b, nbr_x+nbr[i].x, nbr_y+nbr[i].y) == SPACE)||
						(getGrid(b, nbr_x+nbr[i].x, nbr_y+nbr[i].y) == DEST)) {
						c->pos = *(new Position(nbr_x, nbr_y));
						set_box_position(c->box, nbr_x, nbr_y, nbr[i].x, nbr[i].y);
						tmp.push_back(*c);
					}
				}else{
					assert(false);
				}
#if PRINT_DEBUG_INFO
				// PRINT OUT STEPS OF PROCESS RIGHT HERE
				fprintf(stderr, "{%d, %d}",	nbr_x, nbr_y);
				for (ActionBranch* d = b; d->prev!=NULL; d=d->prev){
					fprintf(stderr, "[%d, %d]", d->pos.x, d->pos.y);
				}
				if (counter!=PRINT_LIMIT){fprintf(stderr, "\n");}else{exit(0);}
#endif //PRINT_DEBUG_INFO
				if (finished(c, dest)) {
					fprintf(stderr, "elapsed time: %.3fms\n", timer_01.elapsed()*1000.0f);
					fprintf(stdout, "cost loops: %d\n", counter);
					for (ActionBranch * d = c; d->prev!=NULL; d=d->prev){
						movelist.push_back(d->pos);
					}
					movelist.reverse();
					return;
					//exit(0);
				}
			} // for each neighbor
#if PRINT_DEBUG_INFO			
			if (counter!=PRINT_LIMIT){fprintf(stderr, "\n");}else{exit(0);}
#endif //PRINT_DEBUG_INFO
		} // for each subnode in branch
#if PRINT_DEBUG_INFO
		if (counter!=PRINT_LIMIT){
			fprintf(stderr,
					"------------------------------"
					"------------------------------\n");
		}else{exit(0);}
#endif //PRINT_DEBUG_INFO
		//------------------------------------------------------------
		// set subnode as current node to visit
		//------------------------------------------------------------
		a = b;

		list<ActionBranch>::iterator tmp_it;
		for (tmp_it = tmp.begin(); tmp_it != tmp.end(); tmp_it++){
			a->action.push_back(*tmp_it);
			// if (finished(tmp_it->pointer(), dest)) {exit(0);}
		}
	}
}

void PMSolver::getMoveList(list<Position>& _movelist)
{
	_movelist.assign(movelist.begin(), movelist.end());
}

/**
 * @file   utilities.h
 * @author Liangfu Chen <chenclf@gmail.com>
 * @date   Wed Jul 14 22:36:23 2010
 * 
 * @brief  utility functions for general usage
 */

#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <assert.h>

#ifndef __CUDACC__
#	include <cstdio>
#	include <cstdlib>
#	include <iostream>
#	include <sstream>
#	include <cmath>
#	include <algorithm>
#	include <sys/stat.h>
#endif //__CUDACC__

#include "config.hpp"

using namespace std;

typedef unsigned int	u_int;
typedef unsigned char	u_char;

//-----------------------------------------------------------------------
// information out print function
//-----------------------------------------------------------------------

#if defined(_WIN32)
#define infoLog(...)													\
	fprintf(stderr, "%s: %d: info: %s: %s\n",							\
			__FILE__, __LINE__, __FUNCDNAME__, __VA_ARGS__);			\
	fflush(stderr)

#define infoLogf(fmt, arg)												\
	fprintf(stderr, "%s: %d: info: %s: ",								\
			__FILE__, __LINE__, __FUNCDNAME__);							\
	fprintf(stderr, fmt"\n", ##arg);									\
	fflush(stderr)
#else
#define infoLog(...)													\
	fprintf(stderr, "%s: %d: info: %s: %s\n",							\
			__FILE__, __LINE__, __PRETTY_FUNCTION__, __VA_ARGS__);		\
	fflush(stderr)

#define infoLogf(fmt, arg...)											\
	fprintf(stderr, "%s: %d: info: %s: ",								\
			__FILE__, __LINE__, __PRETTY_FUNCTION__);					\
	fprintf(stderr, fmt"\n", ##arg);									\
	fflush(stderr)
#endif

//-----------------------------------------------------------------------
// fixed length float point vector
//-----------------------------------------------------------------------

#ifndef __CUDACC__

struct float2 {
	float2(float _x, float _y)	{ x = _x; y = _y; }
	float2()					{ x =  0; y =  0; }

	float x,y;
};

struct float3 {
	float3(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
	float3()							 { x =  0; y =  0; z =  0; }

	float x,y,z;
};

struct float4 {
	float4(float _x, float _y, float _z, float _w) {
		x = _x; y = _y; z = _z; w = _w; }
	float4() {
		x =  0; y =  0; z =  0; w =  0; }

	float x,y,z,w;
};

struct int2 {
	int x, y;
	int2(int _x, int _y){x = _x; y = _y;}
	int2()				{x =  0; y =  0;}
	bool operator == (int2& next){
		return ((this->x==next.x) && (this->y==next.y));}
	bool operator != (int2& next){
		return ((this->x!=next.x) || (this->y!=next.y));}
	void operator =  (int2& next){
		this->x =next.x; this->y =next.y;}
};

// inline int2 make_int2(int _x, int _y){
// 	int2 tmp;
// 	tmp.x = _x; tmp.y = y
// 	return n}

typedef struct _int3 {int x, y, z;} int3;

//-----------------------------------------------------------------------
// utility functions
//-----------------------------------------------------------------------

/** 
 * calculate the distance from (0, 0) to (_x, _y) in two dimension
 * 
 * @param _x (in) X coordinate value
 * @param _y (in) Y coordinate value
 * 
 * @return the distance to point (0, 0) in two dimension
 */
template <typename T>
inline T calc_distance(T _x, T _y)
{
	return sqrt( pow(_x, 2) + pow(_y, 2) );
}

/** 
 * calculate the distance from (x0, y0) to (x1, y1) in two dimension
 * 
 * @param x0 (in) X coordinate value of the first vertex
 * @param x1 (in) Y coordinate value of the first vertex
 * @param y0 (in) X coordinate value of the second vertex
 * @param y1 (in) Y coordinate value of the second vertex
 * 
 * @return the distance to point (0, 0) in two dimension
 */
template <typename T>
inline T calc_distance(T x0, T x1, T y0, T y1)
{
	return sqrt( pow(x0-y0, 2) + pow(x1-y1, 2) );
}

template <typename T>
inline T calc_square(T t)
{
	return t*t;
}

template <typename T>
inline T calc_abs(T t)
{
	return (t>0) ? t : -t;
}
	
inline string int_to_str(int num){
	stringstream ss;
	ss << num;
	return ss.str();
}

#endif //__CUDACC__

inline void float2char(float* float_vol , unsigned char* char_vol , int ncells)
{
	float max_f , min_f;
	max_f = min_f = float_vol[0];
	for (int i = 0; i < ncells; i++) {
		if (float_vol[i]<min_f) min_f = float_vol[i];
		if (float_vol[i]>max_f) max_f = float_vol[i];
	}

	for (int i = 0; i < ncells; i++) {
		char_vol[i] =  (unsigned char)((float_vol[i]-min_f)/(max_f-min_f) * 255. );
	}
}

inline int idx_to_vtx(int i , int j , int k, int dim_x, int dim_y, int dim_z)
{
	if ( ((i < 0) || ( i >= dim_x)) ||
	     ((j < 0) || ( j >= dim_y)) ||
	     ((k < 0) || ( k >= dim_z)) ) return -1;

	return i + (j * dim_x) + (k * dim_x * dim_y);
}

template<typename T>
inline void new_array(T** &vtx_array, int dim_x, int dim_y)
{
	vtx_array = new T*[dim_x];
	for (int i = 0; i < dim_x; i++){
		vtx_array[i] = new T[dim_y];
	}
	assert(vtx_array);
}

template<typename T>
inline void free_array(T** &vtx_array, int dim_x, int )
{
	for (int i = 0; i < dim_x; i++){
		delete[] vtx_array[i];
	}
	delete[] vtx_array;
	vtx_array = NULL;
}

template<typename T>
inline void new_array(T*** &vtx_array, int dim_x, int dim_y, int dim_z)
{
	vtx_array = new T**[dim_x];
	for (int i = 0; i < dim_x; i++){
		vtx_array[i] = new T*[dim_y];
		for (int j = 0; j < dim_y; j++){
			vtx_array[i][j] = new T[dim_z];
		}
	}
	assert(vtx_array);
}

template<typename T>
inline void free_array(T*** &vtx_array, int dim_x, int dim_y, int )
{
	for (int i = 0; i < dim_x; i++){
		for (int j = 0; j < dim_y; j++){
			delete[] vtx_array[i][j];
		}
		delete[] vtx_array[i];
	}
	delete[] vtx_array;
	vtx_array = NULL;
}
	
#endif //__UTIL_H__


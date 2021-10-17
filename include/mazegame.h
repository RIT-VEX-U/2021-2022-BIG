#pragma once

#include <vector>
#include "core.h"
// #include "line_circle_intersect.h"

namespace MazeGame
{
  
typedef struct line_s
{
  line_s(Vector::point_t pt1, Vector::point_t pt2) : point1(pt1), point2(pt2) {}
  Vector::point_t point1, point2;
} line_t;

// Robot will use individual lines from dq_pairs for penalties as well.
extern std::vector<line_t> penalty_list;
extern std::vector<std::pair<line_t, line_t>> dq_pairs; 

/**
 * Initialize the list of lines that make up the boundaries that shouldn't be crossed
 */
void init_bounds_lines();

/**
 * Determine if we are issuing a time penalty by crossing a single white line
 */
bool is_single_penalty(position_t &current_pos);

/**
 * Determine if we are disqualifying the driver for crossing double lines
 */
bool is_dq(position_t &current_pos);

}
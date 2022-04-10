#pragma once
#include "core.h"
#include "robot-config.h"
#include <vector>

namespace MazeGame
{

typedef struct line_s
{
  line_s(Vector::point_t pt1, Vector::point_t pt2) : point1(pt1), point2(pt2) {}
  Vector::point_t point1, point2;
} line_t;

extern std::vector<line_t> penalty_list, smup_list;
extern int num_penalties;
extern int num_smups;

void init_boundary_lines();

bool is_single_penalty();

bool is_super_mega_ultra_penalty();

}
#include "mazegame.h"

#define ROBOT_RADIUS 2

std::vector<MazeGame::line_t> MazeGame::penalty_list, MazeGame::smup_list;
int MazeGame::num_smups = 0;
int MazeGame::num_penalties = 0;

void MazeGame::init_boundary_lines()
{
  // DQ lines
  smup_list.push_back({{.x=23.5, .y=0}, {.x=23.5,.y=47}});
  smup_list.push_back({{.x=23.5, .y=47}, {.x= 47, .y=47}});
  smup_list.push_back({{.x=47, .y=47}, {.x=47, .y=118}});

  // Penalty Lines
  penalty_list.push_back({{.x=0, .y=68}, {.x=21, .y=68}});
  penalty_list.push_back({{.x=23, .y=92}, {.x=44, .y=92}});
  penalty_list.push_back({{.x=23, .y=114}, {.x=70, .y=114}});
  penalty_list.push_back({{.x=50, .y=22}, {.x=75, .y=22}});
  penalty_list.push_back({{.x=75, .y=23}, {.x=75, .y=40}});
}

bool MazeGame::is_single_penalty()
{
  static bool was_prev_penalty = false;

  position_t pos = odom.get_position();
  std::vector<Vector::point_t> intersect;

  bool is_penalty = false;
  for(line_t l : penalty_list)
  {
    intersect = PurePursuit::line_circle_intersections({.x=pos.x, .y=pos.y}, ROBOT_RADIUS, l.point1, l.point2);

    if(intersect.size() > 0)
    {
      is_penalty = true;
      break;
    }
  }

  if(!was_prev_penalty && is_penalty)
    num_penalties++;
  
  was_prev_penalty = is_penalty;

  return is_penalty;
}

bool MazeGame::is_super_mega_ultra_penalty()
{
  static bool was_prev_penalty = false;

  position_t pos = odom.get_position();
  std::vector<Vector::point_t> intersect;

  bool is_penalty = false;
  for(line_t l : smup_list)
  {
    intersect = PurePursuit::line_circle_intersections({.x=pos.x, .y=pos.y}, ROBOT_RADIUS, l.point1, l.point2);

    if(intersect.size() > 0)
    {
      is_penalty = true;
      break;
    }
  }

  if(!was_prev_penalty && is_penalty)
    num_smups++;
  
  was_prev_penalty = is_penalty;

  return is_penalty;
}
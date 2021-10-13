#include "mazegame.h"

#define PEN_RADIUS 7
#define DQ_RADIUS 7

std::vector<MazeGame::line_t> MazeGame::penalty_list;
std::vector<std::pair<MazeGame::line_t, MazeGame::line_t>> MazeGame::dq_pairs;

double tile_size(int num_tiles, double offset=0)
{
  return (num_tiles * 24) + offset;
}

/**
 * Initialize the list of lines that make up the boundaries that shouldn't be crossed-
 * Areas and lines correspond to the map, which is in the git repo
 */
void MazeGame::init_bounds_lines()
{
  //N.B. Arguments to line_t constructor are position_t
  // Area 1
  dq_pairs.push_back({
    line_t({.x=tile_size(1, -3), .y=0}, {.x=tile_size(1, -3), .y=tile_size(5)}), 
    line_t({.x=tile_size(1, 3), .y=0}, {.x=tile_size(1, 3), .y=tile_size(5)})
    });

  // Area 2
  dq_pairs.push_back({
    line_t({.x=tile_size(2,-3), .y=tile_size(1)}, {.x=tile_size(2,-3), .y=tile_size(6)}),
    line_t({.x=tile_size(2,3), .y=tile_size(1)}, {.x=tile_size(2,3), .y=tile_size(6)})
  });

  //Area 3
  dq_pairs.push_back({
    line_t({.x=tile_size(2,3), .y=tile_size(2,-3)}, {.x=tile_size(4, -3), .y=tile_size(2, -3)}),
    line_t({.x=tile_size(2, 3), .y=tile_size(2, 3)}, {.x=tile_size(4, 3), .y=tile_size(2, 3)})
  });

  //Area 4
  dq_pairs.push_back({
    line_t({.x=tile_size(3, -3), .y=0}, {.x=tile_size(3, -3), .y=tile_size(1, -3)}),
    line_t({.x=tile_size(3, 3), .y=0}, {.x=tile_size(3, 3), .y=tile_size(1, -3)})
  });

  //Area 5
  dq_pairs.push_back({
    line_t({.x=tile_size(4, -3), .y=tile_size(1, -3)},{.x=tile_size(4, -3), .y=tile_size(2, -3)}),
    line_t({.x=tile_size(4, 3), .y=tile_size(1, 3)},{.x=tile_size(4, 3), .y=tile_size(2, 3)})
  });

  //Area 6
  dq_pairs.push_back({
    line_t({.x=tile_size(4, -3), .y=tile_size(1, -3)},{.x=tile_size(5, 3), .y=tile_size(1, -3)}),
    line_t({.x=tile_size(4, 3), .y=tile_size(1, 3)},{.x=tile_size(5, 3), .y=tile_size(1, 3)})
  });

  //Area 7
  dq_pairs.push_back({
    line_t({.x=tile_size(3, -3), .y=tile_size(3, -3)},{.x=tile_size(5, -3), .y=tile_size(3, -3)}),
    line_t({.x=tile_size(3, -3), .y=tile_size(3, 3)},{.x=tile_size(5, -3), .y=tile_size(3, 3)})
  });

  //Area 8
  dq_pairs.push_back({
    line_t({.x=tile_size(2, 3), .y=tile_size(4, 3)},{.x=tile_size(4, 3), .y=tile_size(4, 3)}),
    line_t({.x=tile_size(2, 3), .y=tile_size(4, -3)},{.x=tile_size(4, 3), .y=tile_size(4, -3)})
  });

  //Area 9
  dq_pairs.push_back({
    line_t({.x=tile_size(3, -3), .y=tile_size(5, 3)},{.x=tile_size(5, 3), .y=tile_size(5, 3)}),
    line_t({.x=tile_size(3, -3), .y=tile_size(5, -3)},{.x=tile_size(5, -3), .y=tile_size(5, -3)})
  });

  //Area 10
  dq_pairs.push_back({
    line_t({.x=tile_size(5, -3), .y=tile_size(5, -3)},{.x=tile_size(5, -3), .y=tile_size(3, 3)}),
    line_t({.x=tile_size(5, 3), .y=tile_size(5, 3)},{.x=tile_size(5, 3), .y=tile_size(4, 3)})
  });

  //Line 1
  penalty_list.push_back(line_t(
    {.x=tile_size(5, -3), .y=tile_size(3, -3)}, 
    {.x=tile_size(5, -3), .y=tile_size(2, -3)}
    ));

  //Line 2
  penalty_list.push_back(line_t(
    {.x=tile_size(5, -3), .y=tile_size(2, -3)},
    {.x=tile_size(6, -3), .y=tile_size(2, -3)}
  ));
}

/**
 * Determine if we are issuing a time penalty by crossing a single white line
 */
bool MazeGame::is_single_penalty(position_t &current_pos)
{
  // Create a list of found intersections.
  // Since the intersections function returns a vector with one or two intersections,
  // save a list of vectors.
  int list_len = (dq_pairs.size() * 2) + penalty_list.size();
  std::vector<Vector::point_t> rvals[list_len];

  // Convert robot's position to a point
  Vector::point_t robot_pt = {.x=current_pos.x, .y=current_pos.y};
  
  // Get the individual lines from the "disqualification" pairs, and look for intersectinos separately
  int index = 0;
  for(int i = 0; i < dq_pairs.size(); i++)
  {
    rvals[index++] = TankDrive::line_circle_intersections(
     robot_pt, PEN_RADIUS, dq_pairs[i].first.point1, dq_pairs[i].first.point2);
    
    rvals[index++] = TankDrive::line_circle_intersections(
     robot_pt, PEN_RADIUS, dq_pairs[i].second.point1, dq_pairs[i].second.point2);
  }

  // Process the remaining lines
  for(int i = 0; i < penalty_list.size(); i++)
    rvals[index++] = TankDrive::line_circle_intersections(
      robot_pt, PEN_RADIUS, penalty_list[i].point1, penalty_list[i].point2);

  static bool is_holding_penalty = false;

  // Go through list and see if we returned any intersections
  for(std::vector<Vector::point_t> v : rvals)
    if (v.size() != 0)
    {
      // Make sure we don't continuously dish out penalties every loop.
      // Instead make it a oneshot, reset when not crossing line.
      if(is_holding_penalty)
        return false;
      else
      {
        is_holding_penalty = true;
        return true;
      }
    }

  is_holding_penalty = false;
  
  // No penalties detected
  return false;
}

/**
 * Determine if we are disqualifying the driver for crossing double lines
 */
bool MazeGame::is_dq(position_t &current_pos)
{
  std::vector<Vector::point_t> line1_rval, line2_rval;

  // Convert robot's position to a point
  Vector::point_t robot_pt = {.x=current_pos.x, .y=current_pos.y};

  for(int i = 0; i < dq_pairs.size(); i++)
  {
    line1_rval = TankDrive::line_circle_intersections(robot_pt, DQ_RADIUS, dq_pairs[i].first.point1, dq_pairs[i].first.point2);
    line2_rval = TankDrive::line_circle_intersections(robot_pt, DQ_RADIUS, dq_pairs[i].second.point1, dq_pairs[i].second.point2);

    // If both lines have at least one intersection, then we are crossing a double line and DQ the driver.
    if(line1_rval.size() > 0 && line2_rval.size() > 0)
      return true;
  }
  
  // No DQ's detected 
  return false;
}
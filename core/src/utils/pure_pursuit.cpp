#include "../core/include/utils/pure_pursuit.h"

/**
  * Returns points of the intersections of a line segment and a circle. The line 
  * segment is defined by two points, and the circle is defined by a center and radius.
  */
std::vector<Vector::point_t> PurePursuit::line_circle_intersections(Vector::point_t center, double r, Vector::point_t point1, Vector::point_t point2)
{
  std::vector<Vector::point_t> intersections = {};

  //Do future calculations relative to the circle's center
  point1.y -= center.y;
  point1.x -= center.x;
  point2.y -= center.y;
  point2.x -= center.x;

  double x1, x2, y1, y2;
  //Handling an infinite slope using mx+b and x^2 + y^2 = r^2
  if(point1.x - point2.x == 0)
  {
    x1 = point1.x;
    y1 = sqrt(pow(r, 2) - pow(x1, 2));
    x2 = point1.x;
    y2 = -sqrt(pow(r, 2) - pow(x2, 2));
  }
  //Non-infinite slope using mx+b and x^2 + y^2 = r^2
  else
  {
    double m = (point1.y - point2.y) / (point1.x - point2.x);
    double b = point1.y - (m * point1.x);

    x1 = ((-m * b) + sqrt(pow(r, 2) + (pow(m, 2) * pow(r, 2)) - pow(b, 2))) / (1 + pow(m,2));
    y1 = m * x1 + b;
    x2 = ((-m * b) - sqrt(pow(r, 2) + (pow(m, 2) * pow(r, 2)) - pow(b, 2))) / (1 + pow(m,2));
    y2 = m * x2 + b;
  }

  //The equations used define an infinitely long line, so we check if the detected intersection falls on the line segment.
  if(x1 >= fmin(point1.x, point2.x) && x1 <= fmax(point1.x, point2.x) && y1 >= fmin(point1.y, point2.y) && y1 <= fmax(point1.y, point2.y))
  {
    intersections.push_back(Vector::point_t{.x = x1 + center.x, .y = y1 + center.y});
  }

  if(x2 >= fmin(point1.x, point2.x) && x2 <= fmax(point1.x, point2.x) && y2 >= fmin(point1.y, point2.y) && y2 <= fmax(point1.y, point2.y))
  {
    intersections.push_back(Vector::point_t{.x = x2 + center.x, .y = y2 + center.y});
  }

  return intersections;
}

/**
 * Selects a look ahead from all the intersections in the path.
 */
Vector::point_t PurePursuit::get_lookahead(std::vector<Vector::point_t> path, Vector::point_t robot_loc, double radius)
{ 
  //Default: the end of the path
  Vector::point_t target = path.back();

  if(target.dist(robot_loc) <= radius)
  {
    return target;
  }

  //Check each line segment of the path for potential targets
  for(int i = 0; i < path.size() - 1; i++)
  {
    Vector::point_t start = path[i];
    Vector::point_t end = path[i+1];

    std::vector<Vector::point_t> intersections = line_circle_intersections(robot_loc, radius, start, end);
    //Choose the intersection that is closest to the end of the line segment
    //This prioritizes the closest intersection to the end of the path
    for(Vector::point_t intersection: intersections)
    {
      if(intersection.dist(end) < target.dist(end))
        target = intersection;
    }
  }

  return target;
}

/**
 Injects points in a path without changing the curvature with a certain spacing.
*/
std::vector<Vector::point_t> PurePursuit::inject_path(std::vector<Vector::point_t> path, double spacing)
{
  std::vector<Vector::point_t> new_path;

  //Injecting points for each line segment
  for(int i = 0; i < path.size() - 1; i++)
  {
    Vector::point_t start = path[i];
    Vector::point_t end = path[i+1];

    Vector::point_t diff = end - start;
    Vector vector = Vector(diff);
    
    int num_points = ceil(vector.get_mag() / spacing);

    //This is the vector between each point
    vector = vector.normalize() * spacing;

    for(int j = 0; j < num_points; j++)
    {
      //We take the start point and add additional vectors
      Vector::point_t path_point = (Vector(start) + vector * j).point();
      new_path.push_back(path_point);
    }
  }
  //Adds the last point
  new_path.push_back(path.back());
  return new_path;
}

/**
 * Returns a smoothed path maintaining the start and end of the path.
 *
 * Weight data is how much weight to update the data (alpha)
 * Weight smooth is how much weight to smooth the coordinates (beta)
 * Tolerance is how much change per iteration is necessary to continue iterating.
 *
 * Honestly have no idea if/how this works.
 * https://medium.com/@jaems33/understanding-robot-motion-path-smoothing-5970c8363bc4
*/

std::vector<Vector::point_t> PurePursuit::smooth_path(std::vector<Vector::point_t> path, double weight_data, double weight_smooth, double tolerance)
{
  std::vector<Vector::point_t> new_path = path;
  double change = tolerance;
  while(change >= tolerance)
  {
    change = 0;
    for(int i = 1; i < path.size() - 1; i++)
    {
        Vector::point_t x_i = path[i];
        Vector::point_t y_i = new_path[i];
        Vector::point_t y_prev = new_path[i-1];
        Vector::point_t y_next = new_path[i+1];
        
        Vector::point_t y_i_saved = y_i;
        
        y_i.x += weight_data * (x_i.x - y_i.x) + weight_smooth * (y_next.x + y_prev.x - (2 * y_i.x));
        y_i.y += weight_data * (x_i.y - y_i.y) + weight_smooth * (y_next.y + y_prev.y - (2 * y_i.y));
        new_path[i] = y_i;
        
        change += y_i.dist(y_i_saved);
    }
  }
  return new_path;
}

/**
  Drives through a path using pure pursuit.
*/
bool PurePursuit::pure_pursuit(bool *drive_to_point(double, double, double, double), std::vector<Vector::point_t> path, Vector::point_t robot_loc, double radius, double speed, double spacing, double weight_data, double weight_smooth)
{
  std::vector<Vector::point_t> injected_path = inject_path(path, spacing);
  std::vector<Vector::point_t> smoothed_path = smooth_path(injected_path, weight_data, weight_smooth, .0001);
  /*
  printf("injected path:\n");
  for(Vector::point_t point: injected_path)
  {
    printf("\tx: %f y: %f\n", point.x, point.y);
  }
  printf("smoothed path:\n");
  for(Vector::point_t point: smoothed_path)
  {
    printf("\tx: %f y: %f\n", point.x, point.y);
  }
  */
  Vector::point_t lookahead = get_lookahead(smoothed_path, robot_loc, radius);

  printf("look ahead x %f y: %f\n", lookahead.x, lookahead.y);
  return drive_to_point(lookahead.x, lookahead.y, speed, speed/2);
}
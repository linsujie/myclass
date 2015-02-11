#ifndef _TABLE2D_H
#define _TABLE2D_H
#include<vector>

class table2D {
private:
  inline double dx(int xpos, int ypos) const;
  inline double dy(int xpos, int ypos) const;
public:
  std::vector <double> x, y;
  std::vector <std::vector<double> > val;

  int search(double axival_, int x_or_y) const;
  int get_pos(double axi_, int x_or_y) const;
  int insx(double x_);
  int insy(double y_);

  int insval(int xpos, int ypos, double val_);
  int insval(double xin_, double yin_, double val_);

  int trans();
  int list();
  int clear();

  double dx2(double x_, double y_) const;
  double dx2(int xpos, int ypos) const;

  double dy2(double x_, double y_) const;
  double dy2(int xpos, int ypos) const;
};
#endif // for #ifndef _TABLE2D_H

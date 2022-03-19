#ifndef DRAWFIELD_H

#define DRAWFIELD_H

#include <QWidget>
#include <QMouseEvent>

#include <list>

#include "../include/sPoint.h"
#include "../include/sCamera.h"
#include "../include/matrix.h"

class DrawField : public QWidget
{

  //Q_OBJECT

  private:
    const size_t W = 1280/*600*/;
    const size_t H = 720/*400*/;
    const double aspect = (double)W / (double)H;

    const double n = 1;
    const double f = 10000;
    const double fov = 90;
    const double t = /* n*tg*(fov/2)  */n * 1; // top
    const double r = t*aspect;                //right

    Matrix<double> *C_;

    int **display;

    sCamera cam;

    void refresh_C_();
    void refresh_display();

  public:
    DrawField(QWidget *parent = 0);
    ~DrawField();

    void keyPressEventFU(QKeyEvent *event);
    
    void putPoint(double x, double y, double z);
    void putLine3D(double x1, double y1, double z1, double x2, double y2, double z2);
    void putLine3D(const sPoint& b, const sPoint& e);

    void putParallelepiped(const sPoint p1, const sPoint p2);
    void putSphere(const sPoint c, double r);
 
  private:
    void paintEvent(QPaintEvent *event);

    //void drawPoint(const sPoint& p, QPainter& qp, QColor *colo = 0);


  protected:
    //void keyPressEvent(QKeyEvent *event);
};

#endif // DRAWFIELD_H
#ifndef DRAWFIELD_H

#define DRAWFIELD_H

#include <QWidget>
#include <QMouseEvent>

#include <list>

#include "../include/sPoint.h"
#include "../include/sCamera.h"

class DrawField : public QWidget
{

  //Q_OBJECT

  private:
    const unsigned W = 600;
    const unsigned H = 400;
    const double aspect = W / H;

    const double n = 1;
    const double f = 10000;
    const double fov = 90;
    const double t = /* n*tg*(fov/2)  */n * 1; // top
    const double r = t*aspect;                //right

    sCamera cam;

  public:
    DrawField(QWidget *parent = 0);
    ~DrawField();

    void keyPressEventFU(QKeyEvent *event);
 
  private:
    void paintEvent(QPaintEvent *event);
    void printLine(const sPoint& p0, const sPoint& p1, QPainter& pen);

    //void drawPoint(const sPoint& p, QPainter& qp, QColor *colo = 0);
    void putPoint(double x, double y, double z);
    void putPoint2(double x, double y, double z);
    void putLine3D(double x1, double y1, double z1, double x2, double y2, double z2);
    void putLine3D(const sPoint& b, const sPoint& e);


  protected:
    //void keyPressEvent(QKeyEvent *event);
};

#endif // DRAWFIELD_H
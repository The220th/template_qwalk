#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <cmath>
#include <iostream>
#include <limits>

#include "../include/drawField.h"

#include "../include/sPoint.h"
#include "../include/sup.h"
#include "../include/matrix.h"

using namespace std;

DrawField::DrawField(QWidget *parent) : QWidget(parent), cam()
{
    resize(W, H);
    this->setStyleSheet("background-color: rgb(200,200,200); margin:0px; border:1px solid rgb(0, 0, 0); ");
}

DrawField::~DrawField()
{
}
 
void DrawField::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    /*
    sPoint A(30, -250, 0);
    sPoint B(200, 300, 0);
    sPoint C(-200, 300, 0);
    sPoint D(100, 100, 100);

    putLine3D(A, B); putLine3D(B, C); putLine3D(C, A);
    putLine3D(A, D); putLine3D(B, D); putLine3D(C, D);
    */

    
    sPoint A1(-5, 5, 0);
    sPoint A2(5, 5, 0);
    sPoint A3(5, 10, 0);
    sPoint A4(-5, 10, 0);
    sPoint B1(-5, 5, 5);
    sPoint B2(5, 5, 5);
    sPoint B3(5, 10, 5);
    sPoint B4(-5, 10, 5);
    putLine3D(A1, A2); putLine3D(A2, A3); putLine3D(A3, A4); putLine3D(A4, A1);
    putLine3D(B1, B2); putLine3D(B2, B3); putLine3D(B3, B4); putLine3D(B4, B1);
    putLine3D(A1, B1); putLine3D(A2, B2); putLine3D(A3, B3); putLine3D(A4, B4);

    /*
    sPoint A1(0, 0, 0);
    sPoint A2(50, 0, 0);
    sPoint A3(0, 50, 0);
    sPoint A4(50, 50, 0);
    sPoint B1(0, 0, 50);
    sPoint B2(50, 0, 50);
    sPoint B3(0, 50, 50);
    sPoint B4(50, 50, 50);
    putLine3D(A1, A2); putLine3D(A2, A3); putLine3D(A3, A4); putLine3D(A4, A1);
    putLine3D(B1, B2); putLine3D(B2, B3); putLine3D(B3, B4); putLine3D(B4, B1);
    putLine3D(A1, B1); putLine3D(A2, B2); putLine3D(A3, B3); putLine3D(A4, B4);*/

    //QPainter qp(this);
}

//void DrawField::keyPressEvent(QKeyEvent *event)
void DrawField::keyPressEventFU(QKeyEvent *event)
{
    int key = event->key();

    double d = 2;
    double dd = 5;
    
    // https://doc.qt.io/qt-5/qt.html#Key-enum
    if(key == Qt::Key_Up)
    {
        cam.moveForward(d);
    }
    else if(key == Qt::Key_Down)
    {
        cam.moveBack(d);
    }
    else if(key == Qt::Key_U)
    {
        cam.move(d, 0.0, 0.0);
    }
    else if(key == Qt::Key_J)
    {
        cam.move(-d, 0.0, 0.0);
    }
    else if(key == Qt::Key_Left)
    {
        cam.moveLeft(d);
    }
    else if(key == Qt::Key_Right)
    {
        cam.moveRight(d);
    }
    else if(key == Qt::Key_H)
    {
        cam.move(0.0, d, 0.0);
    }
    else if(key == Qt::Key_K)
    {
        cam.move(0.0, -d, 0.0);
    }
    else if(key == Qt::Key_Space)
    {
        cam.move(0.0, 0.0, d);
    }
    else if(key == Qt::Key_C)
    {
        cam.move(0.0, 0.0, -d);
    }
    else if(key == Qt::Key_Y)
    {
        cam.moveUp(d);
    }
    else if(key == Qt::Key_I)
    {
        cam.moveDown(d);
    }
    else if(key == Qt::Key_W)
    {
        cam.rotateOX(-dd);
    }
    else if(key == Qt::Key_S)
    {
        cam.rotateOX(dd);
    }
    else if(key == Qt::Key_A)
    {
        cam.rotateOZ(dd);
    }
    else if(key == Qt::Key_D)
    {
        cam.rotateOZ(-dd);
    }
    else if(key == Qt::Key_Q)
    {
        cam.rotateOY(-dd);
    }
    else if(key == Qt::Key_E)
    {
        cam.rotateOY(dd);
    }

    //cout << cam.print() << endl;

    update();
}

void DrawField::putPoint2(double x, double y, double z)
{
    double xp,/* yp,*/ zp;
    xp = ((n*x)/y);
    zp = ((n*z)/y);

    xp = ((xp+r)*(W-1))/(r+r);
    zp = ((zp+t)*(H-1))/(t+t);

    QPainter qp(this);
    qp.drawPoint(xp, H-zp);
}

void DrawField::putPoint(double x, double y, double z)
{

    // https://i.imgur.com/DCahnmS.png
    sPoint cam_o = cam.o();
    double obj_x = x - cam_o.x();
    double obj_y = y - cam_o.y(); 
    double obj_z = z - cam_o.z();

    sPoint vx(cam.vr());
    sPoint vy(cam.vf());
    sPoint vz(cam.vu());

    Matrix<double> C(3, 3);
    C.set(vx.x(), 0, 0); C.set(vx.y(), 1, 0); C.set(vx.z(), 2, 0);
    C.set(vy.x(), 0, 1); C.set(vy.y(), 1, 1); C.set(vy.z(), 2, 1);
    C.set(vz.x(), 0, 2); C.set(vz.y(), 1, 2); C.set(vz.z(), 2, 2);
    //std::cout << C.toString() << std::endl;
    Matrix<double> C_ = C.inverse();

    //std::cout << C_.multiply(C).toString() << std::endl;

    Matrix<double> old_v(3, 1);
    old_v.set(obj_x, 0, 0); old_v.set(obj_y, 1, 0); old_v.set(obj_z, 2, 0);

    Matrix<double> new_v = C_.multiply(old_v);

    double x_ = new_v.get(0, 0);
    double y_ = new_v.get(1, 0);
    double z_ = new_v.get(2, 0);
    /*
    double x_ = vx.x()*obj_x + vx.y()*obj_y + vx.z()*obj_z;
    double y_ = vy.x()*obj_x + vy.y()*obj_y + vy.z()*obj_z;
    double z_ = vz.x()*obj_x + vz.y()*obj_y + vz.z()*obj_z;
    */

    //По факту: В координой системе камеры есть координаты точки
    // (x_, y_, z_)
    // Теперь "Проекция перспективы"
    // pizza time https://www.youtube.com/watch?v=lpvT-Fciu-4

    // ==========================================
    
    double xp,/* yp,*/ zp;
    //yp = n;
    //xp = ((n*x_)/y_);
    //zp = ((n*z_)/y_);
    xp = ((n*x_)/y_);
    zp = ((n*z_)/y_);

    if(xp < -r || zp > r)
        return;
    if(zp < -t || zp > t)
        return;
    if(y_ > f || y_ < n)
        return;
    
    double x_res, y_res;
    x_res = ((xp+r)*(W-1))/(r+r);
    y_res = ((zp+t)*(H-1))/(t+t);

    /*
    y_ = -y_;

    double xp, yp, zp;
    yp = -n;
    xp = -((n*x_)/y_);
    zp = -((n*z_)/y_);

    if(xp < r || zp > r)
        return;
    if(zp < t || zp > t)
        return;
    if(yp < -f || yp > -n)
        return;
    
    double x_res, y_res;
    x_res = ((xp+r)*(W-1)/(r+r));
    y_res = ((zp+t)*(H-1)/(t+t));
    */



    // ==========================================


    QPainter qp(this);
    qp.drawPoint(x_res, H-y_res);

    return;
}

void DrawField::putLine3D(const sPoint& b, const sPoint& e)
{
    putLine3D(b.x(), b.y(), b.z(), e.x(), e.y(), e.z());
}

void DrawField::putLine3D(double x1, double y1, double z1, double x2, double y2, double z2)
{
    double x, y, z;
    for(double _t = 0.0; _t <= 1.0; _t+=0.001)
    {
        x = (1-_t)*x1 + _t*x2;
        y = (1-_t)*y1 + _t*y2;
        z = (1-_t)*z1 + _t*z2;
        putPoint(x,y,z);
    }
}

/*
void DrawField::putLine3D(double x1, double y1, double z1, double x2, double y2, double z2)
{
    // https://rsdn.org/forum/alg/1706012.all

    int dx = abs(x2-x1); // длины путей по коорд. осям
    int dy = abs(y2-y1);
    int dz = abs(z2-z1); // NB: dx>=dy>=dz !

    int sx = (x2>=x1)?1:-1; // единичные направления движения
    int sy = (y2>=y1)?1:-1;
    int sz = (z2>=z1)?1:-1;

    int d1, d2, d;
    int dd1, dd2, dd;
    int x, y, z, i;

    putPoint(x1, y1, z1); // вывод первой точки

    d  = (dy<<1)-dx;  // инициализация пары 'x'-'y'
    d1 = dy<<1;
    d2 = (dy-dx)<<1;

    dd  = (dz<<1)-dy; // инициализация пары 'y'-'z'
    dd1 = dz<<1;
    dd2 = (dz-dy)<<1;

    x = x1 + sx;
    y = y1;
    z = z1;
    for(i = 1; i <= dx; ++i, x += sx)
    { // первый "брезенхем", в счётном цикле
        if(d > 0)
        {
            d += d2;
            y += sy;

            // был выполнен шаг по средней оси - 'y'. 
            // второй "брезенхнм"
            if(dd > 0)
            {
                dd += dd2;
                z += sz;
            }
            else
                dd+=dd1;
            // конец второго "брезенхема"

        }
        else
            d+=d1;
        // конец первого "брезенхема"

        putPoint(x,y,z); // вывод очередной точки

    }

}*/
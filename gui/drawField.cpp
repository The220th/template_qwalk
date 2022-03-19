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

#include "../include/planner.h"

using namespace std;

DrawField::DrawField(QWidget *parent) : QWidget(parent), cam()
{
    resize(W, H);
    this->setStyleSheet("background-color: rgb(200,200,200); margin:0px; border:1px solid rgb(0, 0, 0); ");
    
    C_ = NULL;
    refresh_C_();

    display = new int*[H];
    for(size_t li = 0; li < H; ++li)
        display[li] = new int[W];
    refresh_display();
}

DrawField::~DrawField()
{
    if(C_ != NULL)
        delete C_;

    for(size_t li = 0; li < H; ++li)
        delete display[li];
    delete display;
}
 
void DrawField::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    refresh_display();

    planner_plan(this);


    QPainter qp(this);
    for(size_t li = 0; li < H; ++li)
        for(size_t lj = 0; lj < W; ++lj)
            if(display[li][lj] != 0)
                qp.drawPoint(lj, li);

    /*
    QPainter qp(this);
    for(size_t li = 0; li < H; ++li)
    {
        for(size_t lj = 0; lj < W; ++lj)
            cout << display[li][lj];
        cout << endl;
    }*/

}

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

    refresh_C_();
    update();
}

void DrawField::putPoint(double x, double y, double z)
{
    sPoint cam_o = cam.o();
    double obj_x = x - cam_o.x(); 
    double obj_y = y - cam_o.y(); 
    double obj_z = z - cam_o.z(); 

    //std::cout << C_.multiply(C).toString() << std::endl;

    Matrix<double> old_v(3, 1);
    old_v.set(obj_x, 0, 0); old_v.set(obj_y, 1, 0); old_v.set(obj_z, 2, 0);

    Matrix<double> new_v = C_->multiply(old_v);

    /*
    Конечно, намного лучше найти в новом базисе начало и конец прямой.
    Потом в этом базисе и рисовать. Но хотелось именно в общем виде реализовать
    Поэтому, если есть желание, то для оптимизации лучше переделать отрисовку прямых.
    */
    double x_ = new_v.get(0, 0);
    double y_ = new_v.get(1, 0);
    double z_ = new_v.get(2, 0);

    // В координой системе камеры есть координаты точки
    // (x_, y_, z_)
    // Теперь "Проекция перспективы"
    // pizza time https://www.youtube.com/watch?v=lpvT-Fciu-4

    // ==========================================
    
    double xp,/* yp,*/ zp;

    xp = ((n*x_)/y_);
    zp = ((n*z_)/y_);

    if(xp < -r || xp > r)
        return;
    if(zp < -t || zp > t)
        return;
    if(y_ > f || y_ < n)
        return;
    
    double x_res, y_res;
    x_res = ((xp+r)*(W-1))/(r+r);
    y_res = ((zp+t)*(H-1))/(t+t);

    // ==========================================


    size_t display_x = (size_t)(x_res+0.5);
    size_t display_y = (size_t)(y_res+0.5);
    display[H-1-display_y][display_x] = 1;

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

void DrawField::refresh_C_()
{
    sPoint vx(cam.vr());
    sPoint vy(cam.vf());
    sPoint vz(cam.vu());

    Matrix<double> C(3, 3);
    C.set(vx.x(), 0, 0); C.set(vy.x(), 0, 1); C.set(vz.x(), 0, 2);
    C.set(vx.y(), 1, 0); C.set(vy.y(), 1, 1); C.set(vz.y(), 1, 2);
    C.set(vx.z(), 2, 0); C.set(vy.z(), 2, 1); C.set(vz.z(), 2, 2);
    //std::cout << C.toString() << std::endl;
    Matrix<double> C_buff = C.inverse();
    
    if(C_ != NULL)
        delete C_;
    C_ = new Matrix<double>(C_buff);
}

void DrawField::refresh_display()
{
    for(size_t li = 0; li < H; ++li)
        for(size_t lj = 0; lj < W; ++lj)
            display[li][lj] = 0;
}

/*

           +--------+ <--p2
          /        /|
         /        / |
        +--------+  |
        |        |  |
        | 4      |  +3     
        |        | /          z| /y
        |        |/            |/
 p1--> 1+--------+2            +--x 
*/
void DrawField::putParallelepiped(const sPoint p1, const sPoint p2)
{
    sPoint A1(p1);
    sPoint A2(p2.x(), p1.y(), p1.z());
    sPoint A3(p2.x(), p2.y(), p1.z());
    sPoint A4(p1.x(), p2.y(), p1.z());
    sPoint B1(p1.x(), p1.y(), p2.z());
    sPoint B2(p2.x(), p1.y(), p2.z());
    sPoint B3(p2);
    sPoint B4(p1.x(), p2.y(), p2.z());
    putLine3D(A1, A2); putLine3D(A2, A3); putLine3D(A3, A4); putLine3D(A4, A1);
    putLine3D(B1, B2); putLine3D(B2, B3); putLine3D(B3, B4); putLine3D(B4, B1);
    putLine3D(A1, B1); putLine3D(A2, B2); putLine3D(A3, B3); putLine3D(A4, B4);
}

void DrawField::putSphere(const sPoint c, double r)
{
    const double dr = 0.1;
    putPoint(c.x(), c.y(), c.z());

    // ===================================
    /*
    (hor - hor0)^2 + (wer - wer0)^2 = r^2
    wer = +sqrt(r^2 - (hor - hor0)^2) + y0
    wer = -sqrt(r^2 - (hor - hor0)^2) + y0
    */
    double hor0;
    double wer0;
    double plan;
    // ================Z===================
    hor0 = c.x();
    wer0 = c.y();
    plan = c.z();

    for (double hor = hor0-r; hor <= hor0+r; hor+=dr)
    {
        double sq = sqrt(  r*r - (hor - hor0)*(hor - hor0)  );

        double wer1 = sq + wer0;
        double wer2 = -sq + wer0;

        putPoint(hor, wer1, plan);
        putPoint(hor, wer2, plan);
    }

    // ================Y===================
    hor0 = c.x();
    wer0 = c.z();
    plan = c.y();

    for (double hor = hor0-r; hor <= hor0+r; hor+=dr)
    {
        double sq = sqrt(  r*r - (hor - hor0)*(hor - hor0)  );

        double wer1 = sq + wer0;
        double wer2 = -sq + wer0;

        putPoint(hor, plan, wer1);
        putPoint(hor, plan, wer2);
    }

    // ================X===================
    hor0 = c.z();
    wer0 = c.y();
    plan = c.x();

    for (double hor = hor0-r; hor <= hor0+r; hor+=dr)
    {
        double sq = sqrt(  r*r - (hor - hor0)*(hor - hor0)  );

        double wer1 = sq + wer0;
        double wer2 = -sq + wer0;

        putPoint(plan, wer1, hor);
        putPoint(plan, wer2, hor);
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
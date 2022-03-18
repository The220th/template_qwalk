
#include "../include/drawField.h"
#include "../include/planner.h"
#include "../include/sPoint.h"

void planner_plan(DrawField *df)
{
    //coord
    df->putLine3D(sPoint(0, 0, 0), sPoint(10, 0, 0));
    df->putLine3D(sPoint(0, 0, 0), sPoint(0, 10, 0));
    df->putLine3D(sPoint(0, 0, 0), sPoint(0, 0, 10));

    //house
    df->putParallelepiped(sPoint(0, 50, 0), sPoint(100, 100, 75));
    df->putLine3D(sPoint(50, 75, 75), sPoint(50, 75, 55));
    df->putSphere(sPoint(50, 75, 55), 5);

    //dver'
    df->putParallelepiped(sPoint(10, 45, 0), sPoint(20, 55, 50));
    df->putSphere(sPoint(17, 45, 30), 2);

    //okna
    df->putParallelepiped(sPoint(45, 95, 30), sPoint(65, 105, 50));
    df->putParallelepiped(sPoint(95, 70, 30), sPoint(105, 85, 50));

    //vaza
    df->putParallelepiped(sPoint(80, 55, 0), sPoint(95, 65, 35));
    df->putSphere(sPoint(87, 60, 45), 10);

    //stol
    df->putParallelepiped(sPoint(10, 75, 30), sPoint(40, 95, 35));

    df->putParallelepiped(sPoint(10, 75, 0), sPoint(15, 80, 30));
    df->putParallelepiped(sPoint(35, 75, 0), sPoint(40, 80, 30));
    df->putParallelepiped(sPoint(35, 90, 0), sPoint(40, 95, 30));
    df->putParallelepiped(sPoint(10, 90, 0), sPoint(15, 95, 30));

    df->putParallelepiped(sPoint(20, 80, 30), sPoint(30, 90, 40));

    //krisha
    sPoint A1(0, 50, 75);
    sPoint A2(100, 50, 75);
    sPoint A3(100, 100, 75);
    sPoint A4(0, 100, 75);
    sPoint A5(50, 75, 100);
    //df->putLine3D(A1, A2); df->putLine3D(A2, A3); df->putLine3D(A3, A4); df->putLine3D(A4, A1);
    df->putLine3D(A1, A5); df->putLine3D(A2, A5); df->putLine3D(A3, A5); df->putLine3D(A4, A5);
}
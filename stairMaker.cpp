#include "stairMaker.h"

stairMaker::stairMaker()
{
    //ctor
}

stairMaker::~stairMaker()
{
    //dtor
}

stairMaker::stairMaker(vector startP, vector endP, int numSteps)
{
    _startPoint = startP;
    _endPoint = endP;
    _totRun.x = endP.x - startP.x;   // x,0,0
    _totRise.y = endP.y - startP.y;  // 0,y,0
    _totWidth.z = endP.z - startP.z; // 0,0,z
    _steps = numSteps;
    _run = _totRun/numSteps;
    _rise = _totRise/numSteps + 1;
}

void stairMaker::createStair()
{
    vector v1,v2,v3,v4;
    v1 = _startPoint;
    v4 = v1 + _totWidth;
    for( int i = 0; i < _steps; i ++ )
        v2 = v1 + _rise;
        v3 = v1 + _rise;
        //draw ^ v1 -> v2 -> v3 -> v4

        v1 = v2 + _run;
        v4 = v3 + _run;
        //draw ^ v2 -> v1 -> v4 -> v3
}

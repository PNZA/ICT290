#ifndef STAIRMAKER_H
#define STAIRMAKER_H
#include <GL/glut.h>

typedef struct{
   long double x;
   long double y;
   long double z;
}vector;

class stairMaker
{
    public:
        stairMaker();
        virtual ~stairMaker();
        stairMaker(vector startP, vector endP, int numSteps);
        void createStair();
    private:
        vector _startPoint, _endPoint, _totRun, _totRise, _totWidth;
        float _run, _rise;
        int _steps;
};

#endif // STAIRMAKER_H

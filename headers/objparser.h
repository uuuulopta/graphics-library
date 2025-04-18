#pragma once
#include <iostream>
#include <string>
#include "Shapes.h"
#include "Canvas.h"
struct objData{
    vector<Line> lines;
};
vector<Line>  parseObj(string filepath, Canvas& c);
vector<Triangle>  TparseObj(string filepath, Canvas& c);
float mapZ(float z, float maxz, float minz);
float mapXY(float x, float max, float min);
v3 mapPoint3d(v3 p, float xmax, float xmin, float ymax, float ymin, float zmax, float zmin);

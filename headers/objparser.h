

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

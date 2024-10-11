#include "../headers/objparser.h"
#include "../headers/Canvas.h"
#include "../headers/Shapes.h"
#include <fstream>
#include <ostream>
#include <string>

/*
 * minz .. z .. maxz / / / / - minz
 * 0 ... z-minz ... maxz - minz /
 *
 * */
float mapZ(float z, float maxz, float minz) {
  // (z-lz)/(hz-lz)+1
  if (maxz == minz)
    return 0;
  // return 1 + ((z - minz) * 2) / (maxz - minz);
  // return (z - minz) / (maxz - minz) + 1;
  return z - (minz + (maxz - minz) / 2);
}

/*

minx.... x... maxx   / - minx

0 .... x-minx ... maxx-minxx  / 2 *

0 ...2(x-minx)  2(maxx-minx) /maxx-minx
0 2(x-minx)/(maxx-minx) 2 / -1
-1 (2(x-minx)/(maxx-minx) - 1) 1


minx.... x... maxx   / - minx
0 ... x ... maxx-minx

*/
float mapXY(float x, float max, float min) {
  if (max == min)
    return 0;
  // return (2.f * (x - min) / ((max - min))) - 1;
  return x - (min + (max - min) / 2);
}
/*
 * Normalises the points between X -1 - 1 Y -1 - 1 Z 1 - 2
 * */
v3 mapPoint3d(v3 p, float xmax, float xmin, float ymax, float ymin, float zmax,
              float zmin) {
  return v3(mapXY(p.x, xmax, xmin), mapXY(p.y, ymax, ymin),
            mapZ(p.z, zmax, zmin));
}
vector<Triangle> TparseObj(string filepath, Canvas &c) {
  float xmin = 9999;
  float xmax = -999;
  float ymin = 9999;
  float ymax = -99;
  float zmin = 9999;
  float zmax = -99;

  ifstream file;
  file.open(filepath, ios::in);
  if (!file) {
    cout << "Error: " << filepath << "  cannot be found";
    file.close();
    throwErr("File cannot be found");
  }
  vector<v3> vertices;
  vector<vector<int>> facemap;

  double xavg = 0;
  double yavg = 0;
  double zavg = 0;

  for (string line; getline(file, line);) {
    if (line[0] == '#' || line[1] != ' ')
      continue;
    if (line[0] == 'v' && line[1] == ' ') {
      size_t pos = 0;
      string token;
      string delimiter = " ";
      int substr_l = 2;
      if (line[1] == ' ' && line[2] == ' ')
        substr_l = 3;
      line = line.substr(substr_l, line.length() - 1);
      float points[3];
      int counter = 0;
      while ((pos = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, pos);

        points[counter] = stof(token);
        line.erase(0, pos + delimiter.length());
        counter++;
        cout << points[counter] << "\n";
      }
      points[counter] = stof(line);
      cout << points[counter] << "\n";
      vertices.push_back(v3(points[0], points[1], points[2]));
      if (points[0] > xmax) {
        xmax = points[0];
      }
      if (points[0] < xmin) {
        xmin = points[0];
      }
      if (points[1] > ymax) {
        ymax = points[1];
      }
      if (points[1] < ymin) {
        ymin = points[1];
      }
      if (points[2] > zmax) {
        zmax = points[2];
      }
      if (points[2] < zmin) {
        zmin = points[2];
      }
      counter = 0;
    } else if (line[0] == 'f') {
      size_t pos = 0;
      string token;
      string delimiter = " ";
      line = line.substr(2, line.length() - 1);
      vector<int> face;
      int counter = 0;
      while ((pos = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, pos);
        if (size_t newpos = token.find("/") != std::string::npos)
          token = line.substr(0, newpos);
        face.push_back(stoi(token));
        line.erase(0, pos + delimiter.length());
        counter++;
      }
      if (line.length() > 0 && line != "\r")
        face.push_back(stoi(line));
      facemap.push_back(face);
      counter = 0;
    }
  }

  cout << "Normalized\n" << "-------------\n";
  for (v3 &vert : vertices) {
    vert = mapPoint3d(vert, xmax, xmin, ymax, ymin, zmax, zmin);
    cout << vert.x << " " << vert.y << " " << vert.z << endl;
    xavg += vert.x;
    yavg += vert.y;
    zavg += vert.z;
  }
  vector<Line> lines;
  vector<Triangle> trigs;
  cout << "obj min/max values X: " << xmin << " - " << xmax << " Y: " << ymin
       << " - " << ymax << " Z: " << zmin << " - " << zmax << endl
       << " xavg: " << std::to_string(xavg / vertices.size())
       << " yavg:" << std::to_string(yavg / vertices.size())
       << " zavg: " << std::to_string(zavg / vertices.size());
  for (vector<int> face : facemap) {

    // p1 -> p2
    Line a = Line(vertices[face[0] - 1], vertices[face[1] - 1], {0, 0, 0});
    // p2 -> p3
    Line b = Line(vertices[face[1] - 1], vertices[face[2] - 1], {0, 0, 0});
    // p1 -> p3
    Line c = Line(vertices[face[0] - 1], vertices[face[2] - 1], {0, 0, 0});
    Triangle trig = Triangle(a, b, c);
    trigs.push_back(trig);
  }
  return trigs;
}

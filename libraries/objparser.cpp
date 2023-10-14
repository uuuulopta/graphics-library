#include "../headers/objparser.h"
#include "../headers/Shapes.h"
#include "../headers/Canvas.h"

float mapZ(float z,float maxz, float minz){
    // (z-lz)/(hz-lz)+1
    return (z - minz)/(maxz-minz) + 1;
}

/*

minx.... x... maxx   / - minx

0 .... x-minx ... maxx-minxx  / 2 * 

0 ...2(x-minx)  2(maxx-minx) /maxx-minx
0 2(x-minx)/(maxx-minx) 2 / -1
-1 (2(x-minx)/(maxx-minx) - 1) 1


*/
float mapXY(float x,float max, float min) {
    return  (2.f * (x - min) / (max - min)) - 1;
}
/* 
 * Normalises the points between X -1 - 1 Y -1 - 1 Z 1 - 2
 * */
Point3d mapPoint3d(Point3d p,int xmax, int xmin,int ymax, int ymin,int zmax, int zmin) {
    return Point3d(mapXY(p.x,xmax,xmin),mapXY(p.y,ymax,ymin),mapZ(p.z,zmax,zmin));
}
vector<Triangle> TparseObj(string filepath,Canvas& c)
{
    // change this
    float xmin = 9999;
    float xmax = -999;
    float ymin = 9999;
    float ymax = -99;
    float zmin = 9999;
    float zmax = -99;

    ifstream file;
    file.open(filepath, ios::in);
    if(!file){
        cout << "Error: " << filepath << "  cannot be found";
        file.close();
        throw -1; 
    }
    vector<Point3d> vertices;
    vector<vector<int>> facemap;
    for( string line; getline(file,line); )
    {
        // cout << line << endl;
        if (line[0] == 'v'){
            size_t pos = 0;
            string token;
            string delimiter = " ";
            line = line.substr(2,line.length()-1);
            float points[3];
            int counter = 0;
            while ((pos = line.find(delimiter)) != std::string::npos) {
                token = line.substr(0, pos);
                points[counter] = stof(token);
                line.erase(0, pos + delimiter.length());
                counter++;
            }
            points[counter] = stof(line);
            vertices.push_back(Point3d(points[0],points[1],points[2]));
            if (points[0] > xmax) { xmax = points[0]; }
            if (points[0] < xmin) { xmin = points[0]; }
            if (points[1] > ymax) { ymax = points[1]; }
            if (points[1] < ymin) { ymin = points[1]; }
            if (points[2] > zmax) { zmax = points[2]; }
            if (points[2] < zmin) { zmin = points[2]; }
            counter = 0;
            }
         if (line[0] == 'f'){
            size_t pos = 0;
            string token;
            string delimiter = " ";
            line = line.substr(2,line.length()-1);
            vector<int> face;
            int counter = 0;
            while ((pos = line.find(delimiter)) != std::string::npos) {
                token = line.substr(0, pos);
                face.push_back(stoi(token));
                line.erase(0, pos + delimiter.length());
                counter++;
            }
            face.push_back(stoi(line));
            facemap.push_back(face);
            counter = 0;
            }       
    }
    for (Point3d& vert: vertices){
        vert = mapPoint3d(vert,  xmax,  xmin,  ymax,  ymin,  zmax,  zmin);
    }
    vector<Line> lines;
    vector<Triangle> trigs;
    cout << "obj min/max values X: " << xmin << " - " << xmax << " Y: " << ymin << " - " << ymax << " Z: " << zmin << " - " << zmax << endl;
    for(vector<int> face: facemap){
       // p1 -> p2
       Line a = Line(vertices[face[0]-1].x,vertices[face[0]-1].y,vertices[face[1]-1].x,vertices[face[1]-1].y,{0,0,0},vertices[face[0]-1].z,vertices[face[1]-1].z);
       // p2 -> -3
       Line b = Line(vertices[face[1]-1].x,vertices[face[1]-1].y,vertices[face[2]-1].x,vertices[face[2]-1].y,{0,0,0},vertices[face[1]-1].z,vertices[face[2]-1].z);
       // p1 -> p3
       Line c = Line(vertices[face[0]-1].x,vertices[face[0]-1].y,vertices[face[2]-1].x,vertices[face[2]-1].y,{0,0,0},vertices[face[0]-1].z,vertices[face[2]-1].z);
       Triangle trig = Triangle(a,b,c);
       trigs.push_back(trig);
          }
    return trigs;

}

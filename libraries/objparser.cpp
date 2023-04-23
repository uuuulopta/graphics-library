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
vector<Line> parseObj(string filepath,Canvas& c)
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
    // for(Point3d& p: vertices){
    // cout << p.x << " " << p.y << " " << p.z << endl;
    // }
    // for(vector<int> face: facemap){
    //     for(int p: face){
    //         cout << p << " ";
    //     }
    //     cout << endl;
    // }
    vector<Line> lines;
    cout << "X: " << xmin << " - " << xmax << " Y: " << ymin << " - " << ymax << " Z: " << zmin << " - " << zmax << endl;
    for(vector<int> face: facemap){
       int counter = 0;
       int last;
        for(int p: face){
            if(counter == 0){
                last = p;
                counter++;
                continue;
            }
            
            
            

            // Line a = c.draw_line(vertices[last].x,vertices[last].y,vertices[p].x,vertices[p].y,1,{255,0,0});
            //cout << vertices.size() << " " << last << " " << p << endl;
            Line b = Line(mapXY(vertices[last-1].x,xmax,xmin),mapXY(vertices[last-1].y,ymax,ymin),mapXY(vertices[p-1].x,xmax,xmin),mapXY(vertices[p-1].y,ymax,ymin),{255,0,0},mapZ(vertices[last-1].z,zmax,zmin),mapZ(vertices[p-1].z,zmax,zmin));
            lines.push_back(b);
            counter++;
        }
    }
    return lines;
    // c.draw_line(100,100,200,200,4,{255,0,0});



}

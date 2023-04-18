#include "../headers/objparser.h"
#include "../headers/Shapes.h"
#include "../headers/Canvas.h"
vector<Line> parseObj(string filepath,Canvas& c)
{
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
            Line b = Line(vertices[last].x,vertices[last].y,vertices[p].x,vertices[p].y,{255,0,0},vertices[last].z,vertices[p].z);
            lines.push_back(b);
            counter++;
        }
    }
    return lines;
    // c.draw_line(100,100,200,200,4,{255,0,0});



}

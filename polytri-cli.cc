#include <cstdlib>
#include <iostream>
#include <fstream>

#include "polygon_triangulation.h"
using namespace std;


/** Read a data file to retrieve contour vertices */
int LoadSegments(char *const filename,
                 std::vector<uint32_t> &contour_lengths,
                 std::vector<vertex_t> &vertices)
{
  std::ifstream fd(filename);

  if (fd.fail()) {
    std::cerr << "Error : can't read the file." << std::endl;
    return EXIT_FAILURE;
  }

  unsigned int ncontours=1;

  if (ncontours == 0u) {
    std::cerr << "Invalid contour count : " << ncontours << std::endl;
    return EXIT_FAILURE;
  }
  
  for (auto cid = 0u; cid < ncontours; ++cid) {
    unsigned int npoints;
    fd >> npoints;
    contour_lengths.push_back(npoints);
    for (auto i = 0u; i < npoints; ++i) {
      vertex_t v;
      fd >> v.x >> v.y;
      vertices.push_back(v);
    }
  }

  return EXIT_SUCCESS;
}

void ExportData(PolygonTriangulation::TriangleBuffer_t &triangles,
                 std::vector<vertex_t> &vertices,std::vector<int> vert[3],list<int> *adj,int V,list<int> *org)
{
  const char* filename = "output.txt";

  std::ofstream fd(filename);
  if (fd.fail()) {
    std::cerr << "Error : cannot locate the file \"" << filename << "\"" << std::endl;
    exit(EXIT_FAILURE);
  }

  fd << "var TRI = [" << std::endl;

  for (auto &t : triangles) {
    fd << "   [ "
       << t.v0 << ", "<<vertices[t.v0].x<<" "<<vertices[t.v0].y<<std::endl
       << t.v1 << ", "<<vertices[t.v1].x<<" "<<vertices[t.v1].y<<std::endl
       << t.v2 << "]," <<vertices[t.v2].x<<" "<<vertices[t.v2].y<< std::endl;
  }
  fd << "];" << std::endl;
  fd << std::endl;

  fd << "var vertices = [" << std::endl;
  for (auto i = 0u; i < vertices.size(); ++i) {
    auto &v = vertices[i];
    fd << "   new TPoint( " << v.x << ", " << v.y << ")," << std::endl;
  }
  fd << "];" << std::endl;


  std:: ofstream output_file("triangulated");
  if (output_file.is_open()) {
    output_file << "#!/usr/bin/gnuplot\n";
    output_file << "reset\n";
    output_file << "set terminal png\n";
    output_file << "set yrange [0:600]\n";
    output_file << "set xrange [0:600]\n";
    output_file << "unset colorbox\n";
    // delaunay connections:
    output_file << "set style arrow 1 nohead lc rgb \'black\'\n";
    // voronoi boundaries:
    output_file << "set style arrow 2 nohead lc rgb \'red\'\n";

    for (auto &t : triangles) {
      output_file << "set arrow from "<<vertices[t.v0].x<<","<<vertices[t.v0].y<<" to "<<vertices[t.v1].x<<","<<vertices[t.v1].y<< " as 1\n"<<
                     "set arrow from "<<vertices[t.v1].x<<","<<vertices[t.v1].y<<" to "<<vertices[t.v2].x<<","<<vertices[t.v2].y<< " as 1\n"<<
                     "set arrow from "<<vertices[t.v2].x<<","<<vertices[t.v2].y<<" to "<<vertices[t.v0].x<<","<<vertices[t.v0].y<< " as 1\n"; 

    }
    // ending of the file:
    output_file << "plot NaN notitle\n";
    //output_file << "pause 1000\n"; // ensure that X11 stays visible

    output_file.close();
  }
  else {
    std::cout << "Error: Could not access output file for writing!\n";
  }


  std:: ofstream output_file2("3colored");
  if (output_file2.is_open()) {
    output_file2 << "#!/usr/bin/gnuplot\n";
    output_file2 << "reset\n";
    output_file2 << "set terminal png\n";
    output_file2 << "set yrange [0:600]\n";
    output_file2 << "set xrange [0:600]\n";
    output_file2 << "unset colorbox\n";
    // delaunay connections:
    output_file2 << "set style arrow 1 nohead lc rgb \'black\'\n";
    // voronoi boundaries:
    output_file2 << "set style arrow 2 nohead lc rgb \'red\'\n";

    for(int i=0;i<V;i++){
      for(auto &j:adj[i]){
        output_file2<<"set arrow from "<<vertices[i].x<<","<<vertices[i].y<<" to "<<vertices[j].x<<","<<vertices[j].y<< " as 1\n";
      }
    }

    for(int i=0;i<3;i++){
      for(auto &j:vert[i]){
        if(i==0)
          output_file2<<"set object circle at "<<vertices[j].x<<","<<vertices[j].y<<" size first 4 fc rgb 'navy'\n";
        else if(i==1)
          output_file2<<"set object circle at "<<vertices[j].x<<","<<vertices[j].y<<" size first 4 fc rgb 'red'\n";
        else
          output_file2<<"set object circle at "<<vertices[j].x<<","<<vertices[j].y<<" size first 4 fc rgb 'green'\n";
      }
    }
    // ending of the file:
    output_file2 << "plot NaN notitle\n";
    //output_file << "pause 1000\n"; // ensure that X11 stays visible

    output_file2.close();
  }
  else {
    std::cout << "Error: Could not access output file for writing!\n";
  }
  int s1=vert[0].size(),s2=vert[1].size(),s3=vert[2].size();

  cout<<"colors "<<s1<<" "<<s2<<" "<<s3<<endl;
  int index;
  if(s1<s2&&s1<s3){
    index=0;  
  }
  else if(s2<s1&&s2<s3){
    index=1;  
  }
  else{
    index=2; 
  }
  std:: ofstream output_file3("guards");
  if (output_file3.is_open()) {
    output_file3 << "#!/usr/bin/gnuplot\n";
    output_file3 << "reset\n";
    output_file3 << "set terminal png\n";
    output_file3 << "set yrange [0:600]\n";
    output_file3 << "set xrange [0:600]\n";
    output_file3 << "unset colorbox\n";
    // delaunay connections:
    output_file3 << "set style arrow 1 nohead lc rgb \'black\'\n";
    // voronoi boundaries:
    output_file3 << "set style arrow 2 nohead lc rgb \'red\'\n";

    for(int i=0;i<V;i++){
      for(auto &j:org[i]){
        output_file3<<"set arrow from "<<vertices[i].x<<","<<vertices[i].y<<" to "<<vertices[j].x<<","<<vertices[j].y<< " as 1\n";
      }
    }

      for(auto &j:vert[index]){
        output_file3<<"set object circle at "<<vertices[j].x<<","<<vertices[j].y<<" size first 4 fc rgb 'navy'\n";
      }
    // ending of the file:
    output_file3 << "plot NaN notitle\n";
    //output_file << "pause 1000\n"; // ensure that X11 stays visible

    output_file3.close();
  }
  else {
    std::cout << "Error: Could not access output file for writing!\n";
  }
}




/* -------------------------------------------------------------------------- */

//3 coloring
class Graph
{
public:
    int V;
    list<int> *adj;
    // Constructor and destructor
    Graph(int V)   { this->V = V; adj = new list<int>[V]; }
    ~Graph()       { delete [] adj; }

    // function to add an edge to graph
    void addEdge(int v, int w);

};

void Graph::addEdge(int v, int w)
{
  bool flag=0;
  for(const auto& i:adj[v]){
    if(i==w){
      flag=1;
      break;
    }
  }
  if(flag==0){
    adj[v].push_back(w);
    adj[w].push_back(v);
  }
    // Note: the graph is undirected
}

// Assigns colors (starting from 0) to all vertices and prints
// the assignment of colors
/*void greedyColoring()
{
    int result[V];

    // Assign the first color to first vertex
    result[0]  = 0;

    // Initialize remaining V-1 vertices as unassigned
    for (int u = 1; u < V; u++)
        result[u] = -1;  // no color is assigned to u

    // A temporary array to store the available colors. True
    // value of available[cr] would mean that the color cr is
    // assigned to one of its adjacent vertices
    bool available[V];
    for (int cr = 0; cr < V; cr++)
        available[cr] = false;

    // Assign colors to remaining V-1 vertices
    for (int u = 1; u < V; u++)
    {
        // Process all adjacent vertices and flag their colors
        // as unavailable
        list<int>::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
            if (result[*i] != -1)
                available[result[*i]] = true;

        // Find the first available color
        int cr;
        for (cr = 0; cr < V; cr++)
            if (available[cr] == false)
                break;

        result[u] = cr; // Assign the found color

        // Reset the values back to false for the next iteration
        for (i = adj[u].begin(); i != adj[u].end(); ++i)
            if (result[*i] != -1)
                available[result[*i]] = false;
    }

    // print the result
    for (int u = 0; u < V; u++)
        cout << "Vertex " << u << " --->  Color "
             << result[u] << endl;
}
*/
bool isSafe (int v, list<int> *adj, int color[], int c)
{
  for(const auto& i:adj[v]){
    if(c==color[i])
      return false;
  }
  return true;
}

bool threecolor(list<int> *adj, int color[], int v,int V)
{
    /* base case: If all vertices are assigned a color then
       return true */
    if (v == V)
        return true;
 
    /* Consider this vertex v and try different colors */
    for (int c = 1; c <= 3; c++)
    {
        /* Check if assignment of color c to v is fine*/
        if (isSafe(v, adj, color, c))
        {
           color[v] = c;
 
           /* recur to assign colors to rest of the vertices */
           if (threecolor(adj,color, v+1,V) == true)
             return true;
 
            /* If assigning color c doesn't lead to a solution
               then remove it */
           color[v] = 0;
        }
    }
 
    /* If no color can be assigned to this vertex then return false */
    return false;
}





int main(int argc, char *argv[])
{
  if (argc < 2) {
    std::cerr << "usage : " << argv[0] << " filename." << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<uint32_t> contour_lengths;
  std::vector<vertex_t> vertices;
  if (LoadSegments(argv[1u], contour_lengths, vertices)) {
    std::cerr << "error while reading the data file." << std::endl;
    return EXIT_FAILURE;
  }
  int V= vertices.size();
  list<int> *org=new list<int>[V];
  for(int i=0;i<V;i++){
    if(i==V-1)
      org[i].push_back(0);
    else  
      org[i].push_back(i+1);
  }
  PolygonTriangulation::TriangleBuffer_t triangles;
  PolygonTriangulation::Triangulate(
    contour_lengths.size(),
    contour_lengths.data(),
    vertices.data(),
    triangles
  );
  Graph g(V);

  for (auto &t : triangles) {
    g.addEdge(t.v0,t.v1);
    g.addEdge(t.v1,t.v2);
    g.addEdge(t.v2,t.v0);
  }

  int color[vertices.size()];
  for(int i=0;i<vertices.size();i++)
    color[i]=0;

  threecolor(g.adj,color,0,V);
  vector<int> vert[3];
  for(int i=0;i<V;i++){
    vert[color[i]-1].push_back(i);
  }
  ExportData(triangles, vertices,vert,g.adj,V,org);
  

  return EXIT_SUCCESS;
}

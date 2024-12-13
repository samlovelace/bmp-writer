#include <fstream>
#include <vector>
#include <cstdint>
#include "BmpImage.h"

int main() 
{
    int width = 110; 
    int height = 110; 
    BmpImage* img = new BmpImage(width, height); 
    Pixel BLUE = {255, 0, 0};
    Pixel RED = {0, 0, 255}; 
    Pixel GREEN {0, 255, 0};
    img->set_all_pixels(BLUE);

    Vertex2 a = {0, 0}; 
    Vertex2 b = {100, 100}; 
    Vertex2 c = {100, 0}; 

    std::vector<Vertex2> verts; 
    verts.push_back(a); 
    verts.push_back(b); 
    verts.push_back(c); 

    img->triangle(verts, GREEN); 
    
    img->write_to_file("../green.bmp"); 

    return 0;
}

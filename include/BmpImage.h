#ifndef BMPIMAGE
#define BMPIMAGE

#include <string> 
#include "Pixel.h"
#include "Vertex.h"
#include <vector>



class BmpImage 
{ 
public:
    BmpImage(int width, int height);
    ~BmpImage();

    bool write_to_file(const std::string& filename); 
    void set_all_pixels(Pixel color);
    void set_pixel(int x, int y, Pixel color); 
    void line(int x0, int y0, int x1, int y1, Pixel color); 
    void triangle(std::vector<Vertex2> vertices, Pixel color); 

private:
    std::string m_filename; 
    int m_width; 
    int m_height; 
    int m_row_size; 
    int m_image_size; 
    int m_file_size; 

    std::vector<Pixel> m_pixels; 
    
    static constexpr size_t m_header_size = 14; 
    static constexpr size_t m_dibHeader_size = 40; 
    uint8_t m_header[m_header_size];
    uint8_t m_dibHeader[m_dibHeader_size];


   
};
#endif //BMPIMAGE
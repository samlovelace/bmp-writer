
#include "BmpImage.h"
#include <cstring> 
#include <fstream>


BmpImage::BmpImage(int width, int height) : 
    m_filename(""), m_width(width), m_height(height), m_row_size(0), 
    m_image_size(0), m_file_size(0),  m_pixels(width*height)
{
    // File header (14 bytes)
    uint8_t fileHeader[14] = {
        'B', 'M',            // Signature
        0, 0, 0, 0,          // File size (to be filled later)
        0, 0, 0, 0,          // Reserved
        54, 0, 0, 0          // Pixel data offset (54 bytes)
    };

    // DIB header (40 bytes)
    uint8_t dibHeader[40] = {
        40, 0, 0, 0,         // Header size
        0, 0, 0, 0,          // Image width (to be filled later)
        0, 0, 0, 0,          // Image height (to be filled later)
        1, 0,                // Planes
        24, 0,               // Bits per pixel (24-bit)
        0, 0, 0, 0,          // Compression (none)
        0, 0, 0, 0,          // Image size (can be 0 for no compression)
        0, 0, 0, 0,          // X pixels per meter
        0, 0, 0, 0,          // Y pixels per meter
        0, 0, 0, 0,          // Colors in color table
        0, 0, 0, 0           // Important color count
    };

    // Fill in file size
    int rowSize = (3 * width + 3) & ~3;  // Row size (padded to multiple of 4)
    int imageSize = rowSize * height;
    int fileSize = 54 + imageSize;

    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize >> 8;
    fileHeader[4] = fileSize >> 16;
    fileHeader[5] = fileSize >> 24;

    // Fill in width and height
    dibHeader[4] = width;
    dibHeader[5] = width >> 8;
    dibHeader[6] = width >> 16;
    dibHeader[7] = width >> 24;

    dibHeader[8] = height;
    dibHeader[9] = height >> 8;
    dibHeader[10] = height >> 16;
    dibHeader[11] = height >> 24;

    // copy to member variables
    std::memcpy(m_header, fileHeader, 14);
    std::memcpy(m_dibHeader, dibHeader, 40); 
    m_row_size = rowSize; 
    m_image_size = imageSize; 
    m_file_size = fileSize; 
}

BmpImage::~BmpImage()
{

}

bool BmpImage::write_to_file(const std::string& filename)
{
    // Write to file
    std::ofstream outFile(filename, std::ios::binary);
    outFile.write(reinterpret_cast<char*>(m_header), 14);
    outFile.write(reinterpret_cast<char*>(m_dibHeader), 40);

    // Write pixel data row by row with padding
    uint8_t padding[3] = {0, 0, 0};
    for (int y = 0; y < m_height; ++y) {
        outFile.write(reinterpret_cast<char*>(&m_pixels[y * m_width]), m_width * 3);
        outFile.write(reinterpret_cast<char*>(padding), m_row_size - m_width * 3);
    }

    outFile.close();

    return true; 
}

void BmpImage::set_pixel(int x, int y, Pixel color)
{
    m_pixels[y*m_width + x] = color; 
}

void BmpImage::set_all_pixels(Pixel color)
{
    for(int y = 0; y < m_height; y++)
    {
        for(int x = 0; x < m_width; x++)
        {
            set_pixel(x, y, color); 
        }
    }
}

void BmpImage::line(int x0, int y0, int x1, int y1, Pixel color) 
{
    // Implements Bressenham's algorithm 
    // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

    bool steep = abs(y1 - y0) > abs(x1 - x0);

    // If the line is steep, swap x and y
    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    // Ensure x0 < x1 for consistency
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = abs(y1 - y0);
    int error = dx / 2; // Initialize the error term
    int yStep = (y0 < y1) ? 1 : -1; // Determine the direction of y increment
    int y = y0;

    for (int x = x0; x <= x1; ++x) {
        // Plot the point
        if (steep) {
            set_pixel(y, x, color); // If steep, swap back x and y
        } else {
            set_pixel(x, y, color);
        }

        // Update the error term and y coordinate
        error -= dy;
        if (error < 0) {
            y += yStep;
            error += dx;
        }
    }
}

void BmpImage::triangle(std::vector<Vertex2> vertices, Pixel color)
{
    auto v0 = vertices[0]; 
    auto v1 = vertices[1]; 
    auto v2 = vertices[2]; 

    line(v0.x, v0.y, v1.x, v1.y, color);
    line(v1.x, v1.y, v2.x, v2.y, color);
    line(v2.x, v2.y, v0.x, v0.y, color); 
}


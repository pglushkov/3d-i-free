
#include "misc_utils.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <FreeImage.h>
#include <stdio.h>

namespace my_utils
{

std::string ReadTextFromFile(const char* file)
{
        std::ifstream read;
        read.open(file, std::ifstream::in);
        std::vector<char> data;

        if( read.good() ) {
                read.seekg(0, std::ios::end);
                size_t file_sz = read.tellg();
                read.seekg(0);
                data.resize(file_sz + 1);
                read.read(&data[0], file_sz);
                data[file_sz] = '\0';

        } else {
                throw std::runtime_error("Could not open specified file!");
        }

        read.close();
        return std::string(&data[0]);
}


std::string GetFileExtension(const std::string& file_name)
{
        size_t pos = file_name.find_last_of(".");
        if ( pos != std::string::npos)
                return file_name.substr(pos + 1);
        else
                return "";
}

void ReadImageData(const char* file, std::vector<unsigned char>& buffer,
                unsigned int &width, unsigned int &height, GLuint &gl_flag)
{
        FIBITMAP* bmap;

        /* for now only bmp / jpeg / png files are supported ... */
        std::string file_ext = my_utils::GetFileExtension(file);
        if ( file_ext == "")
                throw std::runtime_error("Error! Specified texture file has no extension!");

        if ( file_ext == "jpg")
                bmap = FreeImage_Load(FIF_JPEG, file, JPEG_DEFAULT);
        else if (file_ext == "bmp")
                bmap = FreeImage_Load(FIF_BMP, file, BMP_DEFAULT);
        else if (file_ext == "png")
                bmap = FreeImage_Load(FIF_PNG, file, PNG_DEFAULT);
        else
                throw std::runtime_error("Error! Specifed texture file must be .jpg or .bmp or .png!");

        if (!bmap)
                throw std::runtime_error("Error! Could not read data from specified texture file!");

        int color_type = FreeImage_GetColorType(bmap);
        if (color_type != FIC_RGB && color_type != FIC_RGBALPHA)
                throw std::runtime_error("Error! Unsupported data format in specified texture file!");

        unsigned int bpp = FreeImage_GetBPP(bmap);
        width = FreeImage_GetWidth(bmap);
        height = FreeImage_GetHeight(bmap);
        unsigned int bytes_per_line = FreeImage_GetLine(bmap);
        unsigned int bytes_per_pixel = bytes_per_line / width;
        unsigned int bytes_to_copy = bytes_per_pixel * width * height;
        void* image_data = FreeImage_GetBits(bmap);

        if (bytes_per_pixel == 3)
                gl_flag = GL_RGB;
        else if (bytes_per_pixel == 4)
                gl_flag = GL_RGBA;
        else
                throw std::runtime_error("Error! For now only RGB-24-bit and RGBA-32-bit is supported!");

        buffer.resize(bytes_to_copy);

        /* print something about it ... */
        std::cout << "FIBITMAP = " << bmap << ", BPP = " << bpp
                  << ", Width = " << width << ", Height = " << height
                  << ", Bytes-per-pixel = " << bytes_per_pixel
                  << ", ColorType = " << color_type << ", DataBits = "
                  << image_data << std::endl;

        for (size_t h = 0; h < height; ++h) {
                /* getting address of first pixel in current line */
                BYTE* bits = FreeImage_GetScanLine(bmap, h);
                for (unsigned int w = 0; w < width; ++w) {
                        size_t buf_idx = h * bytes_per_line + w * bytes_per_pixel;
                        buffer[ buf_idx + 0] = bits[FI_RGBA_RED];
                        buffer[ buf_idx + 1] = bits[FI_RGBA_GREEN];
                        buffer[ buf_idx + 2] = bits[FI_RGBA_BLUE];
                        /* jump to next pixel in current line*/
                        bits += bytes_per_pixel;

                        /* DBG */
                        // printf("texbufidx = %lu  of %u ...\n", buf_idx, bytes_to_copy);
                }
        }

        //memcpy(&buffer[0], image_data, bytes_to_copy);

        FreeImage_Unload(bmap);
}

std::vector<unsigned char> GenerateRgbTexture(size_t w, size_t h, unsigned char R, unsigned char G, unsigned
                char B)
{
        std::vector<unsigned char> result(w * h * 3);
        for (unsigned int k = 0; k < h; ++k) {
                for (unsigned int m = 0; m < w; ++m) {
                        result[k * w + m * 3 + 0] = R;
                        result[k * w + m * 3 + 1] = G;
                        result[k * w + m * 3 + 2] = B;
                }
        }
        return result;
}

std::vector<unsigned char> GenerateRgbaTexture(size_t w, size_t h, unsigned char R, unsigned char G, unsigned
                char B, unsigned char A)
{
        std::vector<unsigned char> result(w * h * 4);
        for (unsigned int k = 0; k < h; ++k) {
                for (unsigned int m = 0; m < w; ++m) {
                        result[k * w + m * 4 + 0] = R;
                        result[k * w + m * 4 + 1] = G;
                        result[k * w + m * 4 + 2] = B;
                        result[k * w + m * 4 + 3] = A;
                }
        }
        return result;
}

} /* of namespace my_utils */


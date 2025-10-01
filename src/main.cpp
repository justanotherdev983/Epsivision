#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>
#include <cstring>
#include <functional>
#include <cstring>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define WIDTH 400
#define HEIGHT 400

float clamp_float(float v, float lo, float hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} rgb32_t;

typedef struct {
    float x, y, z;
} vec3f;

static rgb32_t pixels[WIDTH * HEIGHT];

enum class command {
    generate,
    invalid,
};

enum class output_type {
    image,
    video,
    invalid,
};

command parse_command(const char* str) {
    if (strcmp(str, "generate") == 0) return command::generate;
    return command::invalid;
}

output_type parse_output_type(const char* str) {
    if (strcmp(str, "img") == 0) return output_type::image;
    if (strcmp(str, "vid") == 0) return output_type::video;
    return output_type::invalid;
}

enum class commands {
    generate,
};


void print_usage(const char* program_name) {
    std::cout << "Usage: " << program_name << " <command> <output_type> <input_file> <output_file>\n"
              << "Commands:\n"
              << "  generate    Generate visualization from input file\n"
              << "Output types:\n"
              << "  img         Generate static image\n"
              << "  vid         Generate video\n"
              << "Examples:\n"
              << "  " << program_name << " generate img input.txt output.png\n"
              << "  " << program_name << " generate vid input.txt output.mp4\n";
}

vec3f cool_quadrant_img(float x, float y) {
    if (x*y >= 0) {
        return vec3f{x, y, 1};
    } else {
        float fm = fmodf(x, y);
        return vec3f{fm, fm, fm};
    }

}

void gen_pixels(std::function<vec3f(float x, float y)> f) {
    for (size_t y = 0; y < HEIGHT; ++y) {
        float ny = (float)y/WIDTH*2.0f -1;
        for (size_t x = 0; x < WIDTH; ++x) {
           float nx = (float)x/WIDTH*2.0f -1;
           
            vec3f c = f(ny, nx);
            size_t index = y*WIDTH + x;
            pixels[index].r = clamp_float((c.x + 1)/2*255, 0, 255);
            pixels[index].g = clamp_float((c.y + 1)/2*255, 0, 255);
            pixels[index].b = clamp_float((c.z + 1)/2*255, 0, 255);
            pixels[index].a = 255;
       }
   } 
}

int main(int argc, char** argv) {
    std::cout << "Starting hash visualiser\n";
    
    if (argc != 5) {
        std::cerr << "Error: Expected 4 arguments, got " << (argc - 1) << "\n\n";
        print_usage(argv[0]);
        return 1;
    }
    
    command cmd = parse_command(argv[1]);
    if (cmd == command::invalid) {
        std::cerr << "Error: Invalid command '" << argv[1] << "'\n\n";
        print_usage(argv[0]);
        return 1;
    }
    
    output_type out_type = parse_output_type(argv[2]);
    if (out_type == output_type::invalid) {
        std::cerr << "Error: Invalid output type '" << argv[2] << "'\n\n";
        print_usage(argv[0]);
        return 1;
    }
    
    std::string input_file_path = argv[3];
    std::string output_file_path = argv[4];
    
    if (cmd == command::generate) {
        if (out_type == output_type::image) {
            std::cout << "Creating image from " << input_file_path << " -> " << output_file_path << "\n";

            //int stbi_write_png(char const *filename, int w, int h, int comp, const void  *data, int stride_in_bytes);
            gen_pixels(cool_quadrant_img);
            if(!stbi_write_png(output_file_path.c_str(), WIDTH, HEIGHT, 4, pixels, WIDTH*sizeof(rgb32_t))) {
                std::cout << "Couldnt create PNG img\n";
            } else {
                std::cout << "Created image successfully " << input_file_path << " -> " << output_file_path << "\n"; 

            }
        } else if (out_type == output_type::video) {
            std::cout << "Creating video from " << input_file_path << " -> " << output_file_path << "\n";
            
        }
    }

    return 0;
}

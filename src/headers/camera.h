#ifndef CAMERA_H
#define CAMERA_H

#include "utility.h"

#include "color.h"
#include "hittable.h"

#include <fstream>
#include <vector>

class camera {
    public:
        double aspect_ratio = 1.0;           // Ratio of image width over height
        int    image_width = 1024;                  // Rendered image width in pixel count
        int    samples_per_pixel = 10;              // Count of random samples for each pixel
        int    max_depth = 10;                      // Maximum number of ray bounces into scene
        int    pixel_u = 0;
        int    pixel_v = 0;
        
        std::ofstream outputFile;

        camera() { initialize(); }
        camera(int w, double a_r) : image_width(w), aspect_ratio(a_r) { initialize(); }
        ~camera() { dispose(); }

        // render the whole image
        void render_image(const hittable& world) {
            // Reset output before rendering a new one
            reset_image();

            // Write ppm header
            outputFile << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            // Render each pixel
            std::clog << "Rendering " << image_width << " x " << image_height << " image...\n";
            for (int j = 0; j < image_height; ++j) {
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
                for (int i = 0; i < image_width; ++i) {
                    render_pixel(world, i, j);
                }
            }

            std::clog << "\rDone.                 \n";
        }
    
        // Render a single pixel
        void render_pixel(const hittable& world, int u, int v) {
            color pixel_color(0, 0, 0);
            for (int sample = 0; sample < samples_per_pixel; ++sample) {
                ray r = get_ray(u, v);
                pixel_color += ray_color(r, max_depth, world);
            }
            write_color(outputFile, pixel_color, samples_per_pixel);
        }

    private:
        int    image_height;   // Rendered image height
        point3 center;         // Camera center
        point3 pixel00_loc;    // Location of pixel 0, 0
        vec3   pixel_delta_u;  // Offset to pixel to the right
        vec3   pixel_delta_v;  // Offset to pixel below

        void initialize() {
            // Open output file
            outputFile.open("output.ppm", std::ios::out | std::ios::trunc);

            // Setup basic member vars
            image_height = static_cast<int>(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            center = point3(0, 0, 0);

            // Determine viewport dimensions.
            auto focal_length = 1.0;
            auto viewport_height = 2.0;
            auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

            // Calculate the vectors across the horizontal and down the vertical viewport edges.
            auto viewport_u = vec3(viewport_width, 0, 0);
            auto viewport_v = vec3(0, -viewport_height, 0);

            // Calculate the horizontal and vertical delta vectors from pixel to pixel.
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // Calculate the location of the upper left pixel.
            auto viewport_upper_left =
                center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

            // Log camera properties
            std::clog << "Initialized Camera" << std::endl;
            std::clog << "Image Width: " << image_width << std::endl;
            std::clog << "Image Height: " << image_height << std::endl;
            std::clog << "Samples per Pixel: " << samples_per_pixel << std::endl;
            std::clog << "Max Depth: " << max_depth << std::endl;
        }

        int reset_image() {
            // Check if the file is open
            if (!outputFile.is_open()) {
                std::cerr << "Failed to open the file." << std::endl;
                return 1; // Return an error code
            }

            // reset pixel data
            outputFile << "P3\n" << image_width << ' ' << image_height << "\n255\n";
            for (int j = 0; j < image_height; ++j) {
                for (int i = 0; i < image_width; ++i) {
                    outputFile  << 0 << ' '
                                << 0 << ' '
                                << 0 << '\n';
                }
            }

            outputFile.seekp(0, std::ios::beg);

            return 0; // Return success
        }

        void dispose() {
            // Close the file
            outputFile.close();

            std::clog << "Disposed Camera\n";
        }

        ray get_ray(int i, int j) const {
            // Get a randomly sampled camera ray for the pixel at location i,j.
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto pixel_sample = pixel_center + pixel_sample_square();

            auto ray_origin = center;
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        vec3 pixel_sample_square() const {
            // Returns a random point in the square surrounding a pixel at the origin.
            auto px = -0.5 + random_double();
            auto py = -0.5 + random_double();
            return (px * pixel_delta_u) + (py * pixel_delta_v);
        }

        // shoot rays
        color ray_color(const ray& r, int depth, const hittable& world) const {
            hit_record rec;

            // If we've exceeded the ray bounce limit, no more light is gathered.
            if (depth <= 0)
                return color(0, 0, 0);

            if (world.hit(r, interval(0.001, infinity), rec)) {
                vec3 direction = random_on_hemisphere(rec.normal);
                return 0.5 * ray_color(ray(rec.p, direction), depth - 1, world);
            }

            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5 * (unit_direction.y() + 1.0);
            return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
        }
};

#endif
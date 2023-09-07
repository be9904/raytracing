#include <iostream>

#include "headers/window.h"

#include "headers/utility.h"

#include "headers/camera.h"
#include "headers/hittable_list.h"
#include "headers/sphere.h"

int main() {
    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Camera
    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 1280;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    // Render
    cam.render_image(world);

    // create window
    if (create_window() == -1) {
        std::clog << "Error in create_window\n";
        return -1;
    }
    else
        std::clog << "Successfully initialized window\n";

    while (!glfwWindowShouldClose(main_window)) {
                        // update loop (TODO)
        render();       // render loop
    }

    glfwTerminate();
    return 0;
}
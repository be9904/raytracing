#include <iostream>

#include "headers/window.h"

#include "headers/utility.h"

#include "headers/camera.h"
#include "headers/hittable_list.h"
#include "headers/sphere.h"
#include "headers/cgutil.h"

int main() {
    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Camera
    camera cam(1280, 16.0 / 9.0);

    // cam.aspect_ratio = 16.0 / 9.0;
    // cam.image_width = 1280;
    cam.samples_per_pixel = 50;
    cam.max_depth = 50;

    // Render
    cam.render_image(world);

    // Create window
    if (create_window() == -1) {
        std::clog << "Error in create_window\n";
        return -1;
    }
    else
        std::clog << "Successfully started raytracer\n";

    // Create program
    create_program();

    // Setup viewport
    float viewport[] = {
        // position             // uv coord
        -1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
        +1.0f, -1.0f, 0.0f,     1.0f, 0.0f,
        +1.0f, +1.0f, 0.0f,     1.0f, 1.0f,
        -1.0f, +1.0f, 0.0f,     0.0f, 1.0f
    };
    unsigned int viewport_idx[6] = { 
        0, 1, 3,
        1, 2, 3
    };

    // Generate vertex buffer
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(viewport), viewport, GL_STATIC_DRAW);

    // Generate index buffer
    GLuint index_buffer;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(viewport_idx), viewport_idx, GL_STATIC_DRAW);

    // Generate vertex array
    GLuint vertex_array = 0;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);   // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)3);   // uv attribute
    glEnableVertexAttribArray(1);

    setup_texture();

    while (!glfwWindowShouldClose(main_window)) {
                        // update loop (TODO)
        render();       // render loop
    }

    glfwTerminate();
    return 0;
}
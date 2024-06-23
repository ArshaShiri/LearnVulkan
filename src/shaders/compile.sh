#!/bin/sh
cd "${0%/*}"

/usr/local/bin/glslc simple_shaders/simple_shader.vert -o simple_shaders/simple_shader.vert.spv
/usr/local/bin/glslc simple_shaders/simple_shader.frag -o simple_shaders/simple_shader.frag.spv

/usr/local/bin/glslc simple_shaders/point_light.vert -o simple_shaders/point_light.vert.spv
/usr/local/bin/glslc simple_shaders/point_light.frag -o simple_shaders/point_light.frag.spv

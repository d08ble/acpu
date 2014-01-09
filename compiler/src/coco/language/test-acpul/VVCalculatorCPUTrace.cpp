#include <iostream>
#include "VVCalculatorCPU.h"

//typedef void * id;

#define LOG printf
void vv_cpu_owner_watch(float v)
{
    LOG("watch(%f)\n", v);
}

void vv_cpu_owner_create(float index, float parent, float object, float data, float formula, float param)
{
    LOG("create(%f, %f, %f, %f, %f, %f)\n", index, parent, object, data, formula, param);
}
/*
float vv_cpu_owner_call(float index, float formulaId, float p0, float p1, float p2, float p3, float p4, float p5, float p6, float p7)
{
    LOG("call(%f, %f, %f, %f, %f, %f, %f, %f, %f)", formulaId, p0, p1, p2, p3, p4, p5, p6, p7);
}
*/
void vv_cpu_owner_shader(float vertex, float fragment)
{
    LOG("shader(%f, %f)\n", vertex, fragment);
}

void vv_cpu_owner_uniform(float param, float v)
{
    LOG("uniform(%f, %f)\n", param, v);
}

void vv_cpu_owner_uniform2(float param, float v0, float v1)
{
    LOG("uniform2(%f, %f, %f)\n", param, v0, v1);
}

void vv_cpu_owner_uniform4(float param, float v0, float v1, float v2, float v3)
{
    LOG("uniform4(%f, %f, %f, %f)\n", param, v0, v1, v2, v3);
}

void vv_cpu_owner_draw(float formula)
{
    LOG("draw(%f)\n", formula);
}

void vv_cpu_owner_point(float x0, float y0)
{
    LOG("point(%f, %f)\n", x0, y0);
}

void vv_cpu_owner_line(float type, float x0, float y0, float x1, float y1)
{
    LOG("line(%f, %f, %f, %f, %f)\n", type, x0, y0, x1, y1);
}

void vv_cpu_owner_rect(float type, float x0, float y0, float x1, float y1)
{
    LOG("rect(%f, %f, %f, %f, %f)\n", type, x0, y0, x1, y1);
}

void vv_cpu_owner_circle(float type, float x0, float y0, float radius, float angle, float segments, float drawLineToCenter)
{
    LOG("circle(%f, %f, %f, %f, %f, %f, %f)\n", type, x0, y0, radius, angle, segments, drawLineToCenter);
}

void vv_cpu_owner_quad_bezier(float type, float x0, float y0, float x1, float y1, float x2, float y2, float segments)
{
    LOG("qbezier(%f, %f, %f, %f, %f, %f, %f, %f)\n", type, x0, y0, x1, y1, x2, y2, segments);
}

void vv_cpu_owner_color(float r, float g, float b, float a)
{
    LOG("color(%f, %f, %f, %f)\n", r, g, b, a);
}

void vv_cpu_owner_psize(float sz)
{
    LOG("psize(%f)\n", sz);
}

void vv_cpu_owner_cubic_bezier(float type, float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float segments)
{
    LOG("cbezier(%f, %f, %f, %f, %f, %f, %f, %f, %f, %f)\n", type, x0, y0, x1, y1, x2, y2, x3, y3, segments);
}

void vv_cpu_owner_triangle(float type, float x0, float y0, float x1, float y1, float x2, float y2)
{
    LOG("triangle(%f, %f, %f, %f, %f, %f, %f)\n", type, x0, y0, x1, y1, x2, y2);
}

void vv_cpu_owner_point4(float type, float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3)
{
    LOG("point4(%f, %f, %f, %f, %f, %f, %f, %f, %f)\n", type, x0, y0, x1, y1, x2, y2, x3, y3);
}

// io module
float vv_cpu_owner_io(float address, float p0, float p1, float p2, float p3)
{
    LOG("io(%f, %f, %f, %f, %f)\n", address, p0, p1, p2, p3);
}

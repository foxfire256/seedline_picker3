#ifndef JDS_OPENGL_H
#define JDS_OPENGL_H

#include <GL/glew.h>
#include <GL/gl.h>


namespace jds
{
#define print_opengl_error() print_opengl_error2((char *)__FILE__, __LINE__)
int print_opengl_error2(char *file, int line);
void start_glew();
void print_opengl_info();
float get_glsl_version();
void print_glm_version();
void check_draw_framebuffer();
}

#endif // JDS_OPENGL_H


#include "../Binding_pch.h"

#include <glbinding/gl/functions.h>

using namespace glbinding;


namespace gl
{


void glQueryCounter(GLuint id, GLenum target)
{
    return Binding::QueryCounter(id, target);
}
    
GLbitfield glQueryMatrixxOES(GLfixed * mantissa, GLint * exponent)
{
    return Binding::QueryMatrixxOES(mantissa, exponent);
}
    
void glQueryObjectParameteruiAMD(GLenum target, GLuint id, GLenum pname, GLuint param)
{
    return Binding::QueryObjectParameteruiAMD(target, id, pname, param);
}
    


} // namespace gl

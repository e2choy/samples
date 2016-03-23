#ifndef _SHADER_
#define _SHADER_
#include <glew.h>
#include "nuss_matrix.h"
class Shader{
public:
    Shader();

    int      Create(const char* frag, const char* vert);
    void     UseProgram(bool b = true);
    GLuint   GetProgId() const { return m_progId; }
private:
    char*    ReadCode(const char* filename);
    int      CreateShaderObj(const char* filename, int shaderType, GLuint* shaderId);
    GLint    CreateShaderProgram(GLint vertShaderId, GLint fragShaderId, GLuint* shaderProgId);

    GLuint      m_progId;
    GLuint      m_fragId;
    GLuint      m_vertId;
};
void CopyMatrixToShader(const Matrix4f& matrix, const char* name);

#endif
#include "shader.h"

///////////////////////////////////////////////////////////////////
void CopyMatrixToShader( const Matrix4f& matrix, const char* name, Shader& shader ){
    int loc = glGetUniformLocation( shader.GetProgId(), name );
    assert( loc != -1 );
    glUniformMatrix4fv( loc, 1, true, matrix.data() );
}

///////////////////////////////////////////////////////////////////
Shader::Shader():m_progId(-1),m_fragId(-1), m_vertId(-1){
}

int Shader::Create( const char* vert, const char* frag ){
    int rc = 0;
    rc = CreateShaderObj( vert, GL_VERTEX_SHADER, &m_vertId);
    if (rc == 0) {
        rc = CreateShaderObj(frag, GL_FRAGMENT_SHADER, &m_fragId);
    }
    if (rc == 0) {
        rc = CreateShaderProgram(m_vertId, m_fragId, &m_progId);
    }
    glUseProgram( m_progId );
    return rc;
}

char* Shader::ReadCode( const char* filename ){
    char * shaderCode = NULL;
    int codeLength = 0;
    FILE *fp = NULL;

    // check for error in file name
    fp = fopen(filename, "r");	
    if (fp == NULL) { return NULL; }

    // fine the length of code
    fseek(fp, 0L, SEEK_END);
    codeLength = ftell(fp);
    rewind(fp);	

    if (codeLength > 0) {
        // allocated space for code and check for errors
        shaderCode = (char *)malloc(codeLength + 1);	
        if (shaderCode == NULL) return(NULL);
        memset((void *)shaderCode, 0, codeLength + 1);
        fread((void *)shaderCode, sizeof(char), codeLength, fp);
        shaderCode[codeLength] = 0;
    }

    if (fp != NULL) fclose(fp);
    return shaderCode;
}

int	Shader::CreateShaderObj( const char* filename, int shaderType, GLuint* shaderId ){
    char *code = NULL;
    int rc = 0;

    // create a shader handle
    *shaderId = glCreateShader(shaderType);

    // read the code
    code = ReadCode(filename);
    if (code == NULL) return(-1);

    // attach the source to the shaders
    const char* _code = code;
    glShaderSource(*shaderId, 1, &_code, NULL);

    // compile the code
    glCompileShader(*shaderId);

    // check for errors
    glGetShaderiv(*shaderId, GL_COMPILE_STATUS, &rc);
    if (rc != GL_TRUE) {
        GLsizei length;
        GLsizei bufSize = 0;
        char *error = NULL;
        fprintf(stderr, "Error when creating a shader \n");
        glGetShaderiv(*shaderId, GL_INFO_LOG_LENGTH, &bufSize);
        error = (char *)malloc(bufSize*sizeof(char) + 1);
        if (error != NULL) {
            glGetShaderInfoLog(*shaderId, bufSize, &length, error);
            fprintf(stderr, "%s \n", error);
        }
        if (error != NULL) free(error);
        rc = -1;
    }
    else rc = 0;
    if (code != NULL) free(code);	// free the source code of the shader
    return(rc);
}

GLint Shader::CreateShaderProgram( GLint vertShaderId, GLint fragShaderId, GLuint* shaderProgId ){
    int rc = 0;
    // get a handle to the shader program
    m_progId = glCreateProgram();

    // connect the shaders subprogram to the "main" program
    glAttachShader( m_progId, m_vertId );
    rc = glGetError();
    if (rc != GL_NO_ERROR) {
        fprintf(stderr, "error in attach shaders \n");
        rc = -1;
        goto err;
    }

    glAttachShader(m_progId, fragShaderId);
    rc = glGetError();
    if (rc != GL_NO_ERROR) {
        fprintf(stderr, "error in attach shaders \n");
        rc = -1;
        goto err;
    }

    glLinkProgram(m_progId);
    glGetProgramiv(m_progId, GL_LINK_STATUS, &rc);

    if (rc != GL_TRUE) {
        rc = -1;
        GLsizei length;
        GLsizei bufSize = 0;
        char *error = NULL;
        fprintf(stderr, "Error when creating a shader program \n");
        glGetProgramiv(m_progId, GL_INFO_LOG_LENGTH, &bufSize);
        error = (char *)malloc(bufSize*sizeof(char) + 1);
        if (error != NULL) {
            glGetProgramInfoLog(m_progId, bufSize, &length, error);
            fprintf(stderr, "%s \n", error);
        }
        if (error != NULL) free(error);
    }
    else rc = 0;

    if (shaderProgId != NULL) *shaderProgId = m_progId;
err:
    return (rc);
}

void Shader::UseProgram( bool b ){
    if( b ){
        glUseProgram( m_progId );
    }else{
        glUseProgram( 0 );
    }
}

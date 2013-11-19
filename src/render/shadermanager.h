#ifndef SHADER_MANAGER
#define SHADER_MANAGER

#include <string>
#include "defines.h"


class ShaderManager
{
	public:
		ShaderManager();
		~ShaderManager();
		
		GLuint CreateProgramVF( const std::string& parVertex,const std::string& parFragment);
		GLuint CreateProgramC( const std::string& parCompute);
		void BindProgram( GLuint parProgram);


		GLuint GenerateTexture(size_t parW, size_t parH);
		void BindTexture(GLuint parShaderID, size_t parIndex, const std::string& parName);
};


#endif // SHADER_MANAGER

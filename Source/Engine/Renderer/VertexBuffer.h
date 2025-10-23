#pragma once
#include "Resources/Resource.h"
#include <glad/glad.h>
#include <vector>

namespace neu
{
	class VertexBuffer : public Resource
	{
	public:
		VertexBuffer();
		virtual ~VertexBuffer();

		void Draw(GLenum primitiveType = GL_TRIANGLES);
		void Bind() { glBindVertexArray(m_vao); }

		void CreateVertexBuffer(GLsizei size, GLsizei count, GLvoid* data);
		void CreateIndexBuffer(GLenum indexType, GLsizei count, GLvoid* data);

		void SetAttribute(GLuint index, GLint size, GLsizei stride, GLuint offset);

	protected:
		// vertex array
		GLuint m_vao = 0;  

		// vertex buffer
		GLuint m_vbo = 0;  
		GLuint m_vertexCount = 0;

		// index buffer
		GLuint m_ibo = 0; 
		GLuint m_indexCount = 0;
		GLenum m_indexType = 0; 
	};
}
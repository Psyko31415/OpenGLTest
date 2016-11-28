#include "Font.h"

#define ADVANCE_SCALE 64
#define VECTOR3F_TO_STRING_BUFFER_SIZE 100
#define FLOAT_TO_STRING_BUFFER_SIZE 100

Font::Font(const char * fontPath, int size, FT_Library ft)
{
	if (FT_New_Face(ft, fontPath, 0, &face))
	{
		std::cout << "ERROR: Font " << fontPath << " could not be loaded" << std::endl;
		return;
	}

	FT_Set_Pixel_Sizes(face, 0, size);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (unsigned char c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "WARNING: Char " << c << " (" << (int)c << ") could not be loaded" << std::endl;
			continue;
		}

		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = {
			texture,
			glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		characters[c] = character;
	}
	FT_Done_Face(face);

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Font::~Font()
{
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void Font::renderFloat(GLuint program, float value, float x, float y, float scale, glm::vec4 color, glm::mat4 proj)
{
	char str[FLOAT_TO_STRING_BUFFER_SIZE] = { 0 };
	snprintf(str, FLOAT_TO_STRING_BUFFER_SIZE, "%f", value);
	renderText(program, str, x, y, scale, color, proj);
}

void Font::renderVec3(GLuint program, glm::vec3 value, float x, float y, float scale, glm::vec4 color, glm::mat4 proj)
{
	char str[VECTOR3F_TO_STRING_BUFFER_SIZE] = { 0 };
	snprintf(str, VECTOR3F_TO_STRING_BUFFER_SIZE, "%f, %f, %f", value.x, value.y, value.z);
	renderText(program, str, x, y, scale, color, proj);
}


void Font::renderText(GLuint program, const char * text, float x, float y, float scale, glm::vec4 color, glm::mat4 proj)
{
	glUseProgram(program);

	glUniform4f(glGetUniformLocation(program, "textColor"), color.x, color.y, color.z, color.w);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, &proj[0][0]);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vao);

	const char * c;
	for (c = text; *c; c++)
	{
		Character ch = characters[*c];

		float xpos = x + ch.bearing.x * scale;
		float ypos = y - (ch.size.y - ch.bearing.y) * scale;

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;

		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h, 0.0f, 0.0f },
			{ xpos,     ypos,     0.0f, 1.0f },
			{ xpos + w, ypos,     1.0f, 1.0f },

			{ xpos,     ypos + h, 0.0f, 0.0f },
			{ xpos + w, ypos,     1.0f, 1.0f },
			{ xpos + w, ypos + h, 1.0f, 0.0f }
		};
		glBindTexture(GL_TEXTURE_2D, ch.textureId);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch.advance >> 6) * scale;

	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}
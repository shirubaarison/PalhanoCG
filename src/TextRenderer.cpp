#include "TextRenderer.hpp"
#include "freetype/freetype.h"
#include "Shader.hpp"

TextRenderer::TextRenderer(const std::string& fontPath)
{
  if (!init(fontPath, 48)) {
    std::cerr << "Erro ao iniciar a classe TextRenderer :(" << std::endl;
    return;
  }

  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  std::cout << "[TextRenderer] carregado com fonte: " << fontPath << std::endl;
}

bool TextRenderer::init(const std::string& fontPath, unsigned int fontSize)
{
  FT_Library ft;

  if (FT_Init_FreeType(&ft)) {
    std::cerr << "ERROR::FREETYPE: nao foi possível iniciar a biblioteca FreeType" << std::endl;
    return false;
  }

  FT_Face face;
  if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
    std::cerr << "ERROR::FREETYPE: Falha ao carregar fonte" << std::endl;
    return false;
  }
  
  FT_Set_Pixel_Sizes(face, 0, fontSize);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // desligar restrinção de alinhamento de byte
  
  for (unsigned char c = 0; c < 128; c++) {
    // carrega 
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      std::cerr << "ERROR::FREETYPE: Falha ao carregar glyph" << std::endl;
      continue;
    }
  
    // gerar textura
    unsigned int texture;
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

    // opções 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // guardar 
    Character character = {
      texture,
      glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
      glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
      face->glyph->advance.x
    };

    Characters.insert(std::pair<char, Character>(c, character));
  }

  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  return true;
}

void TextRenderer::renderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color)
{
  shader.activate();
  
  glDisable(GL_DEPTH_TEST);
  glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(vao);

  // passar por todos os caracteres 
  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++) {
    Character ch = Characters[*c];

    float xpos = x + ch.Bearing.x * scale;
    float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

    float w = ch.Size.x * scale;
    float h = ch.Size.y * scale;

    // atualizar vbo para cada caractere
    float vertices[6][4] = {
      { xpos,     ypos + h,   0.0f, 0.0f },            
      { xpos,     ypos,       0.0f, 1.0f },
      { xpos + w, ypos,       1.0f, 1.0f },

      { xpos,     ypos + h,   0.0f, 0.0f },
      { xpos + w, ypos,       1.0f, 1.0f },
      { xpos + w, ypos + h,   1.0f, 0.0f }    
    };

    // renderizar 
    glBindTexture(GL_TEXTURE_2D, ch.textureID);

    // atualizar conteudo da memoria do vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // renderizar quad 
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // va para o prox
    x += (ch.Advance >> 6) * scale;
  }
  glBindVertexArray(0);
  glBindTexture(GL_TEXTURE_2D, 0);

  glEnable(GL_DEPTH_TEST);
}

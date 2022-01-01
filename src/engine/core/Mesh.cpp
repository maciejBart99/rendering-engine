#include "Mesh.h"

#include <utility>

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, Material material)
{
  this->indices = indices;
  this->material = std::move(material);

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
  glBindVertexArray(0);
}

void Mesh::render(SimpleShader& shader)
{
  for(unsigned int i = 0; i < material.textures.size(); i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);

    Texture texture = material.textures[i];

    glUniform1i(glGetUniformLocation(shader.getOpenglId(), texture.type.c_str()), i);
    glBindTexture(GL_TEXTURE_2D, texture.id);
  }

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  glActiveTexture(GL_TEXTURE0);
}
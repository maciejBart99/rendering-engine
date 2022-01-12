#include "Mesh.h"

#include <iostream>
#include <utility>

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, string name, Material material)
{
  this->indices = indices;
  this->name = name;
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
  bool hasDiffuse = false;
  bool hasSpecular = false;
  bool hasNormal = false;

  for(unsigned int i = 0; i < material.textures.size(); i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);

    Texture texture = material.textures[i];

    if (texture.type == "texture_diffuse") {
      shader.set("material.kdMap", (int) i);
      hasDiffuse = true;
    } else if (texture.type == "texture_specular") {
      shader.set("material.ksMap", (int) i);
      hasSpecular = true;
    } else if (texture.type == "texture_normal") {
      shader.set("material.nMap", (int) i);
      hasNormal = true;
    }

    glBindTexture(GL_TEXTURE_2D, texture.id);
  }

  if (useIndividualTransformation) {
    shader.set("iModel", individualTransformation);
  }
  shader.set("useIModel", (int)useIndividualTransformation);
  shader.set("material.Ks", material.ks);
  shader.set("material.Kd", material.kd);
  shader.set("material.Ka", material.ka);
  shader.set("material.shiness", material.shiness);
  shader.set("material.hasNMap", (int)hasNormal);
  shader.set("material.hasKdMap", (int)hasDiffuse);
  shader.set("material.hasKsMap", (int)hasSpecular);
  shader.set("material.opacity", material.opacity == 0 ? 1 : material.opacity);

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  glActiveTexture(GL_TEXTURE0);
}
void Mesh::setUseIndividualTransformation(bool useIndividualTransformation) {
  this->useIndividualTransformation = useIndividualTransformation;
}
void Mesh::setIndividualTransformation(
    const glm::mat4& individualTransformation) {
  this->individualTransformation = individualTransformation;
}

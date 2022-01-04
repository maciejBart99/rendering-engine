#include "Model.h"

void Model::load(const string& path)
{
  Assimp::Importer importer;

  auto scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

  if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
  {
    cout << "Model load error\n" << importer.GetErrorString() << endl;
    return;
  }

  directory = path.substr(0, path.find_last_of('/'));
  processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
  for(unsigned int i = 0; i < node->mNumMeshes; i++)
  {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(processMesh(mesh, scene));
  }

  for(unsigned int i = 0; i < node->mNumChildren; i++)
  {
    processNode(node->mChildren[i], scene);
  }
}
vector<Texture> Model::loadMaterial(aiMaterial* mat, aiTextureType type, const string& typeName)
{
  vector<Texture> textures;

  for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
  {
    aiString str;
    mat->GetTexture(type, i, &str);
    bool skip = false;
    for(auto & j : texturesCache)
    {
      if(std::strcmp(j.path.data(), str.C_Str()) == 0)
      {
        textures.push_back(j);
        skip = true;
        break;
      }
    }
    if(!skip)
    {
      Texture texture;
      string path = this->directory + "/" + string(str.C_Str());
      texture.id = loadTexture(path.c_str());
      texture.type = typeName;
      texture.path = str.C_Str();
      textures.push_back(texture);
      texturesCache.push_back(texture);
    }
  }
  return textures;
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
  {
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
      Vertex vertex;
      glm::vec3 vector;

      vector.x = mesh->mVertices[i].x;
      vector.y = mesh->mVertices[i].y;
      vector.z = mesh->mVertices[i].z;
      vertex.Position = vector;

      if (mesh->HasNormals())
      {
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;
      }

      if(mesh->mTextureCoords[0])
      {
        glm::vec2 vec;
        vec.x = mesh->mTextureCoords[0][i].x;
        vec.y = mesh->mTextureCoords[0][i].y;
        vertex.TexCoords = vec;
        vector.x = mesh->mTangents[i].x;
        vector.y = mesh->mTangents[i].y;
        vector.z = mesh->mTangents[i].z;
        vertex.Tangent = vector;
      }
      else
        vertex.TexCoords = glm::vec2(0.0f, 0.0f);

      vertices.push_back(vertex);
    }

    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
      aiFace face = mesh->mFaces[i];
      for(unsigned int j = 0; j < face.mNumIndices; j++)
        indices.push_back(face.mIndices[j]);
    }
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    vector<Texture> diffuseMaps = loadMaterial(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    vector<Texture> specularMaps = loadMaterial(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    std::vector<Texture> normalMaps = loadMaterial(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    aiColor3D diffuseColor;
    material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);

    aiColor3D specularColor;
    material->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);

    aiColor3D ambientColor;
    material->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor);

    float shiness;
    material->Get(AI_MATKEY_SHININESS, shiness);

    float opacity;
    material->Get(AI_MATKEY_OPACITY, opacity);

    Material m;
    m.opacity = opacity;
    m.ks = glm::vec3(specularColor.r, specularColor.g, specularColor.b);
    m.kd = glm::vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b);
    m.shiness = (int)shiness;
    m.ka = glm::vec3(ambientColor.r, ambientColor.g, ambientColor.b);
    m.textures = textures;

    return {vertices, indices, m};
  }
}

GLuint loadTexture(const char *path)
{
  GLuint textureID;
  glGenTextures(1, &textureID);

  int width, height, channels;
  unsigned char *data = stbi_load(path, &width, &height, &channels, 0);

  GLenum format;

  switch (channels)
  {
    case 1: format = GL_RED;
      break;
    case 3: format = GL_RGB;
      break;
    case 4: format = GL_RGBA;
      break;
    default:
      throw invalid_argument("Channels number invalid");
  }

  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_image_free(data);

  return textureID;
}

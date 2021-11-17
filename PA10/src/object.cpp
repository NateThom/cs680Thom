#include "object.h"
#include "glm/gtx/string_cast.hpp"

// Object::Object()
// {
//   Vertices = {
//     {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}},
//     {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
//     {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
//     {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
//     {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
//     {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}},
//     {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
//     {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}}
//   };

//   Indices = {
//     2, 3, 4,
//     8, 7, 6,
//     1, 5, 6,
//     2, 6, 7,
//     7, 8, 4,
//     1, 4, 8,
//     1, 2, 4,
//     5, 8, 6,
//     2, 1, 6,
//     3, 2, 7,
//     3, 7, 4,
//     5, 1, 8
//   };

//   // The index works at a 0th index
//   for(unsigned int i = 0; i < Indices.size(); i++)
//   {
//     Indices[i] = Indices[i] - 1;
//   }

//   degree = 0.0f;
//   angle = 0.0f;

//   glGenBuffers(1, &VB);
//   glBindBuffer(GL_ARRAY_BUFFER, VB);
//   glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

//   glGenBuffers(1, &IB);
//   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
//   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
// }

Object::Object(std::string o_path, std::string f_path)
{
  loadMaterialFile(f_path);

  loadTexture();

  if(!loadOBJ(Vertices, Indices, o_path))
  {
    printf("Failed to load OBJ file\n");
  }

  degree = 0.0f;
  angle = 0.0f;
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::Update(glm::mat4 position)
{
  // degree += dt * M_PI/10000;
  // angle += dt * M_PI/5000;

  // origin_model = glm::translate(glm::mat4(1.0f), glm::vec3(cos(degree)*8.0f, 0.0f, sin(degree)*8.0f));
  // origin_model = glm::scale(origin_model, glm::vec3(1.0f, 1.0f, 1.0f));

  model = position;
  // model = glm::rotate(model, (angle), glm::vec3(0.0, 1.0, 0.0));
}

glm::mat4 Object::GetModel()
{
  return model;
}

glm::mat4 Object::GetOriginModel()
{
  return origin_model;
}

void Object::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,normal));
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texture));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  for(int i = 0; i < num_meshes; i++){
    glBindTexture(GL_TEXTURE_2D, textures[i]);
    glDrawElements(GL_TRIANGLES, indice_size[i], GL_UNSIGNED_INT, reinterpret_cast<void*>(sizeof(unsigned int)*indice_size[i-1]));
  }

  //glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}

bool Object::loadOBJ(std::vector<Vertex> &out_vertices, std::vector<unsigned int> &out_indices, std::string o_path)
{
  Assimp::Importer importer;
  const aiScene *myScene = importer.ReadFile(o_path, aiProcess_Triangulate);

  int numMeshes = myScene->mNumMeshes;
  num_meshes = numMeshes;

  std::vector<aiMesh*> meshes;
  for(int i = 0; i < numMeshes; i++){
    meshes.push_back(myScene->mMeshes[i]);
  }

  unsigned int* mIndices = &out_indices[0];

  unsigned int index;
  unsigned int offset = 0;

//  std::cout << meshes.size() << std::endl;

  for(int i = 0; i < meshes.size(); i++){
    indice_size.push_back(meshes[i]->mNumFaces * 3);
//    std::cout << Materials[i].newmtl << std::endl;
    for(int j = 0; j < meshes[i]->mNumFaces; j++){
      const aiFace& thisFace = meshes[i]->mFaces[j];

      for(int k = 0; k < 3; k++){
        index = thisFace.mIndices[k] + offset;
        out_indices.push_back(index);

        aiVector3D aiVector = meshes[i]->mVertices[thisFace.mIndices[k]];
        aiVector3D aiNormal = meshes[i]->mNormals[thisFace.mIndices[k]];
        aiVector3D aiUV;
        if(meshes[i]->mTextureCoords[0] == NULL){
          aiUV = {0.0, 0.0, 1.0};
        }
        else{
          aiUV = meshes[i]->mTextureCoords[0][thisFace.mIndices[k]];
        }

        if(Materials[i].map_Kd == ""){
          aiUV.z = 1.0;
        }
        else{
          aiUV.z = 0.0;
        }

        Vertex *temp = new Vertex(glm::vec3(aiVector.x, aiVector.y, aiVector.z), 
                                  glm::vec3(aiNormal.x, aiNormal.y, aiNormal.z), 
                                  glm::vec3(aiUV.x, aiUV.y, aiUV.z));
        out_vertices.push_back(*temp);
      }
    }
    offset += (meshes[i]->mNumFaces * 3);
  }
  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

	return true;
}

bool Object::loadMaterialFile(std::string f_path)
{
  std::string str = "";
  std::ifstream m_file(f_path);
  if(!m_file.is_open()){
    printf("Could not open material file\n");
    return false;
  }

  Material n_material;
  int count = 0;
  int index = 0;

  while(getline(m_file, str)){
    if(str.substr(0,10) == "# Material"){
      std::vector<std::string> line = split(str);
      count = std::stoi(line[3]);
    }
    if(str.substr(0,6) == "newmtl"){
      if(n_material.newmtl.size() != 0){
        Materials.push_back(n_material);
      }
      std::vector<std::string> line = split(str);
      n_material.newmtl = line[1];
    }
    if(str.substr(0,2) == "Ns"){
      std::vector<std::string> line = split(str);
      n_material.Ns = std::stof(line[1]);
    }
    if(str.substr(0,2) == "Ka"){
      std::vector<std::string> line = split(str);
      n_material.Ka = glm::vec3(std::stof(line[1]), std::stof(line[2]), std::stof(line[3]));
    }
    if(str.substr(0,2) == "Kd"){
      std::vector<std::string> line = split(str);
      n_material.Kd = glm::vec3(std::stof(line[1]), std::stof(line[2]), std::stof(line[3]));
    }
    if(str.substr(0,2) == "Ks"){
      std::vector<std::string> line = split(str);
      n_material.Ks = glm::vec3(std::stof(line[1]), std::stof(line[2]), std::stof(line[3]));
    }
    if(str.substr(0,2) == "Ni"){
      std::vector<std::string> line = split(str);
      n_material.Ni = std::stof(line[1]);
    }
    if(str.substr(0,1) == "d"){
      std::vector<std::string> line = split(str);
      n_material.d = std::stof(line[1]);
    }
    if(str.substr(0,5) == "illum"){
      std::vector<std::string> line = split(str);
      n_material.illum = std::stoi(line[1]);
    }
    if(str.substr(0,6) == "map_Kd"){
      std::vector<std::string> line = split(str);
      n_material.map_Kd = line[1];
    }
  }
  Materials.push_back(n_material);

  return true;
}

std::vector<std::string> Object::split(std::string str)
{
	std::vector<std::string> words{};
	std::string word = " ";
	for(char s : str){
		if(s == ' '){
			words.push_back(word);
			word = "";
		}
		else{
			word = word + s;
		}
	}
	words.push_back(word);
	return words;
}

Material Object::GetMaterial()
{
  return Materials[0];
}

bool Object::loadTexture()
{
  int count = Materials.size();

  glGenTextures(count, textures);

  for(int i = 0; i < count; i++){
    if(Materials[i].map_Kd != ""){
      Magick::Image* image = new Magick::Image("textures/" + Materials[i].map_Kd);
      Magick::Blob m_blob;
      image->write(&m_blob,"RGBA");

      glBindTexture(GL_TEXTURE_2D, textures[i]);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->columns(), image->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_blob.data());

      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      delete image;
    }
  }

  return true;
}
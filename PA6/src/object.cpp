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
  loadTexture("Hello");

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

void Object::UpdateFromOrigin(unsigned int dt, float f_scale, float o_speed, float s_speed)
{
  degree += o_speed * dt * M_PI/10000;
  angle += s_speed * dt * M_PI/5000;

  origin_model = glm::translate(glm::mat4(1.0f), glm::vec3(cos(degree)*8.0f, 0.0f, sin(degree)*8.0f));
  origin_model = glm::scale(origin_model, glm::vec3(1.0f, 1.0f, 1.0f));

  model = glm::translate(glm::mat4(1.0f), glm::vec3(cos(degree)*8.0f, 0.0f, sin(degree)*8.0f));
  model = glm::rotate(model, (angle), glm::vec3(0.0, 1.0, 0.0));

  model = glm::scale(model, glm::vec3(f_scale, f_scale, f_scale));
}

void Object::UpdateFromModel(unsigned int dt, glm::mat4 planet_model, float f_scale, float o_speed, float s_speed)
{
  degree += o_speed * dt * M_PI/5000;
  angle += s_speed * dt * M_PI/2000;

  model = glm::translate(planet_model, glm::vec3(cos(degree)*5.0f, 0.0f, sin(degree)*5.0f));
  model = glm::rotate(model, (angle), glm::vec3(0.0, 1.0, 0.0));

  model = glm::scale(model, glm::vec3(f_scale, f_scale, f_scale));
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
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,texture));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  for(int i = 0; i < num_meshes; i++){
    glBindTexture(GL_TEXTURE_2D, textures[i]);
    glDrawElements(GL_TRIANGLES, indice_size[i], GL_UNSIGNED_INT, reinterpret_cast<void*>(sizeof(unsigned int)*indice_size[i-1]));
  }

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);
}

float Object::getOrbitSpeed(unsigned int dt, float o_speed, float rotational_period)
{
  return o_speed * dt * rotational_period;
}

float Object::getSpinSpeed(unsigned int dt, float s_speed, float rotational_period)
{
  return s_speed * dt * rotational_period;
}

bool Object::loadOBJ(std::vector<Vertex> &out_vertices, std::vector<unsigned int> &out_indices, std::string o_path)
{
  glm::vec3 color = {0.2, 0.2, 0.2};

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

  for(int i = 0; i < meshes.size(); i++){
    indice_size.push_back(meshes[i]->mNumFaces * 3);
    for(int j = 0; j < meshes[i]->mNumFaces; j++){
      const aiFace& thisFace = meshes[i]->mFaces[j];

      for(int k = 0; k < 3; k++){
        index = thisFace.mIndices[k] + offset;
        out_indices.push_back(index);

        aiVector3D aiVector = meshes[i]->mVertices[thisFace.mIndices[k]];
        aiVector3D aiUV = meshes[i]->mTextureCoords[0][thisFace.mIndices[k]];

        Vertex *temp = new Vertex(glm::vec3(aiVector.x, aiVector.y, aiVector.z), color, glm::vec2(aiUV.x, aiUV.y));
        out_vertices.push_back(*temp);
      }
    }
    offset += (meshes[i]->mNumFaces * 3);
  }
  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * out_vertices.size(), &out_vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * out_indices.size(), &out_indices[0], GL_STATIC_DRAW);

	return true;
}

bool Object::loadMaterialFile(std::string f_path)
{
  glm::vec3 color = {0.5, 0.5, 0.5};
  std::vector <std::string> temp_str;
  std::vector <std::string> material;
  std::vector <glm::vec3> material_color;

	std::string str = "";
  std::ifstream m_file(f_path);
  if(!m_file.is_open()){
    printf("Could not open material file\n");
    return false;
  }
  while(getline(m_file, str)){
    if(str.substr(0,6) == "newmtl"){
      temp_str = split(str);
      material.push_back(temp_str[1]);
    }
    if(str.substr(0,2) == "Kd"){
      temp_str = split(str);
      color = {std::stof(temp_str.at(1)), std::stof(temp_str.at(2)), std::stof(temp_str.at(3))};
      material_color.push_back(color);
    }
  }
  if(material_color.size() == 0)
    material_color.push_back(color);

  return true;
}

bool Object::loadTexture(std::string texture_file)
{
  std::string texture_file_array[] = {"textures/granite.jpg", "textures/checker.jpg"};

  //initalize image loading with magick++
  Magick::Image* image1 = new Magick::Image(texture_file_array[0]);
  Magick::Blob m_blob1;
  image1->write(&m_blob1,"RGBA");

  Magick::Image* image2 = new Magick::Image(texture_file_array[1]);
  Magick::Blob m_blob2;
  image2->write(&m_blob2,"RGBA");

  glGenTextures(2, textures);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image1->columns(), image1->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_blob1.data());

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  //initialze texture 1
  glBindTexture(GL_TEXTURE_2D, textures[1]);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image2->columns(), image2->rows(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_blob2.data());

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  delete image1;
  delete image2;

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


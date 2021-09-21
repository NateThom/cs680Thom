#include "object.h"

Object::Object()
{  
  /*
    # Blender File for a Cube
    o Cube
    v 1.000000 -1.000000 -1.000000
    v 1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 1.000000
    v -1.000000 -1.000000 -1.000000
    v 1.000000 1.000000 -0.999999
    v 0.999999 1.000000 1.000001
    v -1.000000 1.000000 1.000000
    v -1.000000 1.000000 -1.000000
    s off
    f 2 3 4
    f 8 7 6
    f 1 5 6
    f 2 6 7
    f 7 8 4
    f 1 4 8
    f 1 2 4
    f 5 8 6
    f 2 1 6
    f 3 2 7
    f 3 7 4
    f 5 1 8
  */

  Vertices = {
    {{1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 0.0f}},
    {{1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}},
    {{-1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
    {{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}},
    {{1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f}},
    {{1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 1.0f}},
    {{-1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 1.0f}},
    {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}}
  };

  Indices = {
    2, 3, 4,
    8, 7, 6,
    1, 5, 6,
    2, 6, 7,
    7, 8, 4,
    1, 4, 8,
    1, 2, 4,
    5, 8, 6,
    2, 1, 6,
    3, 2, 7,
    3, 7, 4,
    5, 1, 8
  };

  std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
  std::vector< glm::vec3 > temp_vertices;
  std::vector< glm::vec2 > temp_uvs;
  std::vector< glm::vec3 > temp_normals;
  ReadObjFile("../models/ball_pit.obj", vertexIndices, uvIndices, normalIndices, temp_vertices,
              temp_uvs, temp_normals);

  unsigned int vertexIndex;
  glm::vec3 vertex;
  for( unsigned int i=0; i<vertexIndices.size(); i++ ) {
    vertexIndex = vertexIndices[i];
    vertex = temp_vertices[ vertexIndex-1 ];
  }
  // The index works at a 0th index
  for(unsigned int i = 0; i < Indices.size(); i++)
  {
    Indices[i] = Indices[i] - 1;
  }

  angle_rotate = 0.0f;
  angle_translate = 0.0f;

  glGenBuffers(1, &VB);
  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

  glGenBuffers(1, &IB);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
}

Object::~Object()
{
  Vertices.clear();
  Indices.clear();
}

void Object::UpdateFromOrigin(float rotation_speed, float translation_speed, float scale)
{
    angle_rotate += rotation_speed * M_PI/1000;

    angle_translate += translation_speed * M_PI/1000;

    model_translation = glm::translate(glm::mat4(1.0f), glm::vec3(cos(angle_translate) * 5, 0, sin(angle_translate) * 5));

    model = glm::translate(glm::mat4(1.0f), glm::vec3(cos(angle_translate) * 5, 0, sin(angle_translate) * 5));
    model = glm::rotate(model, (angle_rotate), glm::vec3(0.0, 1.0, 0.0));
    model = glm::scale(model, glm::vec3(scale));
}

void Object::UpdateFromModel(glm::mat4 other_model, float rotation_speed, float translation_speed, float scale)
{
    angle_rotate += rotation_speed * M_PI/1000;

    angle_translate += translation_speed * M_PI/1000;

    model = glm::translate(other_model, glm::vec3(cos(angle_translate) * 5, 0, sin(angle_translate) * 5));
    model = glm::rotate(model, (angle_rotate), glm::vec3(0.0, 1.0, 0.0));
    model = glm::scale(model, glm::vec3( scale));
}

glm::mat4 Object::GetModel()
{
  return model;
}

glm::mat4 Object::GetModelTranslation()
{
  return model_translation;
}

void Object::Render()
{
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, VB);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex,color));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);

  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

bool Object::ReadObjFile(const std::string& objectFilePath, std::vector<unsigned int>& vertexIndices,
                         std::vector<unsigned int>& uvIndices,  std::vector<unsigned int>& normalIndices,
                         std::vector<glm::vec3>& temp_vertices, std::vector<glm::vec2>& temp_uvs,
                         std::vector<glm::vec3>& temp_normals){

  std::ifstream f(objectFilePath);

  bool return_flag = false;

  if (f.is_open()){
    std::string temp_str;
    std::string space_delimeter = " ";
    std::string temp_str2;
    size_t pos;

    while(getline(f, temp_str)) {
      std::cout << "";

      if(temp_str.c_str()[0] == 'v' && temp_str.c_str()[1] == ' '){
        glm::vec3 vertex;
        for(int i=0; i<4; i++){
          pos = temp_str.find(space_delimeter);
          temp_str2 = temp_str.substr(0, pos);

          if(i == 1){
            vertex.x = atof(temp_str2.c_str());
          }
          else if(i == 2){
            vertex.y = atof(temp_str2.c_str());
          }
          else if(i == 3){
            vertex.z = atof(temp_str2.c_str());
          }

          temp_str.erase(0, pos + space_delimeter.length());
        }
        temp_vertices.push_back(vertex);
      }

      else if(temp_str.c_str()[0] == 'v' && temp_str.c_str()[1] == 't') {
        glm::vec2 uv;
        for (int i = 0; i < 3; i++) {
          pos = temp_str.find(space_delimeter);
          temp_str2 = temp_str.substr(0, pos);

          if (i == 1) {
            uv.x = atof(temp_str2.c_str());
          } else if (i == 2) {
            uv.y = atof(temp_str2.c_str());
          }

          temp_str.erase(0, pos + space_delimeter.length());
        }
        temp_uvs.push_back(uv);
      }

      else if(temp_str.c_str()[0] == 'v' && temp_str.c_str()[1] == 'n') {
        glm::vec3 normal;
        for (int i = 0; i < 4; i++) {
          pos = temp_str.find(space_delimeter);
          temp_str2 = temp_str.substr(0, pos);

          if (i == 1) {
            normal.x = atof(temp_str2.c_str());
          }
          else if (i == 2) {
            normal.y = atof(temp_str2.c_str());
          }
          else if (i == 3) {
            normal.z = atof(temp_str2.c_str());
          }

          temp_str.erase(0, pos + space_delimeter.length());
        }
        temp_normals.push_back(normal);
      }

      else if(temp_str.c_str()[0] == 'f' && temp_str.c_str()[1] == ' ') {
        glm::vec3 normal;
        for(int i=0; i<4; i++) {
          pos = temp_str.find(space_delimeter);
          temp_str2 = temp_str.substr(0, pos);

          if (i == 1) {
            std::cout << (temp_str2.c_str()[0] - '0') << std::endl;
            vertexIndices.push_back((temp_str2.c_str()[0] - '0'));
            vertexIndices.push_back((temp_str2.c_str()[2] - '0'));
            vertexIndices.push_back((temp_str2.c_str()[4] - '0'));
          } else if (i == 2) {
            uvIndices.push_back((temp_str2.c_str()[0] - '0'));
            uvIndices.push_back((temp_str2.c_str()[2] - '0'));
            uvIndices.push_back((temp_str2.c_str()[4] - '0'));
          } else if (i == 3) {
            normalIndices.push_back((temp_str2.c_str()[0] - '0'));
            normalIndices.push_back((temp_str2.c_str()[2] - '0'));
            normalIndices.push_back((temp_str2.c_str()[4] - '0'));
          }

          temp_str.erase(0, pos + space_delimeter.length());
        }
      }
    }
  }
  f.close();
  return_flag = true;
}

#pragma once
#include <iostream>
#include "shader.h"
#include "Mesh.h"
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image.h"

using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory);



class Model {
public:
	Model(string &path) {
		loadModel(path);
	}
	
	void Draw(Shader &shader) {
		for (int i = 0; i < meshes.size(); i++) {
			meshes[i].Draw(shader);
		}
	}
	
private:
	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded; //could use a map instead of texture

	void loadModel(string &path) {
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
			return;
		}

		directory = path.substr(0, path.find_last_of('/'));
		processNode(scene->mRootNode, scene);
	}
	
	void processNode(aiNode *node, const aiScene *scene){
		for (int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		for (int i = 0; i < node->mNumChildren; i++) {
			processNode(node->mChildren[i], scene);
		}
	}

	Mesh processMesh(aiMesh* mesh, const aiScene* scene) {
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;

		for (int i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;
			
			vertex.Position = glm::vec3(mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z);

			vertex.Normal = glm::vec3(mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z);

			if (mesh->mTextureCoords[0]) { //check if the mesh contains texture coords
				vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x,
					mesh->mTextureCoords[0][i].y);//meshes may be have multiple texture coords, we'll pick only the first one
			}
			else {
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);
			}

			vertices.push_back(vertex);
		}

		for (int i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		if (mesh->mMaterialIndex >= 0) {
			aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];

			vector<Texture> diffuseMaps = loadMaterialTextures(mat, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

			vector<Texture> specularMaps = loadMaterialTextures(mat, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		return Mesh(vertices, indices, textures);
	}

	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName) {
		vector<Texture> textures;
		for (int i = 0; i < mat->GetTextureCount(type); i++) {
			aiString str;
			mat->GetTexture(type, i, &str);

			bool skip = false;
			for (int j = 0; j < textures_loaded.size(); j++) {
				if (strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
					textures.push_back(textures_loaded[j]);
					skip = true;
					break;
				}
			}
			if (!skip) {
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), directory);
				texture.type = typeName;
				texture.path = str.C_Str();
				textures.push_back(texture);
				textures_loaded.push_back(texture);
			}
		}

		return textures;
	}
};

unsigned int TextureFromFile(const char* path, const string &directory) {
	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureId;
	glGenTextures(1, &textureId);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	//cout << path << endl;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		auto format = GL_RGBA;

		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureId);

		//setting the wrapping and filtering options of the texture object
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture1\n";
	}
	stbi_image_free(data);

	return textureId;
}
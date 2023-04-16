#pragma once
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "rendering/OpenGl/OpenGL_Mesh.h"

namespace LG
{
    class Shader;
    class Texture;
    class Mesh;

    class Model
    {
    public:
        explicit Model(std::string path)
        {
            LoadModel(std::move(path));
        }

        void Draw(Shader& shader);

    private:
        void LoadModel(std::string path);
        void ProcessNode(const aiNode* node, const aiScene* scene);
        Mesh ProcessMesh(const aiMesh* mesh, const aiScene* scene);
        std::vector<std::shared_ptr<Texture>> LoadMaterialTextures(const aiMaterial* mat, aiTextureType type, std::string typeName);
 
    private:
        // model data
        std::vector<std::shared_ptr<Texture>> m_loadedTextures;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
        std::vector<Mesh> m_meshes;
        std::string m_directory;
    };
}

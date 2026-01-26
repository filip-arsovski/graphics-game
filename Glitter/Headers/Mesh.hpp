#pragma once

// System Headers
#include <assimp/importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

// Standard Headers
#include <map>
#include <memory>
#include <vector>

#include "Shader.hpp"
#include "structs.hpp"


// Define Namespace
namespace Mirage
{
    // Vertex Format
    class Mesh
    {
    public:

        // Implement Default Constructor and Destructor
         Mesh() { glGenVertexArrays(1, & mVertexArray); }
        ~Mesh() { glDeleteVertexArrays(1, & mVertexArray); }

        // Implement Custom Constructors
        Mesh(std::string const & folder, std::string const & filename);
        Mesh(std::vector<Vertex> const & vertices,
             std::vector<GLuint> const & indices,
             std::map<GLuint, std::string> const & textures);

    	Mesh(std::vector<glm::vec2> const & vertices);
    	Mesh(std::vector<glm::vec3> const & vertices);

        Mesh(std::vector<Vertex> const & vertices, std::vector<unsigned int> const & indices);

        // Public Member Functions
        void draw(GLuint shader);

        // Disable Copying and Assignment
        Mesh(Mesh const &) = delete;
        Mesh & operator=(Mesh const &) = delete;

        void parse(aiNode const * node, aiScene const * scene);
    	void parse(aiMesh const * mesh, aiScene const * scene);
        std::map<GLuint, std::string> process(aiMaterial * material,
                                              aiTextureType type);

    	void Move(glm::vec3 currentPosition, glm::vec3 previousPosition);

    	std::string folder;
    	std::string filename;
        std::vector<std::unique_ptr<Mesh>> mSubMeshes;
        std::vector<GLuint> mIndices;
        std::vector<Vertex> mVertices;
        std::map<GLuint, std::string> mTextures;

        GLuint mVertexArray;
        GLuint mVertexBuffer;
        GLuint mElementBuffer;

    };
};

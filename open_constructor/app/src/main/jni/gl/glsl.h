#ifndef GL_GLSL_H
#define GL_GLSL_H

#include <string>
#include <vector>
#include "utils/math.h"

namespace oc {
    class GLSL {
    public:
        /**
         * @brief Constructor
         * @param vert is vertex shader code
         * @param frag is fragment shader code
         */
        GLSL(std::string vert, std::string frag);

        ~GLSL();

        /**
         * @brief it sends geometry into GPU
         * @param vertices is vertices
         * @param normals is normals
         * @param coords is texture coords
         * @param colors is vertex colors
         */
        void Attrib(glm::vec3* vertices, glm::vec3* normals, glm::vec2* coords, unsigned int* colors);

        /**
         * @brief it binds shader
         */
        void Bind();

        static GLSL* CurrentShader();

        /**
         * @brief initShader creates shader from code
         * @param vs is vertex shader code
         * @param fs is fragment shader code
         * @return shader program id
         */
        unsigned int InitShader(const char *vs, const char *fs);

        /**
         * @brief it unbinds shader
         */
        void Unbind();

        /**
         * @brief UniformFloat send float into shader
         * @param name is uniform name
         * @param value is uniform value
         */
        void UniformFloat(const char* name, float value);

        /**
         * @brief UniformMatrix send matrix into shader
         * @param name is uniform name
         * @param value is uniform value
         */
        void UniformMatrix(const char* name, const float* value);

    private:
        unsigned int id;          ///< Shader id
        unsigned int shader_vp;   ///< Vertex shader
        unsigned int shader_fp;   ///< Fragment shader
        int attribute_v_vertex;   ///< Pointer to vertices on GPU
        int attribute_v_coord;    ///< Pointer to coords on GPU
        int attribute_v_normal;   ///< Pointer to normals on GPU
        int attribute_v_color;    ///< Pointer to colors on GPU
    };
}
#endif

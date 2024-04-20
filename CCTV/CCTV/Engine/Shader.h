#pragma once

#include <map>
#include <vector>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"

namespace Engine
{
	static std::map< std::string, GLenum > __extensionConvertion = {
		{"_v", GL_VERTEX_SHADER},
		{"_f", GL_FRAGMENT_SHADER},
		{"_c", GL_COMPUTE_SHADER}
	};

	struct ShaderProgram
	{
		std::string program_path;
		unsigned int program_type;
	};

	class __declspec(dllexport) Shader
	{
	public:
		void Use();
		void Compile();
		void Compute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
		void RegisterProgram(std::string fullpath, int shadertype);
		void Copy(Shader* shader);

		void SetName(std::string name);
		std::string GetName();
		unsigned int GetID();

		unsigned int UniformGetLocation(const char* name);
		void UniformSetImage(unsigned location, int id);
		void UniformSetImage(const char* name, int id);
		void UniformSetVec2(unsigned location, glm::vec2 input);
		void UniformSetVec3(unsigned location, glm::vec3 input);
		void UniformSetVec4(unsigned location, glm::vec4 input);
		void UniformSetFloat(unsigned location, float input);
		void UniformSetBool(unsigned location, bool input);

		std::vector<ShaderProgram> programs;
	private:
		std::string name;
		unsigned int id;
		bool isInit = false;

		static std::string ProcessCode(std::string a);
		static std::string IncludeLib(std::string path);
	};

	struct GlobalStorage
	{
	public:
		std::string name;
		unsigned int buffer;
		unsigned int location;
	};

	class __declspec(dllexport) ShaderManager
	{
	public:
		static void Initialize();
		static void OnExit();

		static void Refresh();
		static Shader* GetShader(std::string path);
		static Shader* CopyShader(std::string path);

		static bool DefinitionExist(std::string def);
		static void SetDefinition(std::string def, bool o);
		static void GetShaders(void (*ptr)(Shader*));

		static void CreateGlobalStorage(std::string name, unsigned int buffer, unsigned int size, unsigned int location);
		static void GiveAllStorageToShader(Shader* shader);
		static void GiveStorageToShader(GlobalStorage storage, Shader* shader);

	private:
		static std::vector<Shader*> shader_copies;
		static std::map<std::string, Shader> shaders;
		static std::vector< GlobalStorage> global_storage;
		static std::map<std::string, bool> pp_definitions;
	};
}
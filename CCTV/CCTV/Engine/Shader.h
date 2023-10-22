#pragma once

#include <map>
#include <vector>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

		void UniformSetImage(unsigned location, int id);
		void UniformSetImage(const char* name, int id);

		std::vector<ShaderProgram> programs;
	private:
		std::string name;
		unsigned int id;
		bool isInit = false;

		static std::string ProcessCode(std::string a);
		static std::string IncludeLib(std::string path);
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

	private:
		static std::vector<Shader*> shader_copies;
		static std::map<std::string, Shader> shaders;
		static std::map<std::string, bool> pp_definitions;
	};
}
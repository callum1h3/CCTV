#include "Shader.h"
#include "OSHandler.h"
#include "Debug.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>

using namespace Engine;

std::vector<Shader*> ShaderManager::shader_copies;
std::map<std::string, Shader> ShaderManager::shaders;
std::map<std::string, bool> ShaderManager::pp_definitions;

void ShaderManager::Initialize()
{
    Refresh();
}

void ShaderManager::OnExit()
{
    shaders.clear();
}

void ShaderManager::Refresh()
{
    Debug::Log("Refreshing shaders!");
    std::string shader_path = OSHandler::GetPath() + "/shaders/";
    std::filesystem::create_directory(shader_path);

    for (const auto& entry : std::filesystem::directory_iterator(shader_path))
    {
        if (entry.path().extension().string() != ".glsl")
            continue;

        std::string filename = entry.path().stem().string();
        int fileNameLength = filename.length();
        int fileNameMinus = fileNameLength - 2;

        std::string shader_type = filename.substr(fileNameMinus, fileNameLength);
        filename.erase(fileNameMinus, fileNameLength);

        Shader* shader = GetShader(filename);
        shader->SetName(filename);
        shader->RegisterProgram(entry.path().string(), __extensionConvertion[shader_type]);
    }

    for (std::map<std::string, Shader>::iterator it = shaders.begin(); it != shaders.end(); ++it)
    {
        Shader* shader = &it->second;
        shader->Compile();
    }
}

void ShaderManager::GetShaders(void (*ptr)(Shader*))
{
    for (std::map<std::string, Shader>::iterator it = shaders.begin(); it != shaders.end(); ++it)
    {
        Shader* shader = &it->second;
        ptr(shader);
    }
}

bool ShaderManager::DefinitionExist(std::string def)
{
    return pp_definitions[def];
}

void ShaderManager::SetDefinition(std::string def, bool o)
{
    pp_definitions[def] = o;
}

Shader* ShaderManager::GetShader(std::string path)
{
    return &shaders[path];
}

Shader* ShaderManager::CopyShader(std::string path)
{
    Shader* shader = GetShader(path);
    Shader* copy = new Shader();
    copy->Copy(shader);
    copy->Compile();

    shader_copies.push_back(copy);
    return copy;
}

void Shader::Copy(Shader* shader)
{
    name = shader->GetName() + " (copy)";
    programs = shader->programs;
}

void Shader::RegisterProgram(std::string path, int shadertype)
{
    for (ShaderProgram i : programs)
    {
        if (i.program_path == path)
            return;
    }

    ShaderProgram program;
    program.program_path = path;
    program.program_type = shadertype;

    programs.push_back(program);
}

const std::vector<std::string> explode(const std::string& s, const char& c)
{
    std::string buff{ "" };
    std::vector<std::string> v;

    for (auto n : s)
    {
        if (n != c) buff += n; else
            if (n == c && buff != "") { v.push_back(buff); buff = ""; }
    }
    if (buff != "") v.push_back(buff);

    return v;
}

std::string Shader::IncludeLib(std::string path)
{
    std::string final_str = "";
    std::string lib_path = OSHandler::GetPath() + "/shaders/libs/" + path + "/";
    std::filesystem::create_directory(lib_path);

    for (const auto& entry : std::filesystem::directory_iterator(lib_path))
    {
        if (entry.path().extension().string() != ".glsl")
            continue;

        std::ifstream fileStream(entry.path().string());
        if (!fileStream.good())
            continue;

        std::string code = std::string(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>());
        final_str.append("\n" + code + "\n");
    }

    return final_str;
}

std::string Shader::ProcessCode(std::string a)
{
    std::string final_string;
    std::istringstream f(a);
    std::string line;
    bool is_appending = true;
    int depth = 0;

    while (std::getline(f, line))
    {
        std::vector<std::string> v{ explode(line, ' ') };

        if (v.size() < 1)
        {
            if (is_appending)
                final_string.append(line + "\n");
            continue;
        }

        std::string current_line = line;
        std::string p = v[0];

        if (p == "##include")
        {
            if (is_appending)
                current_line = ProcessCode(IncludeLib(v[1]));
            else
                current_line = "";
        }
        else if (p == "##ifdef")
        {
            if (is_appending)
            {
                if (!ShaderManager::DefinitionExist(v[1]))
                {
                    depth = 0;
                    is_appending = false;
                }
            }

            depth += 1;
            current_line = "";
        }
        else if (p == "##endif")
        {

            depth -= 1;
            if (depth == 0)
                is_appending = true;
            current_line = "";
        }

        if (is_appending)
            final_string.append(current_line + "\n");
    }

    return final_string;
}

void Shader::Compile()
{
    std::vector<unsigned int> compiled_programs;

    for (ShaderProgram i : programs)
    {
        std::ifstream fileStream(i.program_path);
        if (!fileStream.good())
            continue;

        std::string programCode = ProcessCode(std::string(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>()));
        const char* c_str = programCode.c_str();

        unsigned shader_id = glCreateShader(i.program_type);
        glShaderSource(shader_id, 1, &c_str, NULL);
        glCompileShader(shader_id);

        int  success;
        char infoLog[512];
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(shader_id, 512, NULL, infoLog);
            Debug::Log("Shader Program failed to compile! [" + i.program_path + "] " + std::string(infoLog));
            Debug::Log(programCode);
            return;
        }

        compiled_programs.push_back(shader_id);
    }

    if (isInit)
        glDeleteProgram(id);
    id = glCreateProgram();
    isInit = true;

    for (unsigned int i : compiled_programs)
        glAttachShader(id, i);

    glLinkProgram(id);

    for (unsigned int i : compiled_programs)
        glDeleteShader(i);
    compiled_programs.clear();

    int success;
    char infoLog[512];
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        Debug::Log("Shader linking error! [" + GetName() + "] " + infoLog);
        return;
    }

    Debug::Log("Successfully Compiled Shader Program: " + GetName() + "!");
}

void Shader::Use()
{
    glUseProgram(id);
}

void Shader::Compute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z)
{
    Use();
    glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
}

void Shader::UniformSetImage(unsigned location, int id)
{
    Use();
    glUniform1i(location, id);
}

void Shader::UniformSetImage(const char* name, int id)
{
    UniformSetImage(glGetUniformLocation(this->id, name), id);
}


void Shader::SetName(std::string name)
{
    this->name = name;
}

std::string Shader::GetName()
{
    return name;
}

unsigned int Shader::GetID()
{
    return id;
}
#include <engine/resource/Shader.hpp>
#include <engine/Core.hpp>

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem.hpp>
#include <regex>

using namespace boost::filesystem;

Shader::Shader() : Handle()
{
	Handle::_ids[0] = glCreateProgram();
}

Shader::~Shader()
{
	glDeleteProgram(Handle::_ids[0]);
}

namespace internal
{
	static inline std::string trim(std::string const& s)
	{
		auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c)
		{
			return isspace(c);
		});

		auto wsback = std::find_if_not(s.rbegin(), s.rend(), [](int c)
		{
			return isspace(c);
		}).base();

		return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
	}

	static std::vector<std::string> load_file(std::string const& name)
	{
		auto p = Core::instance().settings().shader_path() / (name + ".glsl");
		if(!exists(p)) throw std::invalid_argument("Shader '" + p.string() + "' does not exist");
		ifstream file(p);
		std::vector<std::string> ret;

		for (std::string line; std::getline(file, line);)
			ret.push_back(line);
		return ret;
	}

	static void parse(std::vector<std::string>& lines)
	{
		bool finished = false;
		while (!finished)
		{
			std::size_t i;
			for (i = 0; i < lines.size(); i++)
			{
				std::string line(lines[i]);

				std::regex import("#pragma +import +(.*) *", std::regex_constants::extended);
				std::smatch match;
				if (regex_match(line, match, import))
				{
					if (match.size() == 2)
					{
						lines.erase(lines.begin() + i);
						auto file = load_file(trim(match[1].str().c_str()));
						lines.insert(lines.begin() + i, file.begin(), file.end());
						break;
					}
				}
			}

			if (i >= lines.size()) finished = true;
		}
	}

	static GLuint compile_shader(GLenum type, std::string const& source)
	{
		GLuint s = glCreateShader(type);
		const char* data = source.data();
		glShaderSource(s, 1, &data, 0);
		glCompileShader(s);
		int compile_ok = 0;
		glGetShaderiv(s, GL_COMPILE_STATUS, &compile_ok);
		if (!compile_ok)
		{
			int length = 0;
			glGetShaderiv(s, GL_INFO_LOG_LENGTH, &length);
			char* log_ = new char[length];
			glGetShaderInfoLog(s, length, 0, log_);

			std::string log(log_);
			delete[] log_;
			throw std::runtime_error("Shader compilation error :\n" + log);
		}

		return s;
	}
}

void compile_shader(Shader& shader, std::string const& name)
{
	auto lines = internal::load_file(name);
	internal::parse(lines);

	std::string file;
	for (auto l : lines) file += l + "\n";

	GLuint vs = internal::compile_shader(GL_VERTEX_SHADER, "#version 400 core\n#define VERTEX\n" + file);
	GLuint fs = internal::compile_shader(GL_FRAGMENT_SHADER, "#version 400 core\n#define FRAGMENT\n" + file);

	glAttachShader(shader, vs);
	glDeleteShader(vs);
	glAttachShader(shader, fs);
	glDeleteShader(fs);
}

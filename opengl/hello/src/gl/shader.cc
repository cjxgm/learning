#include "shader.hh"
#include "../log.hh"

namespace gl
{
	shader::shader(shader::kind skind)
	{
		sh = {
			new raw_shader_type(glCreateShader(GLenum(skind))),
			[](raw_shader_ptr sh) {
				library::log() << "shader::delete: " << *sh << "\n";
				glDeleteShader(*sh);
				delete sh;
			}
		};
		if (!*sh) throw std::runtime_error{"cannot create shader"};
		library::log() << "shader::create: " << *sh << "\n";
	}

	void shader::compile(source_cref src) const
	{
		auto data = src.data();
		auto size = GLint(src.size());
		glShaderSource(get(), 1, &data, &size);
		glCompileShader(get());
		check_error();
	}

	void shader::check_error() const
	{
		GLint success;
		glGetShaderiv(get(), GL_COMPILE_STATUS, &success);

		if (!success) {
			GLint len;
			glGetShaderiv(get(), GL_INFO_LOG_LENGTH, &len);
			if (!len) throw compilation_failure{"unknown reason"};

			std::string msg(len+1, 0);
			glGetShaderInfoLog(get(), msg.size(), nullptr, &msg[0]);
			throw compilation_failure{msg};
		}
	}
};


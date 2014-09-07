#include "program.hh"

namespace gl
{
	program::program()
	{
		prog = {
			new raw_program_type(glCreateProgram()),
			[](raw_program_ptr prog) {
				glDeleteProgram(*prog);
				delete prog;
			}
		};
		if (!*prog) throw std::runtime_error{"cannot create program"};
	}

	void program::use() const
	{
		glUseProgram(get());
	}

	void program::attach(shader_cref sh)
	{
		glAttachShader(get(), sh);
		attached_shaders.emplace_back(
				new shader_type{sh},
				[prog=get()](shader_ptr s) {
					glDetachShader(prog, s->get());
					delete s;
				});
	}

	auto program::location(attribute_cref attr) const -> location_type
	{
		return glGetAttribLocation(get(), attr.c_str());
	}

	void program::link()
	{
		glLinkProgram(get());
		check_error();
		glValidateProgram(get());
		check_error();
		attached_shaders.clear();
	}

	void program::check_error() const
	{
		GLint success;
		glGetProgramiv(get(), GL_LINK_STATUS, &success);

		if (!success) {
			GLint len;
			glGetProgramiv(get(), GL_INFO_LOG_LENGTH, &len);
			if (!len) throw linking_failure{"bad program: unknown reason."};

			std::string msg(len+1, 0);
			glGetProgramInfoLog(get(), msg.size(), nullptr, &msg[0]);
			throw linking_failure{msg};
		}
	}
};


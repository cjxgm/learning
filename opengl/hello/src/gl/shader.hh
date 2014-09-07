#pragma once
#include <GL/glew.h>
#include <string>
#include <memory>
#include <stdexcept>

namespace gl
{
	struct shader
	{
		enum class kind
		{
			vertex = GL_VERTEX_SHADER,
			fragment = GL_FRAGMENT_SHADER,
		};

		using raw_shader_type = GLuint;
		using raw_shader_ptr  = raw_shader_type *;
		using raw_shader_sptr = std::shared_ptr<raw_shader_type>;
		using source_type = std::string;
		using source_cref = source_type const&;

		struct compilation_failure : std::runtime_error
		{
			template <class T>
			compilation_failure(T what) : runtime_error{what} {}
		};

		shader(shader::kind skind);
		shader(shader::kind skind, source_cref src) : shader(skind) { compile(src); }
		void compile(source_cref src) const;

		raw_shader_type get() const { return *sh.get(); }
		operator raw_shader_type() const { return get(); }

	private:
		raw_shader_sptr sh;

		void check_error() const;
	};
};


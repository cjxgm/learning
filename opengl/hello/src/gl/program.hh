#pragma once
#include "shader.hh"
#include <GL/glew.h>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

namespace gl
{
	struct program
	{
		using raw_program_type = GLuint;
		using raw_program_ptr  = raw_program_type *;
		using raw_program_sptr = std::shared_ptr<raw_program_type>;
		using shader_type = gl::shader;
		using shader_ptr  = shader_type *;
		using shader_cref = shader_type const&;
		using shader_sptr = std::shared_ptr<shader_type>;
		using attribute_type = std::string;
		using attribute_cref = attribute_type const&;
		using location_type = GLuint;
		using attached_shader_list = std::vector<shader_sptr>;

		struct linking_failure : std::runtime_error
		{
			template <class T>
			linking_failure(T what) : runtime_error{what} {}
		};

		program();
		void attach(shader_cref sh);
		void attribute_location(attribute_cref attr, location_type loc) const;
		void link();

		void use() const;
		void operator()() const { use(); }

		raw_program_type get() const { return *prog.get(); }
		operator raw_program_type() const { return get(); }

	private:
		raw_program_sptr prog;
		attached_shader_list attached_shaders;

		void check_error() const;
	};
};


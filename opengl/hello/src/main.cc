#include "log.hh"
#include "gl/context.hh"
#include "gl/shader.hh"
#include "gl/program.hh"
#include <GL/glew.h>

int main()
{
	library::log() << "main(): enter\n";

	gl::context ctx;

	gl::shader vs{gl::shader::kind::vertex,
		"#version 120\n"
		"attribute vec3 position;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(position, 1);\n"
		"}\n"
	};

	gl::shader fs{gl::shader::kind::fragment,
		"#version 120\n"
		"void main()\n"
		"{\n"
		"	gl_FragColor = vec4(0.6, 1, 0.2, 1);\n"
		"}\n"
	};

	gl::program prog;
	prog.attach(vs);
	prog.attach(fs);
	prog.attribute_location("position", 0);
	prog.link();
	prog.use();

	while (ctx) {
		ctx.clear({1.0, 0.6, 0.2, 1.0});
		ctx.update();
	}

	library::log() << "main(): leave\n";
}


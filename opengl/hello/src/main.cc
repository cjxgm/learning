#include "log.hh"
#include "gl/context.hh"
#include "gl/shader.hh"
#include "gl/program.hh"
#include <GL/glew.h>

#define GLSL(src) "#version 330 core\n" #src

int main()
{
	library::log() << "main(): enter\n";

	gl::context ctx;

	gl::shader vs{gl::shader::kind::vertex, GLSL(
		in vec3 position;

		void main()
		{
			gl_Position = vec4(position, 1);
		}
	)};

	gl::shader fs{gl::shader::kind::fragment, GLSL(
		out vec4 color;

		void main()
		{
			color = vec4(0.6, 1, 0.2, 1);
		}
	)};

	gl::program prog;
	prog.attach(vs);
	prog.attach(fs);
	glBindFragDataLocation(prog, 0, "color");
	prog.link();
	prog.use();

	library::log() << "\tcreating vertex array object\n";
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	library::log() << "\tcreating vertex buffer object\n";
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	library::log() << "\tbinding vertex buffer data\n";
	float verts[] = {
		 0  ,  0.5, 0,
		 0.5, -0.5, 0,
		-0.5, -0.5, 0,
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	auto pos = prog.location("position");
	library::log() << "main(): id[position] = " << pos << "\n";

	glEnableVertexAttribArray(pos);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(pos, 3, GL_FLOAT, false, 0, nullptr);

	library::log() << "main(): mainloop\n";
	while (ctx) {
		ctx.clear({1.0, 0.6, 0.2, 1.0});
		glDrawArrays(GL_TRIANGLES, 0, 3);
		ctx.update();
	}

	library::log() << "main(): leave\n";
}


#include "log.hh"
#include "gl/context.hh"
#include <GL/glew.h>

int main()
{
	library::log() << "main(): enter\n";
	gl::context ctx;
	while (ctx) {
		ctx.clear({1.0, 0.6, 0.2, 1.0});
		ctx.update();
	}
	library::log() << "main(): leave\n";
}


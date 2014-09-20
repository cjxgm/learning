#include "context.hh"
#include "compiler.hh"

#include <iostream>
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

int main()
{
	imgui::context ctx{640, 480};
	imgui::state s;
	imgui::compiler cpr;

	bool a;
	while (ctx) {
		cout << "render" << endl;
		cpr.rect(0, 0, 640, 480, 0, 0, 0, 255);
		if (a) cpr.rect(10, 10, 100, 10, 255, 255, 255, 50);
		cpr.text(10, 30, 100, 40, "hello\nworld 123 yes or no?", 2);
		a = !a;
		cpr.compile();
		auto cmds = cpr.get();
		ctx.render(cmds);
		ctx.update(s);
	}
}


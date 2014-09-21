#include "context.hh"
#include "compiler.hh"
#include "log.hh"

int main()
{
	library::log() << "main(): enter\n";

	imgui::context ctx{640, 480};
	imgui::state s;
	imgui::compiler cpr;
	cpr.size(640, 480);

	bool a = true;
	int size = 1;
	while (ctx) {
		library::log() << "main(): render\n";

		cpr.rect(0, 0, 640, 480, 255, 0, 0, 255);

		std::string text{"hello\ta\tab\tabc\tabcd\tabcde\nworld 123 yes or no?\n\t"};
		text += std::to_string(size);
		text += "\t";
		text += std::to_string(a);
		{
			imgui::clip_guard _(cpr, 100, 100, 400, 200);
			cpr.rect(100, 100, 400, 200, 0, 0, 200);
			cpr.rect(s.mouse.x+10, s.mouse.y+10, 200, 80, 255, 255, 100);
			cpr.text(s.mouse.x+10, s.mouse.y+10, 200, 80, text, size);
		}
		if (a) cpr.rect(10, 10, 100, 10, 255, 255, 255, 255);
		a = !a;

		if (size++ > 64) size = 1;

		cpr.compile();
		auto cmds = cpr.get();

		ctx.render(cmds);
		ctx.update(s);
	}

	library::log() << "main(): leave\n";
}


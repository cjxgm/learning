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

	using imgui::xywh;
	using imgui::xyxy;
	using imgui::rgba;

	bool a = true;
	int size = 1;
	while (ctx) {
		library::log() << "main(): render\n";

		cpr.rect(xywh{0, 0, 640, 480}, rgba{255, 0, 0});

		std::string text{"hello\ta\tab\tabc\tabcd\tabcde\nworld 123 yes or no?\n\t"};
		text += std::to_string(size);
		text += "\t";
		text += std::to_string(a);
		{
			imgui::clip_guard guard(cpr, xywh{100, 100, 400, 200});
			if (guard) {
				cpr.rect(xywh{100, 100, 400, 200}, rgba{0, 0, 255, 200});
				for (int y=0; y<480; y+=20)
					for (int x=0; x<640; x+=20)
						cpr.text(xywh{x, y, 20, 20}, "hi");
				cpr.rect(xywh{s.mouse.x+10, s.mouse.y+10, 20, 20}, rgba{255, 255, 100, 150});
				//cpr.rect(xywh{s.mouse.x+10, s.mouse.y+10, 200, 80}, rgba{255, 255, 100});
				//cpr.text(xywh{s.mouse.x+10, s.mouse.y+10, 200, 80}, text, {0}, cpr.make_skip_and_size(size));
			}
		}
		if (a) cpr.rect(xywh{10, 10, 100, 10}, rgba{255, 200});
		a = !a;

		if (size++ > 64) size = 1;

		cpr.compile();
		auto cmds = cpr.get();

		ctx.render(cmds);
		ctx.update(s);
	}

	library::log() << "main(): leave\n";
}


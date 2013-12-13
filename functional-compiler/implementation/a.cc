#include <iostream>
#include <functional>
#include <memory>
using namespace std;

namespace parser
{
	using Node = function<bool()>;
	auto& input = cin;

	Node ch(char c)
	{
		return [=]() {
			char in;
			if (!input.get(in)) return true;
			if (in != c) {
				cout << ">> " << c << " but " << in << endl;
				return false;
			}
			input.putback(in);
			return true;
		};
	}

	Node success()
	{
		return [](){ return false; };
	}

	Node seq(Node a, Node b)
	{
		return [=](){ return (a() || b()); };
	}

	Node br(Node a, Node b)
	{
		return [=](){ return (a() && b()); };
	}


	/* FIXME: wait gcc to fix the bug that variadic not usable in lambda.
	template <class F, class... Ts>
	Node lazy(F f, Ts... args) { return [=](){ return f(args...)(); }; }
	// */
	//* work around of the gcc bug
	template <class F>
	Node lazy(F f) { return [=]() { return f()(); }; }
	template <class F, class A>
	Node lazy(F f, A a) { return [=]() { return f(a)(); }; }
	template <class F, class A, class B>
	Node lazy(F f, A a, B b) { return [=]() { return f(a, b)(); }; }
	// */

	Node always(Node a)
	{
		return seq(a, lazy(always, a));
	}

	Node until(char c)
	{
		return br(always(ch(c)), success());
	}
};

namespace p = parser;

int main()
{
	auto parser = p::until('t');
	cout << parser() << endl;
}


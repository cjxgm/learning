..	vim: noet ts=4 sw=4 sts=0
	dependencies: python-docutils python-pygments graphviz perl

=====================================
Write a Compiler Simply and Elegantly
=====================================
---------------------
with Lambdas in C++11
---------------------
:Author: eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>

.. contents:: Table of Contents
.. sectnum::

Introduction
============
As we all know, writting a compiler by hand is challenging, but this
article is aiming at making the process simple and elegant. You will
be amazed about the simplicity when the functional ideas like lambdas
and closures are introduced.

Yes, we only borrow some ideas (closures and lambda functions) from
functional programming, not everything, such as we won't make things
immutable.

Blah blah blah...

Concepts
========

Everything is a Tree
--------------------
... In fact, a binary tree. For example, the regular expression
:code:`\.[a-z]+.*x$` can be expressed as:

..	digraph::
	sequence_4 -> sequence_3
	sequence_4 -> EOL
	sequence_3 -> sequence_2
	sequence_3 -> x
	sequence_2 -> sequence_1
	sequence_2 -> repeat_2
	sequence_1 -> dot
	sequence_1 -> repeat_1
	repeat_1 -> a_to_z
	repeat_2 -> any
	sequence_4 [label="sequence"]
	sequence_3 [label="sequence"]
	sequence_2 [label="sequence"]
	sequence_1 [label="sequence"]
	dot [label="'.'"]
	repeat_1 [label="repeat [1, +oo)"]
	a_to_z [label="['a', 'z']"]
	repeat_2 [label="repeat [0, +oo)"]
	x [label="'x'"]


Building Blocks
---------------

Conventions
~~~~~~~~~~~

A node named by a capital letter with orange color represent any legal
nodes.

..	digraph::
	X [color=orange]

It can also be written in text as :code:`A`.


Atomic Nodes
~~~~~~~~~~~~

:code:`success` node (|as| :code:`success`) does *nothing*
but returns *success*:

..	digraph::
	success [color=green]

:code:`failure` node (|as| :code:`failure`) does *nothing*
but returns *failure*:

..	digraph::
	failure [color=red]

:code:`sequence` node (|as| :code:`A+B`) runs its left node.
If succeeds, it runs its right node.
Succeed |iff| *both* of its child nodes *succeed*:

..	digraph::
	sequence [color=magenta]
	A [color=orange]
	B [color=orange]
	sequence -> A
	sequence -> B

:code:`branch` node (|as| :code:`A|B`) runs both of its left
and right node.
Succeed |iff| *at least 1* of its child nodes *succeed*:

..	digraph::
	branch [color=blue]
	A [color=orange]
	B [color=orange]
	branch -> A
	branch -> B

:code:`match` node (|as| :code:`'x'`, or |as| ascii :code:`120`)
matches one character 'x' (ascii 120).
Succeed |iff| matching succeed.

..	digraph::
	x [color=brown label="'x'"]


Repeat Nodes
~~~~~~~~~~~~

With atomic nodes, we can build more complex nodes based on them.
Now, let's make some nodes that do repeating things.

:code:`whether` node (|as| :code:`!A`) *repeat* its child
*1 or 0* time.
Succeed *without* caring if its child succeed just like :code:`?` in
regular expressions.

..	sidebar:: Note

	In the figure, the *left* side is the *representation*,
	while the *right* side is the *implementation*.

	**The same convention is used through out the article.**

	We can also write it as :code:`!A = A|success`

..	digraph::
	whether -> A1
	branch -> A2
	branch -> success
	A1 [color=orange label=A]
	A2 [color=orange label=A]
	branch [color=blue]
	success [color=green]


:code:`times` node (|as| :code:`A*n`, where :math:`n \ge 2`) *repeat*
its child *n* times.
Succeed |iff| its child succeed *n* times.

..	sidebar:: Note

	We used *closed-interval* :math:`[n, n]` in the graph.

	It's a recursive definition. We can also write it as
	:code:`A*n = A*(n-1)+A`

..	digraph::
	times -> A1
	sequence -> A2
	sequence -> times_1 -> A3
	A1 [color=orange label=A]
	A2 [color=orange label=A]
	A3 [color=orange label=A]
	times [label="[n, n]"]
	times_1 [label="[n-1, n-1]"]
	sequence [color=magenta]

..	sidebar:: Note

	This is the recursion base. We can also write it as:
	:code:`A*2 = A+A`

..	digraph::
	times -> A1
	sequence -> A2
	sequence -> A3
	A1 [color=orange label=A]
	A2 [color=orange label=A]
	A3 [color=orange label=A]
	times [label="[2, 2]"]
	sequence [color=magenta]

:code:`more` node (|as| :code:`A*1`) *repeat*
its child *at least 1* time.
Succeed |iff| its child *succeed sequentially at least 1* time.

..	sidebar:: Note

	We used *closed-open-interval* :math:`[1, +\infty)` in the graph.

	It's a recursive definition. We can also write it as
	:code:`A*1 = A+!(A*1)`

..	digraph::
	more -> A1
	sequence -> A2
	sequence -> whether -> more_1 -> A3
	A1 [color=orange label=A]
	A2 [color=orange label=A]
	A3 [color=orange label=A]
	more [label="[1, +oo)"]
	more_1 [label="[1, +oo]"]
	sequence [color=magenta]

:code:`whatever` node (|as| :code:`A*0`) *repeat*
its child until first failure.
Succeed anyway.

..	sidebar:: Note

	We used *closed-open-interval* :math:`[0, +\infty)` and
	:math:`[1, +\infty)` in the graph.

	It's a recursive definition. We can also write it as
	:code:`A*0 = !(A*1)`

..	digraph::
	whatever -> A1
	whether -> more -> A2
	A1 [color=orange label=A]
	A2 [color=orange label=A]
	whatever [label="[0, +oo)"]
	more [label="[1, +oo)"]




..	......................................................................

..	some replacements
..	|as| replace:: written as
..	|iff| replace:: *only if*

..	stylish the document
..	raw:: html

	<style>
	.section { margin: 0.5em; }
	.line-block { margin: 1em; }
	p { margin: 1em; }
	.digraph {
		display: block;
		margin-left: auto;
		margin-right: auto;
	}
	</style>


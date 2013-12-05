..	vim: noet ts=4 sw=4 sts=0
	dependencies: python-docutils python-pygments graphviz perl

=====================================
Write a Compiler Simply and Elegantly
=====================================
---------------------------
the Functional Way in C++11
---------------------------
:Author: eXerigumo Clanjor (哆啦比猫/兰威举) <cjxgm@126.com>

.. contents:: Table of Contents
.. sectnum::

Introduction
============
As we all know, writting a compiler by hand is challenging, but this
article is aiming at making the process simple and elegant. You will
be amazed about the simplicity when the functional ideas are introduced.

Yes, we only borrow some ideas (closures and lambda functions) from
functional programming, but not everything. We will not make things
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

A node named a capital letter with orange color represent any node.

..	digraph::
	X [color=orange]


Nodes that do Nothing
~~~~~~~~~~~~~~~~~~~~~

:code:`success` node does nothing but return success:

..	digraph::
	success [color=green]

:code:`failure` node does nothing but return failure:

..	digraph::
	failure [color=red]


Nodes that Connect Other Nodes
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

:code:`sequence` node runs its left node, if succeed run its right node.
Succeed only if both of its child nodes succeed:

..	digraph::
	sequence [color=magenta]
	A [color=orange]
	B [color=orange]
	sequence -> A
	sequence -> B

:code:`branch` node runs both of its left and right node.
Succeed only if at least one of its child nodes succeed:

..	digraph::
	branch [color=blue]

This is how it can be used:

..	digraph:: one-or-zero-time
	branch -> A
	branch -> success

..	digraph::
	yes[label="yes!"]

..	raw:: html

	<style>
	.section { margin-left: 0.5em; }
	.line-block { margin-left: 1em; }
	p { margin-left: 1em; }
	.digraph {
		display: block;
		margin-left: auto;
		margin-right: auto;
	}
	</style>


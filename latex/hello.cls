% vim: ft=tex
\LoadClass{article}
\usepackage{xcolor}
\usepackage{fontspec}
\usepackage{titlesec}
\setmainfont{Cantarell}

\newcommand \subject[1] {
	\begin{center}
		\bfseries #1
	\end{center}
}

\newcommand \wholeline[1] {
	\parbox{\dimexpr \textwidth-2 \fboxsep \relax}{#1}
}

\newcommand \heading[1] {
	\vskip 0.2em
	\colorbox{black!90}{
		\color{black!10}
		\wholeline{#1}
	}
	\vskip 0.2em
}

\titleformat{\section} {
}{
	%\color{red!50!black!80}\thesection.
}{1em}{}


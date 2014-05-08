// vim: noet ts=4 sw=4 sts=0
#pragma once
#include <string>
#include <iostream>
using namespace std;

class Var
{
public:
	Var();
	Var(const Var&);
	Var(const Var&&);
	Var(int);
	Var(bool);
	Var(double);
	Var(string&);
	Var(string&&);
	Var(const char *);
	~Var();

	static const Var null;

	bool is_void  () const noexcept;
	bool is_int   () const noexcept;
	bool is_bool  () const noexcept;
	bool is_double() const noexcept;
	bool is_string() const noexcept;

	operator int   () const noexcept;
	operator bool  () const noexcept;
	operator double() const noexcept;
	operator string() const noexcept;

	Var clone() const;

private:
	class Type;			friend class Type;
	class TypeVoid;		friend class TypeVoid;
	class TypeInt;		friend class TypeInt;
	class TypeBool;		friend class TypeBool;
	class TypeDouble;	friend class TypeDouble;
	class TypeString;	friend class TypeString;

	union Value
	{
		int     _int;
		bool    _bool;
		double  _double;
		string* _string;
	};

	const Type* type;
	Value value;
};

inline std::ostream& operator<<(std::ostream& o, Var& var)
{
	return o << std::string(var);
}


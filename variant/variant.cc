// vim: noet ts=4 sw=4 sts=0
#include "variant.hh"
using namespace std;

///////////////////////////////////////////////////////////////////////////
class Var::Type
{
public:
	Type() {}
	virtual ~Type() {}

	virtual int    to_int   (const Value&) const noexcept { return 0; }
	virtual bool   to_bool  (const Value&) const noexcept { return false; }
	virtual double to_double(const Value&) const noexcept { return 0.0; }
	virtual string to_string(const Value&) const noexcept { return string{}; }

	virtual bool is_void  () const noexcept { return false; }
	virtual bool is_int   () const noexcept { return false; }
	virtual bool is_bool  () const noexcept { return false; }
	virtual bool is_double() const noexcept { return false; }
	virtual bool is_string() const noexcept { return false; }
private:
};

///////////////////////////////////////////////////////////////////////////
class Var::TypeVoid : public Var::Type
{
public:
	static const TypeVoid instance;

	bool is_void() const noexcept { return true; }
	string to_string(const Value& value) const noexcept { return "void"; }
};

///////////////////////////////////////////////////////////////////////////
class Var::TypeInt : public Var::Type
{
public:
	static const TypeInt instance;

	bool is_int() const noexcept { return true; }
	int    to_int   (const Value& value) const noexcept { return value._int; }
	bool   to_bool  (const Value& value) const noexcept { return value._int != 0; }
	double to_double(const Value& value) const noexcept { return value._int; }
	string to_string(const Value& value) const noexcept { return std::to_string(value._int); }
};

///////////////////////////////////////////////////////////////////////////
class Var::TypeBool : public Var::Type
{
public:
	static const TypeBool instance;

	bool is_bool() const noexcept { return true; }
	int    to_int   (const Value& value) const noexcept { return value._bool; }
	bool   to_bool  (const Value& value) const noexcept { return value._bool; }
	double to_double(const Value& value) const noexcept { return value._bool; }
	string to_string(const Value& value) const noexcept { return (value._bool ? "true" : "false"); }
};

///////////////////////////////////////////////////////////////////////////
class Var::TypeDouble : public Var::Type
{
public:
	static const TypeDouble instance;

	bool is_double() const noexcept { return true; }
	int    to_int   (const Value& value) const noexcept { return value._double; }
	bool   to_bool  (const Value& value) const noexcept { return value._double != 0.0; }
	double to_double(const Value& value) const noexcept { return value._double; }
	string to_string(const Value& value) const noexcept { return std::to_string(value._double); }
};

///////////////////////////////////////////////////////////////////////////
class Var::TypeString : public Var::Type
{
public:
	static const TypeString instance;

	bool is_string() const noexcept { return true; }
	int    to_int   (const Value& value) const noexcept { return stoi(*value._string); }
	bool   to_bool  (const Value& value) const noexcept { return value._string->empty(); }
	double to_double(const Value& value) const noexcept { return stod(*value._string); }
	string to_string(const Value& value) const noexcept { return *value._string; }
};

///////////////////////////////////////////////////////////////////////////
const Var::TypeVoid			Var::TypeVoid  ::instance;
const Var::TypeInt			Var::TypeInt   ::instance;
const Var::TypeBool			Var::TypeBool  ::instance;
const Var::TypeDouble		Var::TypeDouble::instance;
const Var::TypeString		Var::TypeString::instance;

///////////////////////////////////////////////////////////////////////////
const Var Var::null;

///////////////////////////////////////////////////////////////////////////
Var::Var(const Var& _var) : type{_var.type}
{
	if (is_int() || is_bool() || is_double()) value = _var.value;
	else if (is_string()) value._string = new string(*_var.value._string);
}
Var::Var(const Var&& _var) : type{_var.type}
{
	if (is_int() || is_bool() || is_double()) value = _var.value;
	else if (is_string()) value._string = new string(std::move(*_var.value._string));
}
Var::Var(                  ) : type{&TypeVoid  ::instance} {}
Var::Var(int         _value) : type{&TypeInt   ::instance} { value._int = _value; }
Var::Var(bool        _value) : type{&TypeBool  ::instance} { value._bool = _value; }
Var::Var(double      _value) : type{&TypeDouble::instance} { value._double = _value; }
Var::Var(string&     _value) : type{&TypeString::instance} { value._string = new string(_value); }
Var::Var(string&&    _value) : type{&TypeString::instance} { value._string = new string(std::move(_value)); }
Var::Var(const char* _value) : type{&TypeString::instance} { value._string = new string(_value); }
Var::~Var() { if (is_string()) delete value._string; }

///////////////////////////////////////////////////////////////////////////
bool Var::is_void  () const noexcept { return type->is_void  (); }
bool Var::is_int   () const noexcept { return type->is_int   (); }
bool Var::is_bool  () const noexcept { return type->is_bool  (); }
bool Var::is_double() const noexcept { return type->is_double(); }
bool Var::is_string() const noexcept { return type->is_string(); }

///////////////////////////////////////////////////////////////////////////
Var::operator int   () const noexcept { return type->to_int   (value); }
Var::operator bool  () const noexcept { return type->to_bool  (value); }
Var::operator double() const noexcept { return type->to_double(value); }
Var::operator string() const noexcept { return type->to_string(value); }

///////////////////////////////////////////////////////////////////////////
Var Var::clone() const { return Var{*this}; }




#ifndef _VECTOR_HPP
#define _VECTOR_HPP


#include <stdexcept>


namespace sam
{

class vector {
public:
	using value_type	= double;
	using size_type		= unsigned;

public:
	vector() : sz_{0}, cap_{0}, elem_{nullptr} {}

	vector(size_type sz)
		:sz_{sz}
		,cap_{sz}
		,elem_{ new value_type[sz]{} } {}

	~vector() {
		delete[] elem_;
	}

	vector(const vector& o)
		:sz_{o.sz_}
		,cap_{o.cap_}
		,elem_{new value_type[sz_]}
	{
		for (size_type i = 0; i < sz_; ++i)
			elem_[i] = o.elem_[i];
	}

	vector& operator = (const vector&);

	vector(vector&& o) :sz_{o.sz_}, cap_{o.cap_}, elem_{o.elem_} {
		o.sz_ = o.cap_ = 0;
		o.elem_ = nullptr;
	}

	vector& operator = (vector&& o);

public:
	value_type& operator[] (size_type i) {
		return elem_[i];
	}

	const value_type& operator[] (size_type i) const {
		return elem_[i];
	}

	value_type& at(size_type i) {
		if (i < 0 || sz_ <= i) throw std::bad_alloc{};
		return elem_[i];
	}

	const value_type& at(size_type i) const {
		if (i < 0 || sz_ <= i) throw std::bad_alloc{};
		return elem_[i];
	}

	size_type size() const { return sz_; }
	size_type capacity() const { return cap_; }

	void reserve(size_type sz);
	void resize(size_type sz);
	void push_back(value_type val);

private:
	size_type		sz_;
	size_type		cap_;
	value_type*		elem_;
};

///

vector& vector::operator = (const vector& o)
{
	if (this == &o) return *this;

	if (o.sz_ <= cap_) {
		for (size_type i = 0; i < o.sz_; ++i)
			elem_[i] = o.elem_[i];

		sz_ = o.sz_;
		return *this;
	}

	value_type* p = new value_type[o.sz_];
	for (size_type i = 0; i < o.sz_; ++i)
		p[i] = o.elem_[i];

	delete[] elem_;
	elem_ = p;

	sz_ = cap_ = o.sz_;
	return *this;
}

///

vector& vector::operator = (vector&& o) {
	sz_ = o.sz_;
	cap_ = o.cap_;

	delete[] elem_;
	elem_ = o.elem_;

	o.sz_ = o.cap_ = 0;
	o.elem_ = nullptr;

	return *this;
}

///

void vector::reserve(size_type cap)
{
	if (cap <= cap_) return;

	value_type* p = new value_type[cap];
	for (size_type i = 0; i < sz_; ++i) p[i] = elem_[i];
	delete[] elem_;

	elem_ = p;
	cap_ = cap;
}

///

//
// cases:
// sz < sz_
// sz = sz_
// sz_ < sz <= cap_
// cap_ < sz
//
// Kudos to Stroustrup for this brilliant code..! It handles all four cases
// in three lines without explicite `if` statements !!
//
void vector::resize(size_type sz)
{
	reserve(sz);
	for (size_type i = sz_; i < sz; ++i) elem_[i] = value_type{};
	sz_ = sz;
}

///

void vector::push_back(value_type value)
{
	if (cap_ == 0)
		reserve(8);			// start with 8 "slots"
	else if (sz_ == cap_)
		reserve(sz_ * 2);

	elem_[sz_++] = value;
}

} // namespace sam


#endif // _VECTOR_HPP


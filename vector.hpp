
#ifndef _VECTOR_HPP
#define _VECTOR_HPP


#include <memory>
#include <stdexcept>


namespace sam
{

template <typename T, typename A = std::allocator<T>>
	//requires Element<T>() && Allocator<A>()
class vector {
public:
	using value_type	= T;
	using allocator_type	= A;
	using size_type		= unsigned;

public:
	vector(const allocator_type& alloc = std::allocator<value_type>{}) :alloc_{alloc}, sz_{0}, cap_{0}, elem_{nullptr} {}

	vector(size_type sz, const value_type& val = value_type{}, const allocator_type& alloc = std::allocator<value_type>{})
		:alloc_{alloc}
		,sz_{sz}
		,cap_{sz_}
		,elem_{ alloc_.allocate(sz_) }
	{
		for (size_type i = 0; i < sz_; ++i)
			alloc_.construct(&elem_[i], val);	// TODO: deprecated in C++17
	}

	~vector() {
		for (size_type i = 0; i < sz_; ++i)
			alloc_.destroy(&elem_[i]);		// TODO:: deprecated in C++17
		alloc_.deallocate(elem_, cap_);
	}

	vector(const vector& o)					// FIXME: not `const vector<T, A>&` ??
		:alloc_{o.alloc_}
		,sz_{o.sz_}
		,cap_{o.cap_}
		,elem_{ alloc_.allocate(sz_) }
	{
		for (size_type i = 0; i < sz_; ++i)
			alloc_.construct(&elem_[i], o.elem_[i]);	// TODO: deprecated in C++17
	}

	vector& operator = (const vector&);				// FIXME: not `const vector<T, A>&` ??

	vector(vector&& o) :alloc_{o.alloc_}, sz_{o.sz_}, cap_{o.cap_}, elem_{o.elem_} {
		// FIXME: not `const vector<T, A>&` ??

		o.sz_ = o.cap_ = 0;
		o.elem_ = nullptr;
	}

	vector& operator = (vector&& o);				// FIXME: not `const vector<T, A>&` ??

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
	void resize(size_type sz, const value_type& val);
	void push_back(value_type val);

private:
	allocator_type		alloc_;
	size_type		sz_;
	size_type		cap_;
	value_type*		elem_;
};

///

template <typename T, typename A>
vector<T, A>& vector<T, A>::operator = (const vector& o)
{
	if (this == &o) return *this;

	if (o.sz_ <= cap_) {
		for (size_type i = 0; i < sz_; ++i)
			alloc_.destroy(&elem_[i]);
		for (size_type i = 0; i < o.sz_; ++i)
			alloc_.construct(&elem_[i], o.elem_[i]);	// XXX: what if alloc_.construct() throws??

		sz_ = o.sz_;
		return *this;
	}

	value_type* p = alloc_.allocate(o.sz_);
	for (size_type i = 0; i < o.sz_; ++i) alloc_.construct(&p[i], o.elem_[i]);

	for (size_type i = 0; i < sz_; ++i) alloc_.destroy(&elem_[i]);
	alloc_.deallocate(elem_, cap_);
	elem_ = p;

	alloc_ = o.alloc_;
	sz_ = cap_ = o.sz_;
	return *this;
}

///

template <typename T, typename A>
vector<T, A>& vector<T, A>::operator = (vector&& o) {
	for (size_type i = 0; i < sz_; ++i)
		alloc_.destroy(&elem_[i]);
	alloc_.deallocate(&elem_, cap_);

	elem_ = o.elem_;
	alloc_ = o.alloc_;
	sz_ = o.sz_;
	cap_ = o.cap_;

	o.sz_ = o.cap_ = 0;
	o.elem_ = nullptr;

	return *this;
}

///

//
// exception unsafe code
// XXX: fix it
//
template <typename T, typename A>
void vector<T, A>::reserve(size_type cap)
{
	if (cap <= cap_) return;

	value_type* p = alloc_.allocate(cap);
	for (size_type i = 0; i < sz_; ++i) alloc_.construct(&p[i], elem_[i]);		// TODO: deprecated in C++17
	for (size_type i = 0; i < sz_; ++i) alloc_.destroy(&elem_[i]);			// TODO: deprecated in C++17
	alloc_.deallocate(elem_, cap_);

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
// in three lines without explicit `if` statements !!
//
// XXX: check for exception safety
//
template <typename T, typename A>
void vector<T, A>::resize(size_type sz, const value_type& val)
{
	reserve(sz);
	for (size_type i = sz_; i < sz; ++i)
		alloc_.construct(&elem_[i], val);		// TODO: deprecated in C++17
	sz_ = sz;
}

///

//
// TODO: where did 8 come from?
//
template <typename T, typename A>
void vector<T, A>::push_back(value_type val)
{
	if (cap_ == 0)
		reserve(8);					// start with 8 "slots"
	else if (sz_ == cap_)
		reserve(sz_ * 2);

	elem_[sz_++] = val;
}

} // namespace sam


#endif // _VECTOR_HPP


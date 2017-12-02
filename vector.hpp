
#ifndef _VECTOR_HPP
#define _VECTOR_HPP


#include <memory>
#include <utility>
#include <stdexcept>

//
// TODO: allocator_type::construct()  deprecated in C++17
// TODO: allocator_type::destroy()  deprecated in C++17
//
// FIXME: for non-default-constructible classes, value argument for resize() is
// needed even for 0 size... Looks bad no? :/
// example: vb.resize(0, box{0});
//

namespace sam
{

template <typename T, typename A>
	//requires Element<T>() && Allocator<A>()
struct vector_base {
	using value_type	= T;
	using allocator_type	= A;
	using size_type		= unsigned;

public:
	explicit vector_base(allocator_type alloc, size_type sz = 0)
		:alloc_{alloc}
		,sz_{sz}
		,cap_{sz}
		,elem_{ (cap_ > 0) ? alloc_.allocate(cap_) : nullptr } {}

	vector_base(allocator_type alloc, size_type sz, size_type cap)
		:alloc_{alloc}
		,sz_{sz}
		,cap_{cap}
		,elem_{ (cap_ > 0) ? alloc_.allocate(cap_) : nullptr } {}

	~vector_base() {
		alloc_.deallocate(elem_, sz_);
	}

public:
	allocator_type		alloc_;
	size_type		sz_;
	size_type		cap_;
	value_type*		elem_;
};

template <typename T, typename A = std::allocator<T>>
	//requires Element<T>() && Allocator<A>()
class vector : private vector_base<T, A> {
public:
	using value_type	= T;
	using allocator_type	= A;
	using size_type		= unsigned;

public:
	vector(const allocator_type& alloc = std::allocator<value_type>{})
		:vector_base<T, A>{alloc, 0} {}

	vector(size_type sz, const value_type& val = value_type{}, const allocator_type& alloc = std::allocator<value_type>{})
		:vector_base<T, A>{alloc, sz}
	{
		for (size_type i = 0; i < this->sz_; ++i)
			this->alloc_.construct(&this->elem_[i], val);
	}

	~vector() {
		for (size_type i = 0; i < this->sz_; ++i)
			this->alloc_.destroy(&this->elem_[i]);
	}

	vector(const vector& o)					// TODO: not `const vector<T, A>&`.. class name insertion?
		:vector_base<T, A>{o.alloc_, o.sz_}
	{
		for (size_type i = 0; i < this->sz_; ++i)
			this->alloc_.construct(&this->elem_[i], o.elem_[i]);
	}

	vector& operator = (const vector&);			// TODO: not `const vector<T, A>&`.. class name insertion?

	vector(vector&& o)
		:vector_base<T, A>{o.alloc_, o.sz_, o.cap_, o.elem_}
	{
		o.sz_ = o.cap_ = 0;
		o.elem_ = nullptr;
	}

	vector& operator = (vector&& o);			// TODO: not `const vector<T, A>&`.. class name insertion?

public:
	value_type& operator[] (size_type i) {
		return this->elem_[i];
	}

	const value_type& operator[] (size_type i) const {
		return this->elem_[i];
	}

	value_type& at(size_type i) {
		if (i < 0 || this->sz_ <= i) throw std::bad_alloc{};
		return this->elem_[i];
	}

	const value_type& at(size_type i) const {
		if (i < 0 || this->sz_ <= i) throw std::bad_alloc{};
		return this->elem_[i];
	}

	size_type size() const { return this->sz_; }
	size_type capacity() const { return this->cap_; }

	void reserve(size_type sz);
	void resize(size_type sz, const value_type& val);
	void push_back(value_type val);
};

///

template <typename T, typename A>
vector<T, A>& vector<T, A>::operator = (const vector& o)
{
	if (this == &o) return *this;

	if (o.sz_ <= this->cap_) {
		for (size_type i = 0; i < this->sz_; ++i)
			this->alloc_.destroy(&this->elem_[i]);
		for (size_type i = 0; i < o.sz_; ++i)
			this->alloc_.construct(&this->elem_[i], o.elem_[i]);	// XXX: what if alloc_.construct() throws??

		this->sz_ = o.sz_;
		return *this;
	}

	vector_base<T, A> vb{ o.alloc_, o.sz_ };
	for (size_type i = 0; i < o.sz_; ++i)
		this->alloc_.construct(&vb.elem_[i], o.elem_[i]);
	for (size_type i = 0; i < this->sz_; ++i)
		this->alloc_.destroy(&this->elem_[i]);

	std::swap<vector_base<T, A>>(*this, vb);
	return *this;
}

///

template <typename T, typename A>
vector<T, A>& vector<T, A>::operator = (vector&& o)
{
	for (size_type i = 0; i < this->sz_; ++i)
		this->alloc_.destroy(&this->elem_[i]);

	vector_base<T, A> vb{o.alloc_, o.sz_, o.cap_, o.elem_};
	std::swap<vector_base<T, A>>(*this, vb);

	o.sz_ = o.cap_ = 0;
	o.elem_ = nullptr;

	return *this;
}

///

template <typename T, typename A>
void vector<T, A>::reserve(size_type cap)
{
	if (cap <= this->cap_) return;

	//vector_base<T, A> vb{this->alloc_, cap};				// XXX FIXME: this will increase sz_ upto cap_
	vector_base<T, A> vb{this->alloc_, this->sz_, cap};

	for (size_type i = 0; i < this->sz_; ++i)
		this->alloc_.construct(&vb.elem_[i], this->elem_[i]);

	for (size_type i = 0; i < this->sz_; ++i)
		this->alloc_.destroy(&this->elem_[i]);

	std::swap<vector_base<T, A>>(*this, vb);
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
template <typename T, typename A>
void vector<T, A>::resize(size_type sz, const value_type& val)
{
	reserve(sz);
	for (size_type i = this->sz_; i < sz; ++i)
		this->alloc_.construct(&this->elem_[i], val);
	for (size_type i = sz; i < this->sz_; ++i)
		this->alloc_.destroy(&this->elem_[i]);
	this->sz_ = sz;
}

///

//
// TODO: where did 8 come from?
//
template <typename T, typename A>
void vector<T, A>::push_back(value_type val)
{
	if (this->cap_ == 0)
		reserve(8);					// start with 8 "slots"
	else if (this->sz_ == this->cap_)
		reserve(this->sz_ * 2);

	this->elem_[this->sz_++] = val;
}

} // namespace sam


#endif // _VECTOR_HPP


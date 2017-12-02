
#ifndef _VECTOR_HPP
#define _VECTOR_HPP


#include <memory>
#include <utility>
#include <stdexcept>


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
		//:alloc_{alloc}, sz_{0}, cap_{0}, elem_{nullptr} {}	// FIXME: delete

	vector(size_type sz, const value_type& val = value_type{}, const allocator_type& alloc = std::allocator<value_type>{})
		:vector_base<T, A>{alloc, sz}
		//:alloc_{alloc}		// FIXME: delete
		//,sz_{sz}
		//,cap_{sz_}
		//,elem_{ alloc_.allocate(sz_) }
	{
		for (size_type i = 0; i < this->sz_; ++i)
			this->alloc_.construct(&this->elem_[i], val);	// TODO: deprecated in C++17
	}

	~vector() {
		for (size_type i = 0; i < this->sz_; ++i)
			this->alloc_.destroy(&this->elem_[i]);		// TODO:: deprecated in C++17
		//alloc_.deallocate(elem_, cap_);		// FIXME: delete
	}

	vector(const vector& o)					// TODO: not `const vector<T, A>&`.. class name insertion?
		:vector_base<T, A>{o.alloc_, o.sz_}
		//,alloc_{o.alloc_}		// FIXME: delete
		//,sz_{o.sz_}
		//,cap_{o.cap_}			// FIXME: copy o.sz_ not cap_ ?
		//,elem_{ alloc_.allocate(sz_) }
	{
		for (size_type i = 0; i < this->sz_; ++i)
			this->alloc_.construct(&this->elem_[i], o.elem_[i]);	// TODO: deprecated in C++17
	}

	vector& operator = (const vector&);				// FIXME: not `const vector<T, A>&` ??

	vector(vector&& o)
		:vector_base<T, A>{o.alloc_, o.sz_, o.cap_, o.elem_}
		//alloc_{o.alloc_}, sz_{o.sz_}, cap_{o.cap_}, elem_{o.elem_} // FIXME: delete
	{
		o.sz_ = o.cap_ = 0;
		o.elem_ = nullptr;
	}

	vector& operator = (vector&& o);				// FIXME: not `const vector<T, A>&` ??

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

//private:						// FIXME: delete
	//allocator_type		alloc_;
	//size_type		sz_;
	//size_type		cap_;
	//value_type*		elem_;
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

//
// exception unsafe code
// XXX: fix it
//
template <typename T, typename A>
void vector<T, A>::reserve(size_type cap)
{
	if (cap <= this->cap_) return;

	//vector_base<T, A> vb{this->alloc_, cap};				// XXX FIXME: shouldn't the sz_ remain unchanged?
	vector_base<T, A> vb{this->alloc_, this->sz_, cap};

	for (size_type i = 0; i < this->sz_; ++i)
		this->alloc_.construct(&vb.elem_[i], this->elem_[i]);		// TODO: deprecated in C++17
	for (size_type i = 0; i < this->sz_; ++i)
		this->alloc_.destroy(&this->elem_[i]);				// TODO: deprecated in C++17

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
// XXX: check for exception safety
//
template <typename T, typename A>
void vector<T, A>::resize(size_type sz, const value_type& val)
{
	reserve(sz);
	for (size_type i = this->sz_; i < sz; ++i)
		this->alloc_.construct(&this->elem_[i], val);		// TODO: deprecated in C++17
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


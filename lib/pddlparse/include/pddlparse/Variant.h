#ifndef __PDDL_PARSE__VARIANT_H
#define __PDDL_PARSE__VARIANT_H

#include <memory>
#include <ostream>
#include <type_traits>

namespace pddl
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Variant (from clingo, written by Roland Kaminski)
//
////////////////////////////////////////////////////////////////////////////////////////////////////

namespace detail {

template <class T, class... U>
struct TypeInList : std::false_type { };

template <class T, class... U>
struct TypeInList<T, T, U...> : std::true_type { };

template <class T, class V, class... U>
struct TypeInList<T, V, U...> : TypeInList<T, U...> { };

template <unsigned, class... U>
struct VariantHolder;

template <unsigned n>
struct VariantHolder<n> {
	bool check_type() const { return type_ == 0; }
	void emplace() { }
	void emplace2() { }
	void copy(VariantHolder const &) { }
	void destroy() {
		type_ = 0;
		data_ = nullptr;
	}
	void print(std::ostream &) const { }
	void swap(VariantHolder &other) {
		std::swap(type_, other.type_);
		std::swap(data_, other.data_);
	}
	unsigned type_ = 0;
	void *data_ = nullptr;
};

template <unsigned n, class T, class... U>
struct VariantHolder<n, T, U...> : VariantHolder<n+1, U...>{
	using Helper = VariantHolder<n+1, U...>;
	using Helper::check_type;
	using Helper::emplace;
	using Helper::emplace2;
	using Helper::data_;
	using Helper::type_;
	bool check_type(T *) const { return type_ == n; }
	template <class... Args>
	void emplace(T *, Args&& ...x) {
		data_ = new T{std::forward<Args>(x)...};
		type_ = n;
	}
	// NOTE: http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_defects.html#1467
	template <class... Args>
	void emplace2(T *, Args&& ...x) {
		data_ = new T(std::forward<Args>(x)...);
		type_ = n;
	}
	void copy(VariantHolder const &src) {
		if (src.type_ == n) {
			data_ = new T(*static_cast<T const*>(src.data_));
			type_ = src.type_;
		}
		Helper::copy(src);
	}
	// NOTE: workaround for visual studio (C++14 can also simply use auto)
#   define CLINGO_VARIANT_RETURN(Type) decltype(std::declval<V>().visit(std::declval<Type&>(), std::declval<Args>()...))
	template <class V, class... Args>
	using Ret_ = CLINGO_VARIANT_RETURN(T);
	template <class V, class... Args>
	using ConstRet_ = CLINGO_VARIANT_RETURN(T const);
	// non-const
	template <class V, class U1, class... U2, class... Args>
	auto accept_(V &&visitor, Args &&... args) -> CLINGO_VARIANT_RETURN(T) {
		static_assert(std::is_same<Ret_<V, Args...>, typename Helper::template Ret_<V, Args...>>::value, "");
		return n == type_
			? visitor.visit(*static_cast<T*>(data_), std::forward<Args>(args)...)
			: Helper::template accept<V>(std::forward<V>(visitor), std::forward<Args>(args)...);
	}
	template <class V, class... Args>
	auto accept_(V &&visitor, Args &&... args) -> CLINGO_VARIANT_RETURN(T) {
		assert(n == type_);
		return visitor.visit(*static_cast<T*>(data_), std::forward<Args>(args)...);
	}
	template <class V, class... Args>
	auto accept(V &&visitor, Args &&... args) -> CLINGO_VARIANT_RETURN(T) {
		return accept_<V, U...>(std::forward<V>(visitor), std::forward<Args>(args)...);
	}
	// const
	template <class V, class U1, class... U2, class... Args>
	auto accept_(V &&visitor, Args &&... args) const -> CLINGO_VARIANT_RETURN(T const) {
		static_assert(std::is_same<ConstRet_<V, Args...>, typename Helper::template ConstRet_<V, Args...>>::value, "");
		return n == type_
			? visitor.visit(*static_cast<T const *>(data_), std::forward<Args>(args)...)
			: Helper::template accept<V>(std::forward<V>(visitor), std::forward<Args>(args)...);
	}
	template <class V, class... Args>
	auto accept_(V &&visitor, Args &&... args) const -> CLINGO_VARIANT_RETURN(T const) {
		assert(n == type_);
		return visitor.visit(*static_cast<T const *>(data_), std::forward<Args>(args)...);
	}
	template <class V, class... Args>
	auto accept(V &&visitor, Args &&... args) const -> CLINGO_VARIANT_RETURN(T const) {
		return accept_<V, U...>(std::forward<V>(visitor), std::forward<Args>(args)...);
	}
#   undef CLINGO_VARIANT_RETURN
	void destroy() {
		if (n == type_) { delete static_cast<T*>(data_); }
		Helper::destroy();
	}
	void print(std::ostream &out) const {
		if (n == type_) { out << *static_cast<T const*>(data_); }
		Helper::print(out);
	}
};

}

template <class T>
class Optional {
public:
	Optional() { }
	Optional(T const &x) : data_(new T(x)) { }
	Optional(T &x) : data_(new T(x)) { }
	Optional(T &&x) : data_(new T(std::move(x))) { }
	template <class... Args>
	Optional(Args&&... x) : data_(new T{std::forward<Args>(x)...}) { }
	Optional(Optional &&opt) noexcept : data_(opt.data_.release()) { }
	Optional(Optional const &opt) : data_(opt ? new T(*opt.get()) : nullptr) { }
	Optional &operator=(T const &x) {
		clear();
		data_.reset(new T(x));
	}
	Optional &operator=(T &x) {
		clear();
		data_.reset(new T(x));
	}
	Optional &operator=(T &&x) {
		clear();
		data_.reset(new T(std::move(x)));
	}
	Optional &operator=(Optional &&opt) noexcept {
		data_ = std::move(opt.data_);
	}
	Optional &operator=(Optional const &opt) {
		clear();
		data_.reset(opt ? new T(*opt.get()) : nullptr);
	}
	T *get() { return data_.get(); }
	T const *get() const { return data_.get(); }
	T *operator->() { return get(); }
	T const *operator->() const { return get(); }
	T &operator*() & { return *get(); }
	T const &operator*() const & { return *get(); }
	T &&operator*() && { return std::move(*get()); }
	T const &&operator*() const && { return std::move(*get()); }
	template <class... Args>
	void emplace(Args&&... x) {
		clear();
		data_(new T{std::forward<Args>(x)...});
	}
	void clear() { data_.reset(nullptr); }
	explicit operator bool() const { return data_.get() != nullptr; }
private:
	std::unique_ptr<T> data_;
};

template <class... T>
class Variant {
	using Holder = detail::VariantHolder<1, T...>;
public:
	Variant(Variant const &other) : Variant(other.data_) { }
	Variant(Variant &&other) noexcept { data_.swap(other.data_); }
	template <class U>
	Variant(U &&u, typename std::enable_if<detail::TypeInList<U, T...>::value>::type * = nullptr) { emplace2<U>(std::forward<U>(u)); }
	template <class U>
	Variant(U &u, typename std::enable_if<detail::TypeInList<U, T...>::value>::type * = nullptr) { emplace2<U>(u); }
	template <class U>
	Variant(U const &u, typename std::enable_if<detail::TypeInList<U, T...>::value>::type * = nullptr) { emplace2<U>(u); }
	template <class U, class... Args>
	static Variant make(Args&& ...args) {
		Variant<T...> x;
		x.data_.emplace(static_cast<U*>(nullptr), std::forward<Args>(args)...);
		return std::move(x);
	}
	~Variant() { data_.destroy(); }
	Variant &operator=(Variant const &other) { return *this = other.data_; }
	Variant &operator=(Variant &&other) noexcept { return *this = std::move(other.data_); }
	template <class U>
	typename std::enable_if<detail::TypeInList<U, T...>::value, Variant>::type &operator=(U &&u) {
		emplace2<U>(std::forward<U>(u));
		return *this;
	}
	template <class U>
	typename std::enable_if<detail::TypeInList<U, T...>::value, Variant>::type &operator=(U &u) {
		emplace2<U>(u);
		return *this;
	}
	template <class U>
	typename std::enable_if<detail::TypeInList<U, T...>::value, Variant>::type &operator=(U const &u) {
		emplace2<U>(u);
		return *this;
	}
	template <class U>
	U &get() {
		if (!data_.check_type(static_cast<U*>(nullptr))) { throw std::bad_cast(); }
		return *static_cast<U*>(data_.data_);
	}
	template <class U>
	U const &get() const {
		if (!data_.check_type(static_cast<U*>(nullptr))) { throw std::bad_cast(); }
		return *static_cast<U*>(data_.data_);
	}
	template <class U, class... Args>
	void emplace(Args&& ...args) {
		Variant<T...> x;
		x.data_.emplace(static_cast<U*>(nullptr), std::forward<Args>(args)...);
		data_.swap(x.data_);
	}
	template <class U>
	bool is() const { return data_.check_type(static_cast<U*>(nullptr)); }
	void swap(Variant &other) { data_.swap(other.data_); }
	template <class V, class... Args>
	typename Holder::template Ret_<V, Args...> accept(V &&visitor, Args &&... args) {
		return data_.accept(std::forward<V>(visitor), std::forward<Args>(args)...);
	}
	template <class V, class... Args>
	typename Holder::template ConstRet_<V, Args...> accept(V &&visitor, Args &&... args) const {
		return data_.accept(std::forward<V>(visitor), std::forward<Args>(args)...);
	}
	friend std::ostream &operator<<(std::ostream &out, Variant const &x) {
		x.data_.print(out);
		return out;
	}

private:
	Variant() { }
	Variant(Holder const &data) {
		data_.copy(data);
	}
	Variant &operator=(Holder const &data) {
		Variant x(data);
		data_.swap(x.data_);
		return *this;
	}
	Variant &operator=(Holder &&data) noexcept {
		Holder x;
		x.swap(data);
		// Destroy the old data_ only after securing the new data
		// Otherwise, data would be destroyed together with data_ if it was a descendant of data_
		data_.destroy();
		x.swap(data_);
		return *this;
	}
	template <class U, class... Args>
	void emplace2(Args&& ...args) {
		Variant<T...> x;
		x.data_.emplace2(static_cast<U*>(nullptr), std::forward<Args>(args)...);
		data_.swap(x.data_);
	}

private:
	Holder data_;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif

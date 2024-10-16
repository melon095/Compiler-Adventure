#pragma once

#include <memory>

#define SMARTPTR(type)                                                                                                 \
	using type##Ptr = std::shared_ptr<type>;                                                                           \
	using type##ConstPtr = std::shared_ptr<const type>;                                                                \
	using type##WeakPtr = std::weak_ptr<type>;                                                                         \
	using type##ConstWeakPtr = std::weak_ptr<const type>;                                                              \
	using type##UniquePtr = std::unique_ptr<type>;                                                                     \
	using type##ConstUniquePtr = std::unique_ptr<const type>;

//// https://stackoverflow.com/a/21174979
template<typename Derived, typename Base, typename Del>
std::unique_ptr<Derived, Del> dynamic_unique_ptr_cast(std::unique_ptr<Base, Del>&& p)
{
	if(Derived* result = dynamic_cast<Derived*>(p.get()))
	{
		p.release();
		return std::unique_ptr<Derived, Del>(result, std::move(p.get_deleter()));
	}
	return std::unique_ptr<Derived, Del>(nullptr, p.get_deleter());
}
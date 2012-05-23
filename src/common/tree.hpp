#ifndef TREE_H
#define TREE_H

#include <list>
#include <memory>
#include <stdexcept>

#include "../common/typedefs.hpp"

namespace splicpp
{
	template <typename T>
	class tree : public std::enable_shared_from_this<tree<T>>
	{
		const T data;
		const std::weak_ptr<tree<T>> parent;
		std::list<std::shared_ptr<tree<T>>> children;
		
		tree(const T& data, const std::weak_ptr<tree<T>> parent)
		: data(data)
		, parent(parent)
		, children()
		{}
		
	public:
		tree(const T& data)
		: data(data)
		, parent()
		, children()
		{}
		
		s_ptr<tree<T>> create_child(const T& data)
		{
			const std::weak_ptr<tree<T>> parent(this->shared_from_this());
			const std::shared_ptr<tree<T>> child(new tree<T>(data, parent));
			
			children.push_back(child);
			return child;
		}
		
		void add_child(const s_ptr<tree<T>> child)
		{
			if(child->parent != nullptr)
				throw std::runtime_error("Child already has a parent");
			
			child->parent = std::weak_ptr<tree<T>>(this->shared_from_this());
			children->push_back(child);
		}
		
		bool is_leaf() const
		{
			return (children.size() == 0);
		}
		
		std::list<std::shared_ptr<tree<T>>> fetch_children() const
		{
			return children;
		}
		
		bool has_parent() const
		{
			return parent;
		}
		
		std::shared_ptr<tree<T>> fetch_parent() const
		{
			return std::shared_ptr<tree<T>>(parent);
		}
	};
}

#endif

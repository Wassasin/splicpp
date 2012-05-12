#ifndef TREE_H
#define TREE_H

#include <list>
#include <memory>

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
		
		std::shared_ptr<tree<T>> create_child(const T& data)
		{
			const std::weak_ptr<tree<T>> parent(this->shared_from_this());
			const std::shared_ptr<tree<T>> child(new tree<T>(data, parent));
			
			children.push_back(child);
			return child;
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

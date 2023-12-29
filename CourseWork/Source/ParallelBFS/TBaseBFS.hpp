#ifndef PARALLELBFS_TBASEBFS_HPP
#define PARALLELBFS_TBASEBFS_HPP

#include <concepts>
#include <unordered_map>
#include <vector>
#include <algorithm>

namespace bfs {

template<typename T>
concept CBFSUsable = std::equality_comparable<T> and std::copyable<T>
	and requires(T value) { {std::hash<T>{}(value)} -> std::same_as<std::size_t>; };

template<CBFSUsable T>
using AGraph = std::unordered_map<T, std::vector<T>>;

template<CBFSUsable T, typename Derived>
class TBaseBFS {
	public:
	template<typename... Args>
	static std::optional<std::vector<T>> Do(const AGraph<T>& graph, const T& start, const T& end, Args&&... args);

	protected:
	TBaseBFS(const AGraph<T>& graph, const T& start, const T& end);

	protected:
	std::optional<std::vector<T>> Execute();

	protected:
	const Derived* self() const;
	Derived* self();

	protected:
	template<typename ValueType>
	std::optional<std::vector<T>> DeterminePath(const std::unordered_map<T, ValueType>& predecessorNodes) const;

	protected:
	const AGraph<T>& m_refGraph;
	const T& m_refStart;
	const T& m_refEnd;
};

template<CBFSUsable T, typename Derived>
template<typename... Args>
std::optional<std::vector<T>> TBaseBFS<T, Derived>::Do(const AGraph<T>& graph, const T& start, const T& end, Args&&... args) {
	auto alg = Derived(graph, start, end, std::forward<Args>(args)...);
	return alg.Execute();
}

template<CBFSUsable T, typename Derived>
TBaseBFS<T, Derived>::TBaseBFS(const AGraph<T>& graph, const T& start, const T& end)
	: m_refGraph{graph}, m_refStart{start}, m_refEnd{end} {}

template<CBFSUsable T, typename Derived>
std::optional<std::vector<T>> TBaseBFS<T, Derived>::Execute() {
	return DeterminePath(self()->PredecessorNodesImpl());
}

template<CBFSUsable T, typename Derived>
const Derived* TBaseBFS<T, Derived>::self() const {
	return static_cast<const Derived*>(this);
}

template<CBFSUsable T, typename Derived>
Derived* TBaseBFS<T, Derived>::self() {
	return static_cast<Derived*>(this);
}

template<CBFSUsable T, typename Derived>
template<typename ValueType>
std::optional<std::vector<T>> TBaseBFS<T, Derived>::DeterminePath(const std::unordered_map<T, ValueType>& predecessorNodes) const {
	if(not predecessorNodes.contains(this->m_refEnd)) return std::nullopt;
	auto path = std::vector<T>{this->m_refEnd};
	auto currentNode = path.front();
	while(currentNode != this->m_refStart) {
		Derived::UpdateCurrentNodeImpl(currentNode, predecessorNodes.at(currentNode));
		path.push_back(currentNode);
	}
	std::reverse(path.begin(), path.end());
	return path;
}

}

#endif //PARALLELBFS_TBASEBFS_HPP
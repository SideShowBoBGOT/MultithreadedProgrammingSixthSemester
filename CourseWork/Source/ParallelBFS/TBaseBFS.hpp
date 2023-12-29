#ifndef PARALLELBFS_TBASEBFS_HPP
#define PARALLELBFS_TBASEBFS_HPP

#include <concepts>
#include <unordered_map>
#include <vector>
#include <algorithm>

namespace bfs {

template<std::equality_comparable T>
using TGraph = std::unordered_map<T, std::vector<T>>;

template<std::equality_comparable T, typename Derived>
class TBaseBFS {
	public:
	template<typename... Args>
	static std::optional<std::vector<T>> Do(const TGraph<T>& graph, const T& start, const T& end, Args&&... args);

	protected:
	TBaseBFS(const TGraph<T>& graph, const T& start, const T& end);

	protected:
	const Derived* self() const;
	Derived* self();

	protected:
	const TGraph<T>& m_refGraph;
	const T& m_refStart;
	const T& m_refEnd;
};

template<std::equality_comparable T, typename Derived>
template<typename... Args>
std::optional<std::vector<T>> TBaseBFS<T, Derived>::Do(const TGraph<T>& graph, const T& start, const T& end, Args&&... args) {
	auto alg = Derived(graph, start, end, std::forward<Args>(args)...);
	return alg.Execute();
}

template<std::equality_comparable T, typename Derived>
TBaseBFS<T, Derived>::TBaseBFS(const TGraph<T>& graph, const T& start, const T& end)
	: m_refGraph{graph}, m_refStart{start}, m_refEnd{end} {}

template<std::equality_comparable T, typename Derived>
const Derived* TBaseBFS<T, Derived>::self() const {
	return static_cast<const Derived*>(this);
}

template<std::equality_comparable T, typename Derived>
Derived* TBaseBFS<T, Derived>::self() {
	return static_cast<Derived*>(this);
}

}

#endif //PARALLELBFS_TBASEBFS_HPP

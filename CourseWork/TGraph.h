#ifndef COURSEWORK_TGRAPH_H
#define COURSEWORK_TGRAPH_H

#include <gsl/gsl>

#include <vector>
#include <map>
#include <memory>
#include <algorithm>

using T = int;

using PNode = gsl::not_null<std::shared_ptr<T>>;

class TGraph;
using PGraph = gsl::not_null<std::shared_ptr<TGraph>>;

class TGraph {
    public:
    TGraph()=default;
    virtual ~TGraph()=default;

    public:
    [[nodiscard]] const std::map<PNode, std::vector<PNode>>& NodesMap() const {
        return m_mNodesMap;
    }

    void AddConnection(const PNode& from, const PNode& to) {
        m_mNodesMap[from].push_back(to);
    }

    void CreateNode(const T& data) {
        m_mNodesMap.emplace(std::make_shared<T>(data), std::vector<PNode>());
    }

    protected:
    std::map<PNode, std::vector<PNode>> m_mNodesMap;
};


#endif //COURSEWORK_TGRAPH_H

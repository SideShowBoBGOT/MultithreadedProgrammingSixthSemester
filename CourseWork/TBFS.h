#ifndef COURSEWORK_TBFS_H
#define COURSEWORK_TBFS_H

#include "TGraph.h"
#include "TSyncQueue.h"

#include <thread>
#include <ranges>

template<typename T>
class TBFS {
    public:
    TBFS(const PNode& from, const PNode& to, const PGraph& graph, unsigned totalThreads)
        : m_pFrom{from}, m_pTo{to}, m_pGraph{graph}, m_uTotalThreads{totalThreads} {};
    virtual ~TBFS()=default;

    std::vector<PNode> FindPath() {
        for(const auto& n : m_pGraph->NodesMap().at(m_pFrom)) {
            m_qQueue.Push(n);
        }

        for(auto i = 0; i < m_uTotalThreads; ++i) {
            m_vThreads.emplace_back([](const std::stop_token& s) {});
        }
    }

    void ThreadTask(const std::stop_token& stopToken) {
        while(!stopToken.stop_requested()) {
            if(const auto nodeOpt = m_qQueue.Pop()) {
                const auto& node = nodeOpt.value();
                if(node == )
            }
        }
    }

    protected:
    const PNode m_pFrom;
    const PNode m_pTo;
    const PGraph m_pGraph;
    const unsigned m_uTotalThreads = 0;
    std::vector<std::jthread> m_vThreads;
    TSyncQueue<PNode> m_qQueue;
};

#endif //COURSEWORK_TBFS_H

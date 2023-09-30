#ifndef COURSEWORK_TBFS_H
#define COURSEWORK_TBFS_H

#include "TGraph.h"
#include "TSyncQueue.h"

#include <thread>
#include <ranges>

template<typename T>
class TBFS {
    public:
    TBFS()=default;
    virtual ~TBFS()=default;

    public:
    std::vector<PNode> FindPath(const PNode& from, const PNode& to, const PGraph& graph, unsigned totalThreads) {
        m_pFrom = from;
        m_pTo = to;
        m_pGraph = graph;

        for(const auto& n : m_pGraph->NodesMap().at(m_pFrom)) {
            m_qQueue.Push(n);
        }

        for(auto i = 0; i < totalThreads; ++i) {
            m_vThreads.emplace_back([](const std::stop_token& s) {});
        }
    }

    protected:
    void ThreadTask(const std::stop_token& stopToken) {
        while(!stopToken.stop_requested()) {
            const auto nodeOpt = m_qQueue.Pop();
            if(!nodeOpt) continue;

            const auto& node = nodeOpt.value();
            if(node != m_pTo) {

                continue;
            }

            if(m_xStopMutex.try_lock()) {

                m_xStopMutex.unlock();
            }
            return;
        }
    }

    protected:
    PNode m_pFrom;
    PNode m_pTo;
    PGraph m_pGraph;

    protected:
    std::mutex m_xStopMutex;

    protected:
    std::vector<std::jthread> m_vThreads;

    TSyncQueue<PNode> m_qQueue;
};

#endif //COURSEWORK_TBFS_H

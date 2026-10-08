#pragma once
#include "result.h"
#include <chrono>
#include <iostream>
#include <string>

namespace Pathfinding {
    // Base class to inherit from that contains various pathfinding algorithims
    class PathfindingMethod {
        protected:
            World* m_CurrentWorld;

            std::chrono::high_resolution_clock::time_point m_Timer;
            inline void StartTiming() {
                m_Timer = std::chrono::high_resolution_clock::now();
            }
            
            // Time it takes for pathfinding to complete in Miliseconds
            inline float EndTiming(bool _print = false) {
                auto stop = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - m_Timer);
                if (_print) {
                    std::cout << "Pathfinding Executed In: " << duration.count() / 1000.0f << "ms" << std::endl;
                }

                return duration.count() / 1000.0f;

            }
            
        public:
            // Create a reference to the world for this method
            // Do not inherit from
            void SetWorld(World* _current_world) {
                m_CurrentWorld = _current_world;
            }

            virtual PathResult ResolvePath(Node* _start, Node* _end) {return {};};

            virtual ~PathfindingMethod() = default;
    };
    
    class AStar : public PathfindingMethod {
        public:
        PathResult ResolvePath(Node* _start, Node* _end);
    };


    // Helper struct for handling priority in priorityqueue
    struct PathFindingEntry {
    public:
        float priority;
        Node* node;

        bool operator<(const PathFindingEntry& other) const {
            return priority < other.priority;
        }

        bool operator>(const PathFindingEntry& other) const {
            return priority > other.priority;
        }

        PathFindingEntry(Node* _node, float _priority) {
            node = _node;
            priority = _priority;
        }
    };
}
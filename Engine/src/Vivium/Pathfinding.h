#pragma once

#include "Core.h"
#include "Vector2.h"
#include "Logger.h"

namespace Vivium {
	// Ripped from
	// https://dev.to/jansonsa/a-star-a-path-finding-c-4a4h
	class Pathfinder {
	public:
		struct Node {
			Vector2<int> pos;
			Vector2<int> parent_pos = INT_MAX;

			float g_cost = FLT_MAX;
			float h_cost = FLT_MAX;
			float f_cost = FLT_MAX;

			Node() = default;

			Node(const Vector2<int>& pos);

			Node(const Node& other) = default;
			Node(Node&& other) noexcept = default;

			Node& operator=(const Node& other) = default;
			Node& operator=(Node&& other) noexcept = default;

			friend bool operator<(const Node& a, const Node& b) {
				return a.f_cost < b.f_cost;
			}
		};

		typedef Ref(bool[]) ObstacleMap_t;
		typedef Node* NodeMap_t;

	private:
		Vector2<int> m_Start;
		Vector2<int> m_End;
		Vector2<int> m_Dim;

		ObstacleMap_t m_ObstacleMap;
		NodeMap_t m_NodeMap;

		// Check not an obstacle and not out of bounds
		bool m_IsValidNode(const Vector2<int>& pos);

		std::vector<Node> m_MakePath();

	public:
		Pathfinder(const Vector2<int>& start, const Vector2<int>& end, const Vector2<int>& map_dim, ObstacleMap_t map);

		std::vector<Node> Calculate();

		~Pathfinder();
	};
}

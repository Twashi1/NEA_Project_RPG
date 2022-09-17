#include "Pathfinding.h"

namespace Vivium {
	Pathfinder::Node::Node(const Vector2<int>& pos)
		: pos(pos)
	{}

	// Check not an obstacle and not out of bounds

	bool Pathfinder::m_IsValidNode(const Vector2<int>& pos) {
		// OOB check
		if (pos.x < 0 || pos.x >= m_Dim.x || pos.y < 0 || pos.y >= m_Dim.y) {
			return false;
		}

		// Is obstacle check (obstacles represented as true, walkable represented as false)
		return !m_ObstacleMap[pos.x + pos.y * m_Dim.x];
	}
	
	std::vector<Pathfinder::Node> Pathfinder::m_MakePath() {
		LogTrace("Found path");

		std::vector<Node> path;
		std::vector<Node> useable_path;

		Vector2<int> pos = m_End;

		while (!(m_NodeMap[m_End.x + m_End.y * m_Dim.x].parent_pos == pos) && m_End != Vector2<int>(-1)) {
			path.push_back(m_NodeMap[m_End.x + m_End.y * m_Dim.x]);

			pos = m_NodeMap[m_End.x + m_End.y * m_Dim.x].parent_pos;
		}

		path.push_back(m_NodeMap[m_End.x + m_End.y * m_Dim.x]);

		// NOTE: reversing it?
		while (!path.empty()) {
			Node& last = path[path.size() - 1];
			path.pop_back();

			useable_path.emplace_back(last);
		}

		return useable_path;
	}
	
	Pathfinder::Pathfinder(const Vector2<int>& start, const Vector2<int>& end, const Vector2<int>& map_dim, ObstacleMap_t map)
		: m_Start(start), m_End(end), m_Dim(map_dim), m_ObstacleMap(map)
	{
		// Initialise node map
		m_NodeMap = new Node[m_Dim.x * m_Dim.y];

		for (std::size_t i = 0; i < m_Dim.x * m_Dim.y; i++) {
			int y = i / m_Dim.x;
			int x = i - (y * m_Dim.x);

			m_NodeMap[i] = Node({ x, y });
		}
	}
	
	std::vector<Pathfinder::Node> Pathfinder::Calculate() {
		if (!m_IsValidNode(m_End)) { return {}; }
		{
			if (m_Start == m_End) { return {}; }
		}

		// All nodes that have already been checked
		bool* closed = new bool[m_Dim.x * m_Dim.y];

		// Create node map
		std::vector<Node> open;
		open.reserve(m_Dim.x * m_Dim.y); // Not like we really care about memory efficiency anyway

		// Setup our starting node
		Node& start_node = m_NodeMap[m_Start.x + m_Start.y * m_Dim.x];
		start_node.f_cost = 0.0f;
		start_node.g_cost = 0.0f;
		start_node.h_cost = 0.0f;
		start_node.parent_pos = m_Start;

		open.push_back(m_NodeMap[m_Start.x + m_Start.y * m_Dim.x]);

		bool destination_found = false;

		while (!open.empty() && open.size() < m_Dim.x * m_Dim.y) {
			Node node = m_NodeMap[0];

			while (m_IsValidNode(node.pos)) {
				float temp = FLT_MAX;

				Node* closest_node;
				std::size_t closest_index = INT_MAX;

				for (std::size_t i = 0; i < open.size(); i++) {
					Node& current_node = open[i];

					if (current_node.f_cost < temp) {
						temp = current_node.f_cost;
						closest_node = &current_node;
						closest_index = i;
					}
				}

				node = *closest_node;
				open.erase(open.begin() + closest_index);
			}

			closed[node.pos.x + node.pos.y * m_Dim.x] = true;

			for (int offx = -1; offx <= 1; offx++) {
				for (int offy = -1; offy <= 1; offy++) {
					Vector2<int> off_vec(offx, offy);
					Vector2<int> look_pos = node.pos + off_vec;

					if (m_IsValidNode(look_pos)) {
						if (look_pos == m_End) {
							destination_found = true;
							m_NodeMap[look_pos.x + look_pos.y * m_Dim.x].parent_pos = node.pos;

							return m_MakePath();
						}
						else if (!closed[look_pos.x + look_pos.y * m_Dim.x]) {
							float g_cost = node.g_cost + 1.0f;
							float h_cost = Vector2<float>::Distance(look_pos, m_End);
							float f_cost = g_cost + h_cost;

							if (m_NodeMap[look_pos.x + look_pos.y * m_Dim.x].f_cost == FLT_MAX || m_NodeMap[look_pos.x + look_pos.y * m_Dim.x].f_cost > f_cost) {
								Node& node_to_update = m_NodeMap[look_pos.x + look_pos.y * m_Dim.x];
								node_to_update.g_cost = g_cost;
								node_to_update.h_cost = h_cost;
								node_to_update.f_cost = f_cost;

								node_to_update.parent_pos = node.pos;
								open.push_back(node_to_update);
							}
						}
					}
				}
			}

			if (!destination_found) {
				LogTrace("Couldn't find destination while pathfinding!");

				return {};
			}
		}

		delete[] closed;
	}
	
	Pathfinder::~Pathfinder()
	{
		delete[] m_NodeMap;
	}
}

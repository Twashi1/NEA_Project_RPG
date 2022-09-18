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
		std::size_t pos_idx = pos.x + pos.y * m_Dim.x;

		while (!(m_NodeMap[pos_idx].parent_pos == pos) && m_End != Vector2<int>(-1)) {
			path.push_back(m_NodeMap[pos_idx]);

			pos = m_NodeMap[pos_idx].parent_pos;
			pos_idx = pos.x + pos.y * m_Dim.x;
		}

		path.push_back(m_NodeMap[pos_idx]);

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
		if (!m_IsValidNode(m_End)) { LogTrace("End was not valid node"); return {}; }
		if (m_Start == m_End) { LogTrace("Start was also end"); return {}; }

		// All nodes that have already been checked
		bool* closed = new bool[m_Dim.x * m_Dim.y];

		std::fill(closed, closed + m_Dim.x * m_Dim.y, false);

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

		while ((!open.empty()) && (open.size() < m_Dim.x * m_Dim.y)) {
			Node node;

			do {
				float temp = FLT_MAX;

				std::vector<Node>::iterator closest_it;

				for (std::vector<Node>::iterator it = open.begin(); it != open.end(); it = std::next(it)) {
					Node& current = *it;

					if (current.f_cost < temp) {
						temp = current.f_cost;
						closest_it = it;
					}
				}

				// We've looked at this node
				node = *closest_it;
				open.erase(closest_it);
			} while (!m_IsValidNode(node.pos));

			Vector2<int> pos = node.pos;
			std::size_t idx = pos.x + pos.y * m_Dim.x;
			closed[idx] = true;

			for (int offx = -1; offx <= 1; offx++) {
				for (int offy = -1; offy <= 1; offy++) {
					// If we're checking one of the diagonals
					if (std::abs(offx) == std::abs(offy) && offx != 0) {
						// Check that no obstacles blocking diag path
						/*
						Going diagonal here is valid
						00 \
						00  \

						Going diagonal here is **not** valid (take the long path around)
						01 \
						10  \
						
						Going diagonal here is also **not** valid
						00 \	(instead takes this path) -¬
						10  \							   ¦
						*/
						Vector2<int> loc_a = node.pos + Vector2<int>(offx, 0);
						Vector2<int> loc_b = node.pos + Vector2<int>(0, offy);

						// If either are obstacles
						if (m_ObstacleMap[loc_a.x + loc_a.y * m_Dim.x] || m_ObstacleMap[loc_b.x + loc_b.y * m_Dim.x]) {
							continue;
						}
					}

					float gnew, hnew, fnew;

					Vector2<int> off_vec(offx, offy);
					Vector2<int> look_pos = node.pos + off_vec;
					std::size_t look_idx = look_pos.x + look_pos.y * m_Dim.x;

					if (m_IsValidNode(look_pos)) {
						if (look_pos == m_End) {
							destination_found = true;
							m_NodeMap[look_idx].parent_pos = pos;

							return m_MakePath();
						}
						else if (!closed[look_idx]) {
							gnew = node.g_cost + 1.0f;
							hnew = Vector2<float>::Distance(look_pos, m_End);
							fnew = gnew + hnew;

							if (m_NodeMap[look_idx].f_cost == FLT_MAX || m_NodeMap[look_idx].f_cost > fnew) {
								Node& node_to_update = m_NodeMap[look_idx];
								node_to_update.g_cost = gnew;
								node_to_update.h_cost = hnew;
								node_to_update.f_cost = fnew;

								node_to_update.parent_pos = pos;
								open.push_back(node_to_update);
							}
						}
					}
				}
			}
		}

		if (!destination_found) {
			LogTrace("Couldn't find destination while pathfinding!");

			return {};
		}

		delete[] closed;
	}
	
	Pathfinder::~Pathfinder()
	{
		delete[] m_NodeMap;
	}
}

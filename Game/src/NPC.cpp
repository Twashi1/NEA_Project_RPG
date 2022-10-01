#include "NPC.h"
#include "World.h"

namespace Game {
	namespace Pathfinding {
		~NPC()
		{
			for (auto& [id, client] : behaviour_data) {
				delete client;
			}
		}
	}
}

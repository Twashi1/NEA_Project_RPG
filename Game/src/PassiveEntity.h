#pragma once

#include "NPC.h"

namespace Game {
	// Considered two different approaches for passive entities
	// a) PassiveEntity base class, and inherited class for each different type (pig, cow, sheep, etc.)
	// b) PassiveEntity stores an "PassiveEntityID",
	//		which can be used as an index into a "Properties" table, storing information on texture, what item they drop when killed, etc.
	//		this approach doesn't really allow for entity-specific data, nor entity-specific behaviour (like a sheep eating grass, or
	//		cows being milked). Maybe if I had more time I'd use this approach (implemented as an ECS to allow entity-specific stuff?)
	// TODO: implement b here, or have a CreateInstance method and a passive entity id
	// TODO: should be batch rendering npcs
}

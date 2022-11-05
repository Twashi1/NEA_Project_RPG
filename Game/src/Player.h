#pragma once

#include "World.h"
#include "Inventory.h"
#include "Recipe.h"
#include "CraftingInventory.h"
#include "Equipable.h"

namespace Game {
    class Player : Vivium::Streamable {
    private:
        Vivium::Timer m_Timer; // Stores last time object was updated
        double m_Time = 0.0;

        static const Vivium::Vector2<int> s_HEAD_WALK_0;
        static const Vivium::Vector2<int> s_FEET_WALK_0;

        std::unique_ptr<Vivium::Shader> m_PlayerShader;
        std::array<float, 8> m_WALK_0_TEXCOORDS;

        void m_RenderPlayer();
        void m_RenderSelectedTile();
        void m_RenderInventory();

        // Update acceleration according to client inputs
        void m_UpdateMovement();
        void m_UpdateSelectedSlot();
        void m_UpdateInventory(World& world);
        void m_UpdateSelectedTile(World& world);

        Vivium::Quad* m_SelectedTileQuad;
        Vivium::Shader* m_SelectedTileShader;

        Inventory m_MainInventory;
        CraftingInventory m_CraftingInventory;

        Inventory::Slot m_SelectedSlot = Inventory::Slot::INVALID;
        Item m_SelectedItem = Item(Item::ID::VOID, 0);

        std::shared_ptr<HandEquipable> m_HandEquipable = nullptr;

        bool m_isMainInventoryOpened = false;

    public:
        std::shared_ptr<Vivium::Body> body; // Describes physical properties of player and how it should interact with other objects
        std::shared_ptr<Vivium::Quad> quad; // Holds vertex buffer and index buffer of player

        Vivium::Vector2<int> selected_tile_pos; // World position of hovered tile
        Tile selected_tile;

        Health health;

        // Miscellaneous constants for player movement
        static constexpr float FRICTION = 20.0f;
        static constexpr float MAXACCEL = 15000.0f;

        void Update(World& world);

        void Render();

        void Write(Vivium::Serialiser& s) const override;
        void Read(Vivium::Serialiser& s) override;

        const Inventory& GetMainInventory() const;

        void Save(World& world);

        // Load from world
        Player(const std::string& world_name);
        Player();
        ~Player();

        friend World;
    };
}
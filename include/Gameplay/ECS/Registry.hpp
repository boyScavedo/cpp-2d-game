#pragma once

#include <vector>
#include <memory>
#include <map>
#include <algorithm>
#include <typeindex>

#include "Gameplay/ECS/Components.hpp"

namespace Gameplay
{
    namespace ECS
    {
        using EntityID = int;

        struct LevelConfig
        {
            bool isLeftWallClamped = true;
            bool isRightWallClamped = true;
        };

        class Registry
        {
        public:
            EntityID createEntity() { return m_nextEntityID++; }

            LevelConfig levelConfig;

            // Component Storage
            std::map<EntityID, Transform> transforms;
            std::map<EntityID, Sprite> sprites;
            std::map<EntityID, Physics> physics;
            std::map<EntityID, PlayerControl> players;
            std::map<EntityID, Parallax> backgrounds;
            std::map<EntityID, Collider> colliders;
            std::map<EntityID, LevelExit> exits;

            // --- Helper Methods ---
            template <typename T> bool has(EntityID entity) const;

            // Non-const get (for systems that modify components)
            template <typename T> T& get(EntityID entity);

            // Const get (CRITICAL for RenderSystem)
            template <typename T> const T& get(EntityID entity) const;

            template <typename T> void add(EntityID entity, T component);

            void destroy(EntityID entity) {
                transforms.erase(entity);
                sprites.erase(entity);
                physics.erase(entity);
                players.erase(entity);
                backgrounds.erase(entity);
                colliders.erase(entity);
                exits.erase(entity);
            }

        private:
            EntityID m_nextEntityID = 0;
        };

        // --- SPECIALIZATIONS ---
        // Note: Use 'EntityID' directly, not 'Registry::EntityID'

        // --- Transform ---
        template <> inline bool Registry::has<Transform>(EntityID e) const { return transforms.count(e); }
        template <> inline Transform& Registry::get<Transform>(EntityID e) { return transforms.at(e); }
        template <> inline const Transform& Registry::get<Transform>(EntityID e) const { return transforms.at(e); }
        template <> inline void Registry::add<Transform>(EntityID e, Transform c) { transforms[e] = c; }

        // --- Sprite ---
        template <> inline bool Registry::has<Sprite>(EntityID e) const { return sprites.count(e); }
        template <> inline Sprite& Registry::get<Sprite>(EntityID e) { return sprites.at(e); }
        template <> inline const Sprite& Registry::get<Sprite>(EntityID e) const { return sprites.at(e); }
        template <> inline void Registry::add<Sprite>(EntityID e, Sprite c) { sprites[e] = c; }
        
        // --- Physics ---
        template <> inline bool Registry::has<Physics>(EntityID e) const { return physics.count(e); }
        template <> inline Physics& Registry::get<Physics>(EntityID e) { return physics.at(e); }
        template <> inline const Physics& Registry::get<Physics>(EntityID e) const { return physics.at(e); }
        template <> inline void Registry::add<Physics>(EntityID e, Physics c) { physics[e] = c; }
        
        // --- PlayerControl ---
        template <> inline bool Registry::has<PlayerControl>(EntityID e) const { return players.count(e); }
        template <> inline PlayerControl& Registry::get<PlayerControl>(EntityID e) { return players.at(e); }
        template <> inline const PlayerControl& Registry::get<PlayerControl>(EntityID e) const { return players.at(e); }
        template <> inline void Registry::add<PlayerControl>(EntityID e, PlayerControl c) { players[e] = c; }
        
        // --- Parallax ---
        template <> inline bool Registry::has<Parallax>(EntityID e) const { return backgrounds.count(e); }
        template <> inline Parallax& Registry::get<Parallax>(EntityID e) { return backgrounds.at(e); }
        template <> inline const Parallax& Registry::get<Parallax>(EntityID e) const { return backgrounds.at(e); }
        template <> inline void Registry::add<Parallax>(EntityID e, Parallax c) { backgrounds[e] = c; }

        // --- Collider ---
        template <> inline bool Registry::has<Collider>(EntityID e) const { return colliders.count(e); }
        template <> inline Collider& Registry::get<Collider>(EntityID e) { return colliders.at(e); }
        template <> inline const Collider& Registry::get<Collider>(EntityID e) const { return colliders.at(e); }
        template <> inline void Registry::add<Collider>(EntityID e, Collider c) { colliders[e] = c; }

        // --- LevelExit ---
        template <> inline bool Registry::has<LevelExit>(EntityID e) const { return exits.count(e); }
        template <> inline LevelExit& Registry::get<LevelExit>(EntityID e) { return exits.at(e); }
        template <> inline const LevelExit& Registry::get<LevelExit>(EntityID e) const { return exits.at(e); }
        template <> inline void Registry::add<LevelExit>(EntityID e, LevelExit c) { exits[e] = c; }

    } // namespace ECS
} // namespace Gameplay
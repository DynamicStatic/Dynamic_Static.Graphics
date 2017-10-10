
/*
==========================================
    Copyright (c) 2017 Dynamic_Static
    Licensed under the MIT license
    http://opensource.org/licenses/MIT
==========================================
*/

// Based on "Make a Neon Vector Shooter in XNA"
// https://gamedevelopment.tutsplus.com/series/cross-platform-vector-shooter-xna--gamedev-10559

#pragma once

#include "Sprite_ex.hpp"

#include "Dynamic_Static/Core/Input.hpp"
#include "Dynamic_Static/Core/Math.hpp"
#include "Dynamic_Static/Core/Time.hpp"

namespace ShapeBlaster_ex {

    class Entity
    {
    public:
        class Manager;

    protected:
        Sprite* mSprite { nullptr };
        dst::Vector2 mVelocity;
        float mRadius { 20 };
        float mEnabled { false };

    public:
        virtual void update(
            const dst::Clock& clock,
            const dst::Input& input,
            const dst::Vector2& playField
        ) = 0;
    };

} // namespace ShapeBlaster_ex
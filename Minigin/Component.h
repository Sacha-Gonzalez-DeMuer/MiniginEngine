#pragma once
#include "Transform.h"
#include "IObject.h"
#include <memory>
#include "GameObject.h"

namespace dae
{
    class GameObject;

    class Component : public IObject
    {
    public:
        Component() {};

        void SetOwner(std::shared_ptr<GameObject> owner) { m_owner = owner; };
    protected:
        std::shared_ptr<GameObject> m_owner;

    private:
    };
}



//
// Created by Marcin Praski on 23.10.19.
//

#include "Scene.hpp"

engine::Scene::Scene(std::vector<EntityPtr> &&layers)
    : mLayers{std::move(layers)} {
}

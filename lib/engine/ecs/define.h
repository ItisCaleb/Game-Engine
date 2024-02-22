#ifndef ECS_DEFINE_H_
#define ECS_DEFINE_H_

#include <cstddef>
#include <cstdint>
#include <bitset>

using Entity = std::size_t;

const Entity MAX_ENTITIES = 50000;

using ComponentType = std::uint8_t;

const ComponentType MAX_COMPONENTS = 64;

using ComponentFlag = std::bitset<MAX_COMPONENTS>;

#endif
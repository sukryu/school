#pragma once
#include "entity.hpp"
#include <memory>
#include <vector>

template<typename T>
class EntityManager {
public:
    virtual ~EntityManager() = default;
    virtual void add(std::unique_ptr<T> entity) = 0;
    virtual T* get_at(const Position& pos) = 0;
    virtual const T* get_at(const Position& pos) const = 0;
    virtual void remove(T* entity) = 0;
    virtual const std::vector<std::unique_ptr<T>>& get_all() const = 0;
};
#include <raylib.h>
#include <raymath.h>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include "Entity.hpp"

// to find an item in a vector: https://stackoverflow.com/questions/571394/how-to-find-out-if-an-item-is-present-in-a-stdvector
// to convert an iterator to a "size_t" variable for a vector: https://stackoverflow.com/questions/58767786/c-turn-vector-iterator-into-index
// to use the "size_t" variable to access an item in a vector: https://stackoverflow.com/questions/62675109/how-to-remove-elements-from-a-vector-that-uses-the-vector-size-in-the-for-loop

void Entity::AddObserver(Entity* observer) {
  observers.push_back(observer);
}

void Entity::RemoveObserver(Entity* observer) {
  std::vector<Entity*>::iterator it = std::find(observers.begin(), observers.end(), observer);
  if (it != observers.end()) {
    std::size_t index = std::distance(observers.begin(), it);
    observers.erase(observers.begin() + index);
  }
}

void Entity::Notify(Entity* source, std::string event) {
  // loop thru all observers and trigger their OnNotify function
  for (auto it = observers.begin(); it != observers.end(); ++it) {
    (*it)->OnNotify(source, event);
  }
}
#ifndef THREADSAFE_MAP_H
#define THREADSAFE_MAP_H
#include <vector>

template <class Key, class Value>
class ThreadSafeMap
{
private:
  std::map<Key, std::vector<Value> > map;
  std::mutex m;

public:
  void insert(Key key, Value value)
  {
    std::unique_lock<std::mutex> lock(m);
    map[key].push_back(value);
  }
  std::vector<Value> get(Key key)
  {
    std::unique_lock<std::mutex> lock(m);
    return map[key];
  }
  bool contains(Key key)
  {
    std::unique_lock<std::mutex> lock(m);
    return map.find(key) != map.end();
  }
  void erase(Key key)
  {
    std::unique_lock<std::mutex> lock(m);
    map.erase(key);
  }
};
#endif
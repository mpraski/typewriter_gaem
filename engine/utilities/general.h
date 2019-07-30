//
// Created by marcin on 7/30/19.
//

#ifndef TYPEWRITER_GAEM_GENERAL_H
#define TYPEWRITER_GAEM_GENERAL_H

#include <sstream>

class general {
public:
    template<typename T, typename ... Ts>
    static auto str(T &&t, Ts &&... ts) {
      std::ostringstream os;

      os << std::forward<T>(t);
      (void) std::initializer_list<int>{0, (void(os << std::forward<Ts>(ts)), 0)...};

      return os.str();
    }

    template<class Base, class T>
    static bool instanceof(const T *ptr) {
      return dynamic_cast<const Base *>(ptr) != nullptr;
    }
};


#endif //TYPEWRITER_GAEM_GENERAL_H

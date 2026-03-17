#pragma once
#include <cstdint>
#include <glm/fwd.hpp>
#include <string>

enum class TypeID : int32_t {
    Unsupported = 0,
    Int = 1,
    Double = 2,
    String = 3,
    GLMVec3 = 4, 
    GLMVec4 = 5, 
};

template<typename T> struct TypeToId { static constexpr TypeID Id = TypeID::Unsupported; };

template<> struct TypeToId<int> { static constexpr TypeID Id = TypeID::Int; };
template<> struct TypeToId<double> { static constexpr TypeID Id = TypeID::Double; };
template<> struct TypeToId<std::string> { static constexpr TypeID Id = TypeID::String; };
template<> struct TypeToId<glm::vec3> { static constexpr TypeID Id = TypeID::GLMVec3; };
template<> struct TypeToId<glm::vec4> { static constexpr TypeID Id = TypeID::GLMVec4; };

template<typename T>
constexpr bool is_supported_type_v = (TypeToId<T>::id != TypeID::Unsupported);
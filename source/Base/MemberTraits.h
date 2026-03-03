#pragma once

namespace CitrusCore
{
    template <typename M> struct MemberTraits;
    template <typename T, typename FieldType>
    struct MemberTraits<FieldType T::*> {
        using class_type = T;
        using field_type = FieldType;
    };
}

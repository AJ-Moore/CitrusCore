#pragma once

#include <CCCommon.h>
#include <Serialisation/ByteStream.h>

namespace CitrusCore
{
	class CITRUS_CORE_API ISerialisable {
	public:
        virtual ~ISerialisable() = default;
        virtual void Serialise(ByteStream& byteStream) = 0;
        virtual void Deserialise(ByteStream& byteStream) = 0;
	};
}
#pragma once

#include <CCCommon.h>
#include <Serialisation/ByteStream.h>
#include <Serialisation/ByteStreamReader.h>

namespace CitrusCore
{
	class CITRUS_CORE_API ISerialisable {
	public:
        virtual ~ISerialisable() = default;
        virtual void Serialise(ByteStream& byteStream) = 0;
        virtual void Deserialise(ByteStreamReader& byteStream) = 0;
	};
}
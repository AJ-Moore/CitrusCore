#pragma once

#include <CCCommon.h>
#include <Serialisation/SerialiserContext.h>
#include <Serialisation/ByteStream.h>
#include <Serialisation/ByteStreamReader.h>

namespace CitrusCore
{
	class CITRUS_CORE_API ISerialisable {
	public:
                virtual ~ISerialisable() = default;
                virtual void Serialise(StreamWriter& streamWriter) = 0;
                virtual void Deserialise(StreamReader& streamReader) = 0;
	};
}
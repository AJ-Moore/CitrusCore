#pragma once

#include <CCCommon.h>
#include <atomic>

namespace CitrusCore
{
	class CITRUS_CORE_API UID
	{
	public:
		UID();

		uint64 GetID() const { return m_uid; }

		//const UID& operator=(const UID& other) const;
		bool operator!=(const UID& rhs) const;
		bool operator==(const UID& rhs) const;

		// <,<=,>= required for sorting.
		bool operator< (const UID& rhs) const;
		bool operator<=(const UID& rhs) const;
		bool operator>=(const UID& rhs) const;

	private: 
		mutable uint64 m_uid;
		static std::atomic<uint64> m_counter;
	};
}

template <>
struct std::hash<CitrusCore::UID>
{
	std::size_t operator()(const CitrusCore::UID& k) const
	{
		return hash<uint64>()(k.GetID());
	}
};
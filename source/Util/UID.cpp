#include <Util/UID.h>

namespace CitrusCore
{
	std::atomic<uint64> UID::m_counter = 0;

	UID::UID()
	{
		m_uid = m_counter++;
	}

	//const UID& UID::operator=(const UID& other) const
	//{
	//	m_uid = other.m_uid;
	//	return *this;
	//}

	bool UID::operator!=(const UID& rhs) const
	{
		return m_uid != rhs.m_uid;
	}

	bool UID::operator==(const UID& rhs) const
	{
		return m_uid == rhs.m_uid;
	}

	bool UID::operator<(const UID& rhs) const
	{
		return m_uid < rhs.m_uid;
	}

	bool UID::operator<=(const UID& rhs) const
	{
		return m_uid <= rhs.m_uid;
	}

	inline bool UID::operator>=(const UID& rhs) const
	{
		return m_uid >= rhs.m_uid;
	}
}

#pragma once

#include <Interface/IManagedObject.h>
#include <Util/UID.h>

namespace CitrusCore
{
	/** Base object class. */
	class CITRUS_CORE_API Object
	{
	public:
		virtual ~Object(){}
		void SetName(const std::string& name) { m_name = name; }
		const std::string& GetName() const { return m_name; }
		const UID GetUID() const { return m_uid; }
	private:
		UID m_uid;
		std::string m_name = "Object";
	};
}
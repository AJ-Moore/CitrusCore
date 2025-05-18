#pragma once
#include <CCCommon.h>
#include <functional>

namespace CitrusCore
{
	template <typename... Args>
	class CITRUS_CORE_API Event
	{
	public:
		void AddListener(std::function<void(Args...)> callback) { m_listeners.push_back(callback); }

		void RemoveListener(std::function<void(Args...)> callback) {
			m_listeners.erase(std::remove(m_listeners.begin(), m_listeners.end(), callback), m_listeners.end());
		}

		void Invoke(Args... args)
		{
			for (std::function<void(Args...)>  listener : m_listeners)
			{
				listener(args...);
			}
		}
	private:
		std::vector <std::function<void(Args...)>> m_listeners;
	};
}
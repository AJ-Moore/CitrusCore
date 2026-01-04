#pragma once 
#include <CCCommon.h>
#include <vector>
#include <unordered_map>

namespace CitrusCore {
template <typename UID, typename T>
    class CITRUS_CORE_API IndexedVector
    {
    private:
        std::vector<T> m_data;
        std::unordered_map<UID, std::size_t> m_index;
    public:
        auto begin()             { return m_data.begin(); }
        auto end()               { return m_data.end(); }
        auto begin() const       { return m_data.begin(); }
        auto end() const         { return m_data.end(); }
        auto cbegin() const      { return m_data.cbegin(); }
        auto cend() const        { return m_data.cend(); }

        bool Contains(const UID& uid) const
        {
            return m_index.contains(uid);
        }

        bool Add(const UID& uid, T value)
        {
            if (m_index.contains(uid))
            {
                return false;
            }

            m_index[uid] = m_data.size();
            m_data.push_back(value);
            return true;
        }

        bool Remove(const UID& uid)
        {
            if (!m_index.contains(uid))
            {
                CitrusCore::Logger::Log(CitrusCore::Logger::WARN, "Unable to remove object");
                return false;
            }

            m_index[m_data.back()->GetUID()] = m_index.at(uid);
            std::swap(m_data[m_index.at(uid)], m_data.back());   
            m_data.pop_back();
            m_index.erase(uid);
            return true;
        }

        T& Get(const UID& uid)
        {
            return m_data.at(m_index.at(uid));
        }

        const T& Get(const UID& uid) const
        {
            return m_data.at(m_index.at(uid));
        }
    };
}
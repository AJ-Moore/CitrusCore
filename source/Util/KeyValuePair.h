#pragma once

namespace CitrusCore
{
	template <class KEY, class VALUE>
	class KeyValuePair {
	public:
		/// Constructor property key, value 
		KeyValuePair(KEY Key, VALUE Val) {
			this->key = Key;
			this->value = Val;
		}
		virtual ~KeyValuePair() {}

		/// The property key 
		KEY key;

		/// The property value
		VALUE value;

		// Used by render queue.
		bool operator<(const KeyValuePair& property) const { return value < property.value; }
	};
}

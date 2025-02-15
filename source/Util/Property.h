#pragma once

namespace CitrusCore
{
	template <class KEY, class VALUE>
	class Property {
	public:
		/// Constructor property key, value 
		Property(KEY Key, VALUE Val) {
			this->key = Key;
			this->value = Val;
		}
		~Property() {}

		/// The property key 
		KEY key;

		/// The property value
		VALUE value;

		// Used by render queue.
		bool operator<(const Property& property) const { return value < property.value; }
	};
}

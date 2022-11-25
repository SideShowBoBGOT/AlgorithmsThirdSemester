#ifndef LAB6_TCARDOBJECT_H
#define LAB6_TCARDOBJECT_H
#include "NNCardGroup.h"
#include "NNCardValue.h"
namespace Logic {
	class TCardObject {
		public:
		TCardObject() = default;
		TCardObject(NCardGroup group, NCardValue value);
		virtual ~TCardObject() = default;
		
		public:
		virtual NCardGroup Group();
		virtual NCardValue Value();
	
		protected:
		NCardGroup m_xGroup = NCardGroup::None;
		NCardValue m_xValue = NCardValue::None;
	};
}

#endif //LAB6_TCARDOBJECT_H

#include "TCardObject.h"
namespace Core {
	TCardObject::TCardObject(NCardGroup group, NCardValue value) {
		m_xGroup = group;
		m_xValue = value;
	}

	NCardGroup TCardObject::Group() { return m_xGroup; }
	NCardValue TCardObject::Value() { return m_xValue; }
}

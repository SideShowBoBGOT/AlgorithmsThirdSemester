#ifndef LAB6_NNCARDVALUE_H
#define LAB6_NNCARDVALUE_H
namespace Core {
	namespace NNCardValue {
		enum class NCardValue {
			None = 0,
			Two = 1,
			Three = 2,
			Four = 3,
			Five = 4,
			Six = 5,
			Seven = 6,
			Eight = 7,
			Nine = 8,
			Ten = 9,
			Valet = 10,
			Queen = 11,
			King = 12,
			Ace = 13,
			Size = 14
		};
	}
	typedef NNCardValue::NCardValue NCardValue;
}

#endif //LAB6_NNCARDVALUE_H

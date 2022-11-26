#ifndef LAB6_NNCARDGROUP_H
#define LAB6_NNCARDGROUP_H
namespace Core {
	namespace NNCardGroup {
		enum class NCardGroup {
			None = 0,
			Chirva = 1,
			Pika = 2,
			Buba = 3,
			Cross = 4,
			Size = 5
		};
	}

	typedef NNCardGroup::NCardGroup NCardGroup;
}
#endif //LAB6_NNCARDGROUP_H

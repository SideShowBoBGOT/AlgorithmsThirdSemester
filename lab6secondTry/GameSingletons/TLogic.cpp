//
// Created by choleraplague on 26.11.22.
//

#include "TLogic.h"

TLogic::TLogic() {
	for(auto i=0;i<NCardType::Size;++i) {
		for(auto j=0;j<NCardValue::Size;++j) {
			auto card = std::make_shared<TCard>();
			card->Type(static_cast<NCardType>(i));
			card->Value(static_cast<NCardValue>(j));
			Cards.emplace_back(std::move(card));
		}
	}
}
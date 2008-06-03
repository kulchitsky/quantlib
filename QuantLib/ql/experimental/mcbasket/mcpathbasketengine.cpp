/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2008 Andrea Odetti

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#include <ql/experimental/mcbasket/mcpathbasketengine.hpp>

namespace QuantLib {

    EuropeanPathMultiPathPricer::EuropeanPathMultiPathPricer(
                                       boost::shared_ptr<PathPayoff> & payoff,
                                       std::vector<Size> timePositions,
                                       DiscountFactor discount)
    :  payoff_(payoff), timePositions_(timePositions), discount_(discount) {}

    Real EuropeanPathMultiPathPricer::operator()(const MultiPath& multiPath)
                                                                       const {

        Size n = multiPath.pathSize();
        QL_REQUIRE(n > 0, "the path cannot be empty");

        Size numAssets = multiPath.assetNumber();
        QL_REQUIRE(numAssets > 0, "there must be some paths");

        const Size numberOfTimes = timePositions_.size();

        // calculate the final price of each asset
        Matrix finalPrice(numAssets, numberOfTimes, 0.0);

        for (Size i = 0; i < numberOfTimes; ++i) {
            const Size pos = timePositions_[i];
            for (Size j = 0; j < numAssets; j++)
                finalPrice[j][i] = multiPath[j][pos];
        }

        Real value = (*payoff_)(finalPrice);
        return value * discount_;
    }

}

//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_VERIFIABLE_HPP_INCLUDED
#define MOCK_VERIFIABLE_HPP_INCLUDED

#include <boost/noncopyable.hpp>

namespace mock
{
    class verifiable : private boost::noncopyable
    {
    public:
        verifiable() {}
        virtual ~verifiable() {}

        virtual bool verify() const = 0;

        virtual void reset() = 0;
    };
}

#endif // MOCK_VERIFIABLE_HPP_INCLUDED

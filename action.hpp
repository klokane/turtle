//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MOCK_ACTION_HPP_INCLUDED
#define MOCK_ACTION_HPP_INCLUDED

#include "lambda.hpp"
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/ref.hpp>

namespace mock
{
namespace detail
{
    template< typename Result, typename Signature >
    class action
    {
        typedef BOOST_DEDUCED_TYPENAME
            boost::function< Signature > functor_type;
        typedef BOOST_DEDUCED_TYPENAME
            boost::remove_reference<
                BOOST_DEDUCED_TYPENAME boost::remove_const< Result >::type
            >::type result_type;

        typedef lambda< Result, Signature > lambda_type;

    public:
        template< typename Value >
        void returns( Value v )
        {
            r_.reset( new result_type( v ) );
            f_ = lambda_type::make_val( boost::ref( *r_ ) );
        }
        template< typename Y >
        void returns( const boost::reference_wrapper< Y >& r )
        {
            f_ = lambda_type::make_val( r );
        }

        void calls( const functor_type& f )
        {
            if( !f )
                throw std::invalid_argument( "null functor" );
            f_ = f;
        }

        template< typename Exception >
        void throws( Exception e )
        {
            f_ = lambda_type::make_throw( e );
        }

        const functor_type& functor() const
        {
            return f_;
        }

    private:
        boost::shared_ptr< result_type > r_;
        functor_type f_;
    };

    template< typename Result, typename Signature >
    class action< Result*, Signature >
    {
        typedef BOOST_DEDUCED_TYPENAME
            boost::function< Signature > functor_type;

        typedef lambda< Result*, Signature > lambda_type;

    public:
        void returns( Result* r )
        {
            f_ = lambda_type::make_val( r );
        }
        template< typename Y >
        void returns( const boost::reference_wrapper< Y >& r )
        {
            f_ = lambda_type::make_val( r );
        }

        void calls( const functor_type& f )
        {
            if( !f )
                throw std::invalid_argument( "null functor" );
            f_ = f;
        }

        template< typename Exception >
        void throws( Exception e )
        {
            f_ = lambda_type::make_throw( e );
        }

        const functor_type& functor() const
        {
            return f_;
        }

    private:
        functor_type f_;
    };

    template< typename Signature >
    class action< void, Signature >
    {
        typedef BOOST_DEDUCED_TYPENAME
            boost::function< Signature > functor_type;

        typedef lambda< void, Signature > lambda_type;

    public:
        action()
            : f_( lambda_type::make_nothing() )
        {}

        void calls( const functor_type& f )
        {
            if( !f )
                throw std::invalid_argument( "null functor" );
            f_ = f;
        }

        template< typename Exception >
        void throws( Exception e )
        {
            f_ = lambda_type::make_throw( e );
        }

        const functor_type& functor() const
        {
            return f_;
        }

    private:
        functor_type f_;
    };

    template< typename Result, typename Signature >
    class action< std::auto_ptr< Result >, Signature >
    {
        typedef BOOST_DEDUCED_TYPENAME
            boost::function< Signature > functor_type;

        typedef lambda< std::auto_ptr< Result >, Signature > lambda_type;

    public:
        action()
            : r_()
            , f_()
        {}
        action( const action& rhs )
            : r_( const_cast< action& >( rhs ).r_.release() )
            , f_( r_.get() ? lambda_type::make_val( boost::ref( r_ ) ) : rhs.f_ )
        {}

        template< typename Value >
        void returns( Value v )
        {
            set( v );
        }

        void calls( const functor_type& f )
        {
            if( !f )
                throw std::invalid_argument( "null functor" );
            f_ = f;
        }

        template< typename Exception >
        void throws( Exception e )
        {
            f_ = lambda_type::make_throw( e );
            r_.reset();
        }

        const functor_type& functor() const
        {
            return f_;
        }

    private:
        template< typename Y >
        void set( std::auto_ptr< Y > r )
        {
            r_ = r;
            f_ = lambda_type::make_val( boost::ref( r_ ) );
        }
        template< typename Y >
        void set( const boost::reference_wrapper< Y >& r )
        {
            f_ = lambda_type::make_val( r );
            r_.reset();
        }
        template< typename Y >
        void set( Y* r )
        {
            r_.reset( r );
            f_ = lambda_type::make_val( boost::ref( r_ ) );
        }

        mutable std::auto_ptr< Result > r_;
        functor_type f_;
    };

}
}

#endif // MOCK_ACTION_HPP_INCLUDED

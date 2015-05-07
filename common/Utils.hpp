/* 
 * File:   Utils.hpp
 * Author: asalazar
 *
 * Created on May 5, 2015, 7:36 PM
 */

#ifndef UTILS_HPP
#define	UTILS_HPP

#include<sstream>

/**
 * Usage:
 * 
 * int i = 42;
 * std::cout << SSTR( "i is: " << i );
 * std::string s = SSTR( i );
 * puts( SSTR( i ).c_str() );
 * 
 */
#define SSTR( x ) dynamic_cast< std::ostringstream & >(( std::ostringstream() << std::dec << x ) ).str()

class Utils {
public:
    Utils(const Utils& orig);
    virtual ~Utils();
private:
    Utils();
};

/* Constructors */
Utils::Utils()
{
    // No op
}

Utils::Utils(const Utils& orig)
{
    // No op
}

/* Public methods */

/* Destructor */
Utils::~Utils()
{
    // No op
}
#endif	/* UTILS_HPP */


/*
 * basicvector.hpp
 *
 *  Created on: 2015.01.02.
 *      Author: moravas
 */

#ifndef BASICVECTOR_HPP_
#define BASICVECTOR_HPP_

template<typename t, unsigned int size> class basicVector {
public: t container[size];
public: unsigned int len;
};

#endif /* BASICVECTOR_HPP_ */

// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			27.03.2015.
//! \note
// =============================================================================
#ifndef _SINGLETONFACTORY_HPP_
#define _SINGLETONFACTORY_HPP_

#include "projectconfiguration.hpp"

namespace libs {

template<typename T> class SingletonFactory: public T {
public: static inline SingletonFactory* get();

private: inline SingletonFactory();

private: static SingletonFactory* _instance;
};

// =============================================================================
// Inline method implementation
// =============================================================================

template<typename T> SingletonFactory<T>* SingletonFactory<T>::_instance = NULL;

template<typename T> inline SingletonFactory<T>* SingletonFactory<T>::get() {
	if(!_instance) {
		xSemaphoreHandle sync = NULL;
		if(!sync) {
			sync = xSemaphoreCreateMutex();
		}

		 xSemaphoreTake(sync, portMAX_DELAY);
		if(!_instance) {
			_instance = new SingletonFactory<T>();
		}

		xSemaphoreGive(sync);
	}

	return _instance;
}

template<typename T> inline SingletonFactory<T>::SingletonFactory() {

}

} // libs

#endif // _SINGLETONFACTORY_HPP_
// =============================================================================
//! \file
//! \copyright
// ====================== end of file: singletonfactory.hpp ====================

// =============================================================================
//! \file
//! \brief
//! \author         Norbert Toth
//! \date			27.03.2015.
//! \note
// =============================================================================
#ifndef _TASKFACTORY_HPP_
#define _TASKFACTORY_HPP_

#include "singletonfactory.hpp"
#include "projectconfiguration.hpp"

namespace libs {

template<typename T> class TaskFactory: public T {
public: static inline void start(void* args);

private: inline TaskFactory();
};

// =============================================================================
// Inline method implementation
// =============================================================================

template<typename T> inline void TaskFactory<T>::start(void* args) {
	T::get()->task(args);
	UARTprintf("Task returned from infinte loop\n");
}

template<typename T> inline TaskFactory<T>::TaskFactory() {

}

} // libs

#endif // _TASKFACTORY_HPP_
// =============================================================================
//! \file
//! \copyright
// ========================= end of file: taskfactory.hpp ======================

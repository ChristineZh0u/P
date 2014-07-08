/**
* \file PrtConfig.h
* \brief Defines available runtime configurations.
* The runtime can be configured to compile on different platforms
* and with different concurrency models.
*
* 1. **Platform** - Set `PRT_PLAT` to one of the following values:
*    + `PRT_PLAT_WINUSER` - Specializes Prt to run in Windows user mode.
*    + `PRT_PLAT_WINKMDF` - Specializes Prt to run in Windows kernel mode (KMDF).
*	 + `PRT_PLAT_WINDIST` - Specialized the Prt to run in windows user mode in a distributed setting, RPC protocol is used for communication across machines.

*
*
* 2. **Architecture** - Set `PRT_ARCH` to one of the following values:
*    + `PRT_ARCH_X86` - Specializes Prt to run on a 32-bit x86 architecture
*    + `PRT_ARCH_X64` - Specializes Prt to run on a 64-bit AMD architecture
*
* 3. **Execution** - Set `PRT_EXEC` to one of the following values:
*    + `PRT_EXEC_REACTIVE` - Prt processes only react when inputs are enqueued. 
*       Enqueues are blocking until reaction is complete.
*    + `PRT_EXEC_PROACTIVE` - Prt processes proactively execute on their own threads.
*       Enqueues are non-blocking.
*
* 4. **Distribution** - Set `PRT_DISTR` to one of the following values:
*    + `PRT_DISTR_NONE` - Prt runtime is not distributed. Instances of the runtime
*       cannot communicate across process boundaries.
*    + `PRT_DISTR_CM` - Prt runtime is distributed across many process with one central
*       manager. Processes connected to the same central manager can interact. 
*/

#ifndef PRTCONFIG_H
#define PRTCONFIG_H

/** Build runtime in Windows user mode. */
#define PRT_PLAT_WINUSER 1
/** Build runtime in Windows kernel mode (KMDF). */
#define PRT_PLAT_WINKMDF 2
/** Build runtime in Windows user mode for distributed setting using RPC */
#define PRT_PLAT_WINDIST 3

/** Build runtime for 32-bit x86 architecture */
#define PRT_ARCH_X86 4
/** Build runtime for 64 - bit AMD architecture */
#define PRT_ARCH_X64 5

/** Build runtime with reactive scheduling. */
#define PRT_EXEC_REACTIVE 6
/** Build runtime with proactive scheduling. */
#define PRT_EXEC_PROACTIVE 7

/** Build runtime without distribution */
#define PRT_DISTR_NONE 8
/** Build runtime with distribution using a central manager. */
#define PRT_DISTR_CM 9

/**
* \def PRT_DEBUG
* Uses debug versions of some libraries and performs extra run-time checks.
*/

/**
* \def PRT_PLAT
* Sets the target platform. Default is `PRT_PLAT_WINUSER`
*/
#ifndef PRT_PLAT
#define PRT_PLAT PRT_PLAT_WINDIST
#endif

/**
* \def PRT_ARCH
* Sets the target architecture. Default is `PRT_ARCH_X86`
*/
#ifndef PRT_ARCH
#define PRT_ARCH PRT_ARCH_X86
#endif

/**
* \def PRT_EXEC
* Sets the execution model. Default is `PRT_EXEC_REACTIVE`
*/
#ifndef PRT_EXEC
#define PRT_EXEC PRT_EXEC_REACTIVE
#endif

/**
* \def PRT_DISTR
* Sets the distribution model. `Default is PRT_DISTR_NONE`
*/
#ifndef PRT_DISTR
#define PRT_DISTR PRT_DISTR_NONE
#endif

#ifndef PRT_DEBUG
/**
* Expands to PrtAssert if PRT_DEBUG is defined.
*/
#define PRT_DBG_ASSERT(condition, message) 

/**
* Marks the beginning of a region of code that may allocate memory,
* but will deallocate all such memory by the end of the region.
* @see PRT_DBG_END_MEM_BALANCED_REGION
*/
#define PRT_DBG_START_MEM_BALANCED_REGION

/**
* Marks the end of a region of code that may have allocated memory,
* but must have deallocate all such memory be now. An exception is thrown
* if memory was not returned to the starting state.
* @see PRT_DBG_START_MEM_BALANCED_REGION
*/
#define PRT_DBG_END_MEM_BALANCED_REGION
#endif

#if !defined(DISTRIBUTED_RUNTIME)

#endif

#ifndef PRT_PLAT
  #error No platform was specified (see PrtConfig.h for details)
#elif PRT_PLAT == PRT_PLAT_WINUSER
  #include "PrtConfigWinUser.h"
#elif PRT_PLAT == PRT_PLAT_WINKMDF
  #include "PrtConfigWinKMDF.h"
#elif PRT_PLAT == PRT_PLAT_WINDIST
  #include "PrtConfigWinDist.h"
#else
  #error An invalid platform was specified (see PrtConfig.h for details)
#endif

#endif
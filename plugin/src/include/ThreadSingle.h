/*
 *  Copyright (c) 2011 GoCast. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. All contributing project authors may be found in the 
 *  AUTHORS file in the root of the source tree.
 */

//!
//! \file ThreadSingle.h
//!
//! \brief Base class for threaded support. Only requires pthread.h and linking to -lpthreads
//!
//! \author Bob Wolff (rwolff)
//! \date Created on: Jun 10, 2010
//!

#ifndef THREADSINGLE_H_
#define THREADSINGLE_H_

#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <errno.h>

#ifdef WIN32
#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#endif
#include <winsock2.h>
//#pragma comment(lib, "ws2_32.lib")
#else
#include <pthread.h>
#endif

//!
//! \def REPORT_ERROR(a,b) is used to give feedback on true error conditions. Currently this is done via
//!      a simple printf(). In the future this needs to be made into a more formal throw of an exception.
//!
//! \todo REPORT_ERROR furthermore does not allow for flexible use of variable args lists currently. To change
//!       this would be quite helpful as well.
//!
#ifdef REPORT_ERROR
#undef REPORT_ERROR
#endif
//#if defined(__APPLE__) & defined(__MACH__)
//#define REPORT_ERROR(a,b) (NSLog(@a,b))
//#else
#define REPORT_ERROR(a,b) printf(a,b)
//#endif

//!
//! \class ThreadSingle
//!
//! \brief Base class to be inherited for spawning a single workerBee() pure virtual.
//!        For multiple threads, use ThreadMultiple instead.
//!
//! Details: Simply inherit this class and create your own workerBee() function in the parent.
//!   Then, use startThread() to start the new thread which will call your workerBee from the newly
//!   created thread. When it is time to shutdown, simply call stopThread() and when it returns, you're done.
//!   \note It is important to note that your implementation of workerBee() must be looking for the isStopRequested
//!         flag to signal it to return() - otherwise the thread class will not end nicely and will crash upon
//!         a change of scope and stack.
//!   \par
//!   Also, there is a built in single mutex created at instantiation time. Parent may pthread_mutex_lock(&mutex) and
//!         then can pthread_mutex_unlock(&mutex) or even using signal/wait actions as well.
//!
//! \todo Clean up error reporting by using throw of a new type of exception - gui and console and logging automatically.
//!
class ThreadSingle {
public:
	ThreadSingle()
    {
        int ret = 0;
        isRunning = false;
        isStopRequested = false;

#ifdef WIN32
        InitializeCriticalSection(&mutex);
#else
        if (pthread_mutex_init(&mutex, NULL))
        {
            assert(false);
            REPORT_ERROR("mutex init failed - err:%d", ret);
        }
#endif
    }

	~ThreadSingle()
	{
	  stopThread();
#ifdef WIN32
      DeleteCriticalSection(&mutex);
#else
	  pthread_mutex_destroy(&mutex);
#endif
	}

    //!
    //! \brief Parent calls startThread() when they are ready to launch/fork/spawn the new thread.
    //!
    //! Once this function returns, the pthread_create() within the function will cause workerBee() to
    //! be called. The parent must implement workerBee() for its work item(s) and/or calls outwards.
    //!
    //! NOTE: isStopRequested is a variable which must be monitored by workerBee() to know when it is
    //!       being told to stop (return).
    //!
	void startThread()
	{
        int ret;
        assert(!isRunning);
        assert(!isStopRequested);
        // Now set to running and create the thread -- and in an error condition, set to 'NOT running' again.
        isRunning = true;
#ifdef WIN32
        thread = CreateThread(NULL, 0, ThreadSingle::threadLaunchpoint, this, 0, NULL);
        ret = WSAGetLastError();
        if (thread == NULL)
#else
        ret = pthread_create(&thread, 0, &(ThreadSingle::threadLaunchpoint), this);
        if (ret)
#endif
        {
            isRunning = false;
            assert(false);
            REPORT_ERROR("thread creation failed. Err:%d", ret);
        }
	}
    

    //!
    //! \brief Parent calls stopThread() when they are ready to close/stop the thread.
    //!
    //! Once this function returns, the thread is no longer active. It has been destroyed.
    //! The parent, in its implementation of workerBee() must monitor 'isStopRequested'
    //! or it will never know when to quit itself via a return() call.
    //!
    void stopThread()
	{
        int ret;
        if (isRunning)
        {
            // Set the stop requested flag, stop the thread, and then clear the isRunning flag.
            isStopRequested = true;
#ifdef WIN32
            ret = CloseHandle(thread);
            if (!ret)
#else
            ret = pthread_join(thread, 0);
            if (/*ret != EDEADLK &&*/ ret)
#endif
            {
                assert(false);
                REPORT_ERROR("ending/joining thread failed. Err:%d", ret);
            }
            isRunning = false;
            isStopRequested = false;
        }
	}

    bool IsRunning(void) const
    {
        return isRunning;
    }

    bool IsStopRequested(void) const
    {
        return isStopRequested;
    }

private:
#ifdef WIN32
    HANDLE thread;
#else
	pthread_t thread;
#endif
	volatile bool isRunning;

#ifdef WIN32
    static DWORD WINAPI threadLaunchpoint(LPVOID pData) 
#else
	static void* threadLaunchpoint(void* pData)
#endif
	{
	  ThreadSingle *pthrd;

	  pthrd = (ThreadSingle*)pData;
	  pthrd->workerBee();
#ifdef WIN32
      return 0;
#else
	  return (void*)NULL;
#endif
	}

protected:
        //! A mutex for communication/sync.
#ifdef WIN32
        CRITICAL_SECTION mutex;
#else
        pthread_mutex_t mutex;
#endif
        //! A single flag for the thread involved to indicate that a kill has been requested.
        //! Worker bee function must listen/watch for this to happen.
        volatile bool isStopRequested;
        //!
        //! \brief Parent implements this pure virtual in its class.
        //!
        //! This is where the new thread has its entry point. All work is done here (or in subsequent calls)
        //! to other functions. The biggest items to note are 'be kind - no tight loops for long periods' and
        //! also to monitor and adhere quickly to 'isStopRequested' flag by calling return().
        //!
        //! \a NOTE: isStopRequested is a variable which must be monitored by workerBee() to know when it is
        //!       being told to stop (return).
        //!
        virtual int workerBee()=0;
};

#endif /* THREADSINGLE_H_ */

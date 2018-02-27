#ifndef STATE_H
#define STATE_H

#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/vfs.h>

#include "ptracer.hpp"
#include "valueMapper.hpp"
#include "systemCall.hpp"

using namespace std;

// Needed to avoid recursive dependencies between classes.
class systemCall;

/**
 * Class to hold all state that we will need to update in between system calls inside the
 * tracer so far this includes:
 * Inodes Mappings.
 * PID Mappings.
 * Logical Clocks.
 * Logger.
 */
class state{
private:
  /**
   * Logical clock. Ticks only on time related system calls where the user can observe
   * time since we want to present progress.
   * See [[https://github.com/upenn-acg/detTrace/issues/24][Github issue]] for more
   * information.
   */
  size_t clock = 0;
public:
  /**
   * @pidMap: Notice this is a reference -> same map is shared among all instances of
   * of state.
   * @ppid: Parent pid of this process.
   */
  state(logger& log, pid_t myPid);

  /**
   * The pid of the process represented by this state.
   */
  pid_t traceePid;

  /*
   * Isomorphism between inodes and vitual inodes.
   */
  valueMapper inodeMap;
  logger log;

  bool doSystemcall;

  /*
   * It's our job to keep track whether we are on a system call pre or post.
   */
  syscallState syscallStopState = syscallState::pre;
  
/*
 * Insn pointer from predet
 */
  uint64_t preIp = 0;

  /*
   * We need to know what system call was/is that we are not. This is important in
   * cases like clone:
   * 1) Parent performs clone.
   * 2) We receive a pre-exit for clone.
   * 3) The process is switched to the child.
   * 4) Child does an arbitrary number of system calls before exiting.
   * 5) Without this variable the fact we were doing a clone is lost.

   * Basically, we cannot guarantee we will always be able to do system calls in a
   * pre-post pairs. As we extend to multi processes, this will become more useful.

   * We use a a pointer since we rely on dynamic dispatch for the right subclass for
   * the methods to work properly.
   */
  unique_ptr<systemCall> systemcall;

  /**
   * Increase value of internal logical clock.
   */
  void incrementTime();

  /**
   * Get value of internal logical clock.
   */
  int getLogicalTime();
};

#endif

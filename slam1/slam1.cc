#include <chrono>
#include <future>
#include <iostream>
#include <thread>
#include "common/common.hh"

using namespace ILLIXR;

class slam1 : public abstract_slam {
public:
	slam1() {
		/* You can do anything (processes, threads, network). Most
		   SLAMs already manage their own mainloop. That would go
		   here. */
		_m_current_pose.data[0] = 0;
		_m_current_pose.data[1] = 0;
		_m_current_pose.data[2] = 0;
		_m_thread = std::thread{&slam1::main_loop, this};
	}
	void main_loop() {
		while (!_m_terminate.load()) {
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(200ms);
			_m_current_pose.data[0] += 1;

			/*
			  1. look at cam_frame (already fed in)
			  2. look at accel (already fed in)
			  3. do computation (could go to network here)
			  4. write to _m_current_pose
			*/
		}
	}
private:
	pose _m_current_pose;
	std::thread _m_thread;
	std::atomic<bool> _m_terminate {false};

public:
	/* compatibility interface */
	virtual void feed_cam_frame_nonbl(cam_frame&) override { }
	virtual void feed_accel_nonbl(accel&) override { }
	virtual ~slam1() override {
		_m_terminate.store(true);
		_m_thread.join();
		/*
		  This developer is responsible for killing their processes
		  and deallocating their resources here.
		*/
	}
	virtual pose& produce_nonbl() override {
		return _m_current_pose;
	}
};

ILLIXR_make_dynamic_factory(abstract_slam, slam1)
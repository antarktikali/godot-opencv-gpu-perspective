#ifndef OPENCVPERSPECTIVE_H
#define OPENCVPERSPECTIVE_H

#include <opencv2/gpu/gpu.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <pthread.h>
#include "reference.h"

class OpenCVPerspective : public Reference
{
	OBJ_TYPE(OpenCVPerspective, Reference);

	protected:
		static void _bind_methods();
	public:
		OpenCVPerspective();
		~OpenCVPerspective();
		// Starts capturing from the webcam and tracks the face position
		// returns false if the webcam could not be opened.
		//
		// Also starts another thread for processing the video input.
		bool start();
		// Stops the thread which was started, together with the webcam capture.
		void stop();
		// Returns a float value between -1.0 and 1.0 depending on the
		// horizontal position of the face. 0.0 is the middle of the screen,
		// which is also the default value if no faces were detected.
		float getHorizontalOffset();
		// Same as above, for the vertical axis.
		float getVerticalOffset();
	private:
		static void* startMainLoopThread(void*);
		pthread_t main_loop;
		void* mainLoop();
		// Processes the given array and returns the position of the face.
		cv::Rect detectFace(cv::Mat);
		// Flag for terminating the loop in the pthread.
		bool is_running;
		float offset_x;
		float offset_y;
		cv::gpu::CascadeClassifier_GPU cascade_gpu;
		cv::Mat frame;
		cv::VideoCapture* capture;
		cv::Rect face_rect;
};

#endif // OPENCVPERSPECTIVE_H

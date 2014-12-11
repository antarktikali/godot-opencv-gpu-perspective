#include "OpenCVPerspective.h"

OpenCVPerspective::OpenCVPerspective()
{
	this->is_running = false;
	this->offset_x = 0.0;
	this->offset_y = 0.0;
	// TODO
	// Find a way to access the resources directory of a Godot project
	this->cascade_gpu.load("haarcascade_frontalface_alt2.xml");
}

OpenCVPerspective::~OpenCVPerspective()
{
	this->stop();
}

bool OpenCVPerspective::start()
{
	this->capture = new cv::VideoCapture(0);
	if(!this->capture->isOpened())
	{
		return false;
	}
	this->is_running = true;
	pthread_create(&(this->main_loop), NULL,
							&(OpenCVPerspective::startMainLoopThread), this);
	
	return true;
}

void OpenCVPerspective::stop()
{
	this->is_running = false;
	pthread_join(this->main_loop, NULL);
	delete this->capture;
}

float OpenCVPerspective::getHorizontalOffset()
{
	return this->offset_x;
}

float OpenCVPerspective::getVerticalOffset()
{
	return this->offset_y;
}

void* OpenCVPerspective::startMainLoopThread(void* object_)
{
	OpenCVPerspective* object = reinterpret_cast<OpenCVPerspective*>(object_);
    return object->mainLoop();
}

void* OpenCVPerspective::mainLoop()
{
	while(this->is_running)
	{
		*(this->capture) >> this->frame;
		this->face_rect = detectFace(this->frame);
		this->offset_x = static_cast<float>( (this->face_rect.x+(this->face_rect.width/2))-(this->frame.cols/2) ) / (this->frame.cols/2.0);
		this->offset_y = static_cast<float>( (this->face_rect.y+(this->face_rect.height/2))-(this->frame.rows/2) ) / (this->frame.rows/2.0);
	}
	return NULL;
}

cv::Rect OpenCVPerspective::detectFace(cv::Mat frame_)
{
	// Return the middle of the screen by default
	cv::Rect face_position(this->frame.cols/2 - 5, this->frame.rows/2 - 5,
																	10, 10);

	cv::Mat faces_downloaded;
	cv::gpu::GpuMat faces_buf;
	
	// Convert the frame to grayscale
	cv::Mat frame_grayscale(frame_.size(), CV_8UC1);
	cv::cvtColor(frame_, frame_grayscale, CV_BGR2GRAY);
	// Convert the grayscale frame to a GPU compatible array
	cv::gpu::GpuMat frame_grayscale_gpu(frame_grayscale);

	cascade_gpu.findLargestObject = true;
	int number_of_faces = cascade_gpu.detectMultiScale(frame_grayscale_gpu,
																	faces_buf);

	// If no faces were found, just return the default position
	if(0 == number_of_faces)
	{
		return face_position;
	}

	faces_buf.colRange(0, number_of_faces).download(faces_downloaded);
	cv::Rect *face_positions = faces_downloaded.ptr<cv::Rect>();
	// Only get the first detected face, ignore everything else
	face_position = face_positions[0];
	
	return face_position;
}

void OpenCVPerspective::_bind_methods()
{
	ObjectTypeDB::bind_method("start", &OpenCVPerspective::start);
	ObjectTypeDB::bind_method("stop", &OpenCVPerspective::stop);
	ObjectTypeDB::bind_method("get_horizontal_offset",
								&OpenCVPerspective::getHorizontalOffset);
	ObjectTypeDB::bind_method("get_vertical_offset",
								&OpenCVPerspective::getVerticalOffset);
}


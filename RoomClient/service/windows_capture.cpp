/**
 * This file is part of mediasoup_client project.
 * Author:    Jackie Ou
 * Created:   2021-11-01
 **/
#pragma warning(disable:4996)
#include "windows_capture.h"
#include <stdint.h>
#include <memory>
#include "rtc_base/checks.h"
#include "rtc_base/logging.h"
#include "api/video/i420_buffer.h"
#include "api/video/video_frame_buffer.h"
#include "api/video/video_rotation.h"
//#include "logger/u_logger.h"

namespace vi {

	using namespace webrtc;

	SimpleVideoCapturer::~SimpleVideoCapturer() = default;

	void SimpleVideoCapturer::OnFrame(const VideoFrame & original_frame) {
		int cropped_width = 0;
		int cropped_height = 0;
		int out_width = 0;
		int out_height = 0;

		VideoFrame frame = MaybePreprocess(original_frame);

		if (!video_adapter_.AdaptFrameResolution(
			frame.width(), frame.height(), frame.timestamp_us() * 1000,
			&cropped_width, &cropped_height, &out_width, &out_height)) {
			// Drop frame in order to respect frame rate constraint.
			return;
		}

		if (out_height != frame.height() || out_width != frame.width()) {
			// Video adapter has requested a down-scale. Allocate a new buffer and
			// return scaled version.
			// For simplicity, only scale here without cropping.
			rtc::scoped_refptr<I420Buffer> scaled_buffer =
				I420Buffer::Create(out_width, out_height);
			scaled_buffer->ScaleFrom(*frame.video_frame_buffer()->ToI420());
			VideoFrame::Builder new_frame_builder =
				VideoFrame::Builder()
				.set_video_frame_buffer(scaled_buffer)
				.set_rotation(kVideoRotation_0)
				.set_timestamp_us(frame.timestamp_us())
				.set_id(frame.id());
			if (frame.has_update_rect()) {
				VideoFrame::UpdateRect new_rect = frame.update_rect().ScaleWithFrame(
					frame.width(), frame.height(), 0, 0, frame.width(), frame.height(),
					out_width, out_height);
				new_frame_builder.set_update_rect(new_rect);
			}
			broadcaster_.OnFrame(new_frame_builder.build());

		}
		else {
			// No adaptations needed, just return the frame as is.
			broadcaster_.OnFrame(frame);
		}
	}

	rtc::VideoSinkWants SimpleVideoCapturer::GetSinkWants() {
		return broadcaster_.wants();
	}

	void SimpleVideoCapturer::AddOrUpdateSink(
		rtc::VideoSinkInterface<VideoFrame>*sink,
		const rtc::VideoSinkWants & wants) {
		broadcaster_.AddOrUpdateSink(sink, wants);
		UpdateVideoAdapter();
	}

	void SimpleVideoCapturer::RemoveSink(rtc::VideoSinkInterface<VideoFrame>*sink) {
		broadcaster_.RemoveSink(sink);
		UpdateVideoAdapter();
	}

	void SimpleVideoCapturer::UpdateVideoAdapter() {
		video_adapter_.OnSinkWants(broadcaster_.wants());
	}

	VideoFrame SimpleVideoCapturer::MaybePreprocess(const VideoFrame & frame) {
		MutexLock lock(&lock_);
		if (preprocessor_ != nullptr) {
			return preprocessor_->Preprocess(frame);
		}
		else {
			return frame;
		}
	}

	VcmCapturer::VcmCapturer(rtc::Thread* thread)
		: vcm_(nullptr)
		, thread_(thread)
	{

	}

	bool VcmCapturer::Init(size_t width,
		size_t height,
		size_t target_fps,
		size_t capture_device_index,
		rtc::VideoSinkInterface<VideoFrame>* sink) {
		std::unique_ptr<VideoCaptureModule::DeviceInfo> device_info(
			VideoCaptureFactory::CreateDeviceInfo());

		char device_name[256];
		char unique_name[256];
		if (device_info->GetDeviceName(static_cast<uint32_t>(capture_device_index),
			device_name, sizeof(device_name), unique_name,
			sizeof(unique_name)) != 0) {
			Destroy();
			return false;
		}

		vcm_ = thread_->BlockingCall(std::bind(&VcmCapturer::_createDevice, this, unique_name));
		if (!vcm_) {
			return false;
		}
		vcm_->RegisterCaptureDataCallback(sink);

		device_info->GetCapability(vcm_->CurrentDeviceName(), 0, capability_);

		capability_.width = static_cast<int32_t>(width);
		capability_.height = static_cast<int32_t>(height);
		capability_.maxFPS = static_cast<int32_t>(target_fps);
		capability_.videoType = VideoType::kI420;

		if (thread_->BlockingCall(std::bind(&VcmCapturer::_startCapture, this)) != 0) {
			Destroy();
			return false;
		}

		return true;
	}

	VcmCapturer* VcmCapturer::Create(size_t width,
		size_t height,
		size_t target_fps,
		size_t capture_device_index,
		rtc::VideoSinkInterface<VideoFrame>* sink,
		rtc::Thread* thread) {
		std::unique_ptr<VcmCapturer> vcm_capturer(new VcmCapturer(thread));
		if (!vcm_capturer->Init(width, height, target_fps, capture_device_index, sink)) {
			RTC_LOG(LS_WARNING) << "Failed to create VcmCapturer(w = " << width
				<< ", h = " << height << ", fps = " << target_fps
				<< ")";
			return nullptr;
		}
		return vcm_capturer.release();
	}

	void VcmCapturer::Destroy() {
		if (!vcm_)
			return;

		if (thread_->IsCurrent()) {
			_stopCapture();
		}
		else {
			thread_->BlockingCall(std::bind(&VcmCapturer::_stopCapture, this));
		}

		vcm_->DeRegisterCaptureDataCallback(); 

		// Release reference to VCM.
		if (thread_->IsCurrent()) {
			_release();
		}
		else {
			thread_->BlockingCall(std::bind(&VcmCapturer::_release, this));
		}
	}

	VcmCapturer::~VcmCapturer() {
		Destroy();
	}
	
	int32_t VcmCapturer::start() {
		return thread_->BlockingCall(std::bind(&VcmCapturer::_startCapture, this));
	}

	int32_t VcmCapturer::stop() {
		return thread_->BlockingCall(std::bind(&VcmCapturer::_stopCapture, this));
	}

	rtc::scoped_refptr<webrtc::VideoCaptureModule> VcmCapturer::_createDevice(const char* uniqueID) {
		return webrtc::VideoCaptureFactory::Create(uniqueID);
	}

	int32_t VcmCapturer::_startCapture() {
		return vcm_->StartCapture(capability_);
	}

	int32_t VcmCapturer::_stopCapture() {
		return vcm_->StopCapture();
	}

	void  VcmCapturer::_release() {
		vcm_ = nullptr;
	}
}

/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2022, Google Inc.
 *
 * fc_queue.h - IPA Frame context queue
 */

#pragma once

#include <array>

#include <libcamera/base/log.h>

#include <libcamera/request.h>

namespace libcamera {

LOG_DECLARE_CATEGORY(FCQueue)

namespace ipa {

/*
 * Maximum number of frame contexts to be held onto
 *
 * \todo Should be platform-specific and match the pipeline depth
 */
static constexpr uint32_t kMaxFrameContexts = 16;

template<typename FrameContext>
class FCQueue : private std::array<FrameContext, kMaxFrameContexts>
{
private:
	void initialise(FrameContext &frameContext, const uint32_t frame)
	{
		frameContext = {};
		frameContext.frame = frame;
	}

public:
	void clear()
	{
		this->fill({});
	}

	FrameContext &initialise(const uint32_t frame)
	{
		FrameContext &frameContext = this->at(frame & kMaxFrameContexts);

		/*
		 * Do not re-initialise if a get() call has already fetched this
		 * frame context to preseve the error flags already raised.
		 *
		 * \todo If the the sequence number of the context to initialise
		 * is smaller than the sequence number of the queue slot to use,
		 * it means that we had a serious request underrun and more
		 * frames than the queue size has been produced since the last
		 * time the application has queued a request. Does this deserve
		 * an error condition ?
		 */
		if (frame != 0 && frame <= frameContext.frame)
			LOG(FCQueue, Warning)
				<< "Frame " << frame << " already initialised";
		else
			initialise(frameContext, frame);

		return frameContext;
	}

	FrameContext &get(uint32_t frame)
	{
		FrameContext &frameContext = this->at(frame & kMaxFrameContexts);

		/*
		 * If the IPA algorithms try to access a frame context slot which
		 * has been already overwritten by a newer context, it means the
		 * frame context queue has overflowed and the desired context
		 * has been forever lost. The pipeline handler shall avoid
		 * queueing more requests to the IPA than the frame context
		 * queue size.
		 */
		if (frame < frameContext.frame)
			LOG(FCQueue, Fatal)
				<< "Frame " << frame << " has been overwritten";

		if (frame == frameContext.frame)
			return frameContext;

		LOG(FCQueue, Warning)
			<< "Obtained an uninitialised FrameContext for " << frame;

		initialise(frameContext, frame);

		/*
		 * The frame context has been retrieved before it was
		 * initialised through the initialise() call. This indicates an
		 * algorithm attempted to access a Frame context before it was
		 * queued to the IPA.
		 *
		 * Controls applied for this request may be left unhandled.
		 */
		frameContext.error |= Request::PFCError;

		return frameContext;
	}
};

} /* namespace ipa */

} /* namespace libcamera */

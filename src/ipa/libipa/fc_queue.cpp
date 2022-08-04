/* SPDX-License-Identifier: LGPL-2.1-or-later */
/*
 * Copyright (C) 2022, Google Inc.
 *
 * fc_queue.cpp - IPA Frame context queue
 */

#include "fc_queue.h"

#include <libcamera/base/log.h>

namespace libcamera {

LOG_DEFINE_CATEGORY(FCQueue)

namespace ipa {

/**
 * \file fc_queue.h
 * \brief Queue to access per-frame Context
 */

/**
 * \class FCQueue
 * \brief A support class for queueing FrameContext instances through the IPA
 * \tparam FrameContext The IPA specific FrameContext derived class type
 *
 * The frame context queue provides a simple circular buffer implementation to
 * store IPA specific context for each frame through its lifetime within the
 * IPA.
 *
 * FrameContext instances are expected to be referenced by a monotonically
 * increasing sequence count corresponding to a frame sequence number.
 *
 * A FrameContext is initialised for a given frame when the corresponding
 * Request is first queued into the IPA. From that point on the FrameContext can
 * be obtained by the IPA and its algorithms by referencing it from the frame
 * sequence number.
 *
 * A frame sequence number from the image source must correspond to the request
 * sequence number for this implementation to be supported in an IPA. It is
 * expected that the same sequence number will be used to reference the context
 * of the Frame from the point of queueing the request, specifying controls for
 * a given frame, and processing of any ISP related controls and statistics for
 * the same corresponding image.
 *
 * IPA specific frame context implementations shall inherit from the
 * IPAFrameContext base class to support the minimum required features for a
 * FrameContext, including the frame number and the error flags that relate to
 * the frame.
 *
 * FrameContexts are overwritten when they are recycled and re-initialised by
 * the first access made on them by either initialise(frame) or get(frame). It
 * is required that the number of available slots in the frame context queue is
 * larger or equal to the maximum number of in-flight requests a pipeline can
 * handle to avoid overwriting frame context instances that still have to be
 * processed.
 *
 * In the case an application does not queue requests to the Camera fast
 * enough, frames can be produced and processed by the IPA without a
 * corresponding Request being queued. In this case the IPA algorithm
 * will try to access the FrameContext with a call to get() before it
 * had been initialized at queueRequest() time. In this case there
 * is now way the controls associated with the late Request could be
 * applied in time, as the frame as already been processed by the IPA,
 * the PFCError flag is automatically raised on the FrameContext.
 */

/**
 * \fn FCQueue::clear()
 * \brief Clear the context queue
 *
 * Reset the queue and ensure all FrameContext slots are re-initialised.
 * IPA modules are expected to clear the frame context queue at the beginning of
 * a new streaming session, in IPAModule::start().
 */

/**
 * \fn FCQueue::initialise(uint32_t frame)
 * \brief Initialize and return the Frame Context at slot specified by \a frame
 * \param[in] frame The frame context sequence number
 *
 * The first call to obtain a FrameContext from the FCQueue should be handled
 * through this call. The FrameContext will be initialised for the frame and
 * returned to the caller if it was not already initialised.
 *
 * If the FrameContext was already initialized for this sequence, a warning will
 * be reported and the previously initialized FrameContext is returned.
 *
 * Frame contexts are expected to be initialised when a Request is first passed
 * to the IPA module in IPAModule::queueRequest().
 *
 * \return A reference to the FrameContext for sequence \a frame
 */

/**
 * \fn FCQueue::get()
 * \brief Obtain the Frame Context at slot specified by \a frame
 * \param[in] frame The frame context sequence number
 *
 * Obtains an existing FrameContext from the queue and returns it to the caller.
 *
 * If the FrameContext is not correctly initialised for the \a frame, it will be
 * initialised and a PFCError will be flagged on the context to be transferred
 * to the Request when it completes.
 *
 * \return A reference to the FrameContext for sequence \a frame
 */

} /* namespace ipa */

} /* namespace libcamera */

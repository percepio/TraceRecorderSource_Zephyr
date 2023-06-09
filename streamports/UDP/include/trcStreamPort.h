/*
 * Trace Recorder for Tracealyzer v4.8.0
 * Copyright 2023 Percepio AB
 * www.percepio.com
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * The interface definitions for trace streaming ("stream ports").
 * This "stream port" sets up the recorder to use UDP as streaming channel.
 * The example is for lwIP.
 */

#ifndef TRC_STREAM_PORT_H
#define TRC_STREAM_PORT_H

#include <stdint.h>
#include <trcTypes.h>
#include <trcStreamPortConfig.h>

#ifdef __cplusplus
extern "C" {
#endif

#define TRC_USE_INTERNAL_BUFFER (TRC_CFG_STREAM_PORT_USE_INTERNAL_BUFFER)

#define TRC_INTERNAL_EVENT_BUFFER_WRITE_MODE (TRC_CFG_STREAM_PORT_INTERNAL_BUFFER_WRITE_MODE)

#define TRC_INTERNAL_EVENT_BUFFER_TRANSFER_MODE (TRC_CFG_STREAM_PORT_INTERNAL_BUFFER_TRANSFER_MODE)

#define TRC_INTERNAL_BUFFER_CHUNK_SIZE (TRC_CFG_STREAM_PORT_INTERNAL_BUFFER_CHUNK_SIZE)

#define TRC_INTERNAL_BUFFER_CHUNK_TRANSFER_AGAIN_SIZE_LIMIT (TRC_CFG_STREAM_PORT_INTERNAL_BUFFER_CHUNK_TRANSFER_AGAIN_SIZE_LIMIT)

#define TRC_INTERNAL_BUFFER_CHUNK_TRANSFER_AGAIN_COUNT_LIMIT (TRC_CFG_STREAM_PORT_INTERNAL_BUFFER_CHUNK_TRANSFER_AGAIN_COUNT_LIMIT)

/**
 * @def TRC_STREAM_PORT_BUFFER_SIZE
 *
 * @brief The buffer size, aligned to base type.
 */
#define TRC_STREAM_PORT_BUFFER_SIZE ((((TRC_CFG_STREAM_PORT_BUFFER_SIZE) + sizeof(TraceUnsignedBaseType_t) - 1) / sizeof(TraceUnsignedBaseType_t)) * sizeof(TraceUnsignedBaseType_t))

typedef struct TraceStreamPortBuffer	/* Aligned */
{
#if (TRC_USE_INTERNAL_BUFFER)
	uint8_t buffer[(TRC_STREAM_PORT_BUFFER_SIZE)];
#else
	TraceUnsignedBaseType_t buffer[1];
#endif
	uint8_t recv_buffer[32];
	uint32_t recv_len;
	uint32_t reserved;		/* alignment */
} TraceStreamPortBuffer_t;

int32_t prvTraceUdpWrite(void* pvData, uint32_t uiSize, int32_t* piBytesWritten);

int32_t prvTraceUdpRead(void* pvData, uint32_t uiSize, int32_t* piBytesRead);

traceResult xTraceStreamPortInitialize(TraceStreamPortBuffer_t* pxBuffer);

#if (TRC_USE_INTERNAL_BUFFER == 1)
/* Push to internal buffer. It will call on xTraceStreamPortWriteData() periodically. */

#define xTraceStreamPortAllocate xTraceInternalEventBufferAlloc
#define xTraceStreamPortCommit xTraceInternalEventBufferAllocCommit
#else
/* Write directly */
#define xTraceStreamPortAllocate(uiSize, ppvData) ((void)(uiSize), xTraceStaticBufferGet(ppvData))
#define xTraceStreamPortCommit xTraceStreamPortWriteData
#endif

#define xTraceStreamPortWriteData(pvData, uiSize, piBytesWritten) (prvTraceUdpWrite(pvData, uiSize, piBytesWritten) == 0 ? TRC_SUCCESS : TRC_FAIL)

#define xTraceStreamPortReadData(pvData, uiSize, piBytesRead) (prvTraceUdpRead(pvData, uiSize, piBytesRead) == 0 ? TRC_SUCCESS : TRC_FAIL)

#define xTraceStreamPortOnEnable(uiStartOption) ((void)(uiStartOption), TRC_SUCCESS)

#define xTraceStreamPortOnDisable() (TRC_SUCCESS)

#define xTraceStreamPortOnTraceBegin() (TRC_SUCCESS)

traceResult xTraceStreamPortOnTraceEnd(void);

#ifdef __cplusplus
}
#endif

#endif /* TRC_STREAM_PORT_H */


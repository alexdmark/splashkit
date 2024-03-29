/*
 * Copyright (c) 2008-2010 Brent Fulgham <bfulgham@gmail.org>.  All rights reserved.
 *
 * This source code is a modified version of the CoreFoundation sources released by Apple Inc. under
 * the terms of the APSL version 2.0 (see below).
 *
 * For information about changes from the original Apple source release can be found by reviewing the
 * source control system for the project at https://sourceforge.net/svn/?group_id=246198.
 *
 * The original license information is as follows:
 * 
 * Copyright (c) 2009 Apple Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

/*	CFStream.h
	Copyright (c) 2000-2009, Apple Inc. All rights reserved.
*/

#if !defined(__COREFOUNDATION_CFSTREAM__)
#define __COREFOUNDATION_CFSTREAM__ 1

#include <CoreFoundation/CFBase.h>
#include <CoreFoundation/CFString.h>
#include <CoreFoundation/CFDictionary.h>
#include <CoreFoundation/CFURL.h>
#include <CoreFoundation/CFRunLoop.h>
#include <CoreFoundation/CFSocket.h>
#include <CoreFoundation/CFError.h>

CF_EXTERN_C_BEGIN

enum {
    kCFStreamStatusNotOpen = 0,
    kCFStreamStatusOpening,  /* open is in-progress */
    kCFStreamStatusOpen,
    kCFStreamStatusReading,
    kCFStreamStatusWriting,
    kCFStreamStatusAtEnd,    /* no further bytes can be read/written */
    kCFStreamStatusClosed,
    kCFStreamStatusError
};
typedef CFIndex CFStreamStatus;

enum {
    kCFStreamEventNone = 0,
    kCFStreamEventOpenCompleted = 1,
    kCFStreamEventHasBytesAvailable = 2,
    kCFStreamEventCanAcceptBytes = 4, 
    kCFStreamEventErrorOccurred = 8,
    kCFStreamEventEndEncountered = 16
};
typedef CFOptionFlags CFStreamEventType;

typedef struct {
    CFIndex version;
    void *info;
    void *(*retain)(void *info);
    void (*release)(void *info);
    CFStringRef (*copyDescription)(void *info);
} CFStreamClientContext;

typedef struct __CFReadStream * CFReadStreamRef;
typedef struct __CFWriteStream * CFWriteStreamRef;

typedef void (*CFReadStreamClientCallBack)(CFReadStreamRef stream, CFStreamEventType type, void *clientCallBackInfo);
typedef void (*CFWriteStreamClientCallBack)(CFWriteStreamRef stream, CFStreamEventType type, void *clientCallBackInfo);

CF_EXPORT
CFTypeID CFReadStreamGetTypeID(void);
CF_EXPORT
CFTypeID CFWriteStreamGetTypeID(void);

/* Memory streams */

/* Value will be a CFData containing all bytes thusfar written; used to recover the data written to a memory write stream. */
CF_EXPORT
const CFStringRef kCFStreamPropertyDataWritten;

/* Pass kCFAllocatorNull for bytesDeallocator to prevent CFReadStream from deallocating bytes; otherwise, CFReadStream will deallocate bytes when the stream is destroyed */
CF_EXPORT
CFReadStreamRef CFReadStreamCreateWithBytesNoCopy(CFAllocatorRef alloc, const UInt8 *bytes, CFIndex length, CFAllocatorRef bytesDeallocator);

/* The stream writes into the buffer given; when bufferCapacity is exhausted, the stream is exhausted (status becomes kCFStreamStatusAtEnd) */
CF_EXPORT
CFWriteStreamRef CFWriteStreamCreateWithBuffer(CFAllocatorRef alloc, UInt8 *buffer, CFIndex bufferCapacity);

/* New buffers are allocated from bufferAllocator as bytes are written to the stream.  At any point, you can recover the bytes thusfar written by asking for the property kCFStreamPropertyDataWritten, above */
CF_EXPORT
CFWriteStreamRef CFWriteStreamCreateWithAllocatedBuffers(CFAllocatorRef alloc, CFAllocatorRef bufferAllocator);

/* File streams */
CF_EXPORT
CFReadStreamRef CFReadStreamCreateWithFile(CFAllocatorRef alloc, CFURLRef fileURL);
CF_EXPORT
CFWriteStreamRef CFWriteStreamCreateWithFile(CFAllocatorRef alloc, CFURLRef fileURL);
CF_EXPORT
void CFStreamCreateBoundPair(CFAllocatorRef alloc, CFReadStreamRef *readStream, CFWriteStreamRef *writeStream, CFIndex transferBufferSize);

#if MAC_OS_X_VERSION_10_2 <= MAC_OS_X_VERSION_MAX_ALLOWED
/* Property for file write streams; value should be a CFBoolean.  Set to TRUE to append to a file, rather than to replace its contents */
CF_EXPORT
const CFStringRef kCFStreamPropertyAppendToFile;
#endif

#if MAC_OS_X_VERSION_10_3 <= MAC_OS_X_VERSION_MAX_ALLOWED

CF_EXPORT const CFStringRef kCFStreamPropertyFileCurrentOffset AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;   // Value is a CFNumber

#endif

/* Socket stream properties */

/* Value will be a CFData containing the native handle */
CF_EXPORT
const CFStringRef kCFStreamPropertySocketNativeHandle;

/* Value will be a CFString, or NULL if unknown */
CF_EXPORT
const CFStringRef kCFStreamPropertySocketRemoteHostName;

/* Value will be a CFNumber, or NULL if unknown */
CF_EXPORT
const CFStringRef kCFStreamPropertySocketRemotePortNumber;

/* Socket streams; the returned streams are paired such that they use the same socket; pass NULL if you want only the read stream or the write stream */
CF_EXPORT
void CFStreamCreatePairWithSocket(CFAllocatorRef alloc, CFSocketNativeHandle sock, CFReadStreamRef *readStream, CFWriteStreamRef *writeStream);
CF_EXPORT
void CFStreamCreatePairWithSocketToHost(CFAllocatorRef alloc, CFStringRef host, UInt32 port, CFReadStreamRef *readStream, CFWriteStreamRef *writeStream);
#if MAC_OS_X_VERSION_10_2 <= MAC_OS_X_VERSION_MAX_ALLOWED
CF_EXPORT
void CFStreamCreatePairWithPeerSocketSignature(CFAllocatorRef alloc, const CFSocketSignature *signature, CFReadStreamRef *readStream, CFWriteStreamRef *writeStream);
#endif


/* Returns the current state of the stream */
CF_EXPORT
CFStreamStatus CFReadStreamGetStatus(CFReadStreamRef stream);
CF_EXPORT
CFStreamStatus CFWriteStreamGetStatus(CFWriteStreamRef stream);

/* Returns NULL if no error has occurred; otherwise returns the error. */
CF_EXPORT
CFErrorRef CFReadStreamCopyError(CFReadStreamRef stream) AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
CF_EXPORT
CFErrorRef CFWriteStreamCopyError(CFWriteStreamRef stream) AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/* Returns success/failure.  Opening a stream causes it to reserve all the system
   resources it requires.  If the stream can open non-blocking, this will always 
   return TRUE; listen to the run loop source to find out when the open completes
   and whether it was successful, or poll using CFRead/WriteStreamGetStatus(), waiting 
   for a status of kCFStreamStatusOpen or kCFStreamStatusError.  */
CF_EXPORT
Boolean CFReadStreamOpen(CFReadStreamRef stream);
CF_EXPORT
Boolean CFWriteStreamOpen(CFWriteStreamRef stream);

/* Terminates the flow of bytes; releases any system resources required by the 
   stream.  The stream may not fail to close.  You may call CFStreamClose() to 
   effectively abort a stream. */
CF_EXPORT
void CFReadStreamClose(CFReadStreamRef stream);
CF_EXPORT
void CFWriteStreamClose(CFWriteStreamRef stream);

/* Whether there is data currently available for reading; returns TRUE if it's 
   impossible to tell without trying */
CF_EXPORT
Boolean CFReadStreamHasBytesAvailable(CFReadStreamRef stream);

/* Returns the number of bytes read, or -1 if an error occurs preventing any 
   bytes from being read, or 0 if the stream's end was encountered.  
   It is an error to try and read from a stream that hasn't been opened first.  
   This call will block until at least one byte is available; it will NOT block
   until the entire buffer can be filled.  To avoid blocking, either poll using
   CFReadStreamHasBytesAvailable() or use the run loop and listen for the 
   kCFStreamCanRead event for notification of data available. */
CF_EXPORT
CFIndex CFReadStreamRead(CFReadStreamRef stream, UInt8 *buffer, CFIndex bufferLength);

/* Returns a pointer to an internal buffer if possible (setting *numBytesRead
   to the length of the returned buffer), otherwise returns NULL; guaranteed 
   to return in O(1).  Bytes returned in the buffer are considered read from 
   the stream; if maxBytesToRead is greater than 0, not more than maxBytesToRead
   will be returned.  If maxBytesToRead is less than or equal to zero, as many bytes
   as are readily available will be returned.  The returned buffer is good only 
   until the next stream operation called on the stream.  Caller should neither 
   change the contents of the returned buffer nor attempt to deallocate the buffer;
   it is still owned by the stream. */
CF_EXPORT
const UInt8 *CFReadStreamGetBuffer(CFReadStreamRef stream, CFIndex maxBytesToRead, CFIndex *numBytesRead);

/* Whether the stream can currently be written to without blocking;
   returns TRUE if it's impossible to tell without trying */
CF_EXPORT
Boolean CFWriteStreamCanAcceptBytes(CFWriteStreamRef stream);

/* Returns the number of bytes successfully written, -1 if an error has
   occurred, or 0 if the stream has been filled to capacity (for fixed-length
   streams).  If the stream is not full, this call will block until at least
   one byte is written.  To avoid blocking, either poll via CFWriteStreamCanAcceptBytes
   or use the run loop and listen for the kCFStreamCanWrite event. */
CF_EXPORT
CFIndex CFWriteStreamWrite(CFWriteStreamRef stream, const UInt8 *buffer, CFIndex bufferLength);

/* Particular streams can name properties and assign meanings to them; you
   access these properties through the following calls.  A property is any interesting
   information about the stream other than the data being transmitted itself.
   Examples include the headers from an HTTP transmission, or the expected 
   number of bytes, or permission information, etc.  Properties that can be set
   configure the behavior of the stream, and may only be settable at particular times
   (like before the stream has been opened).  See the documentation for particular 
   properties to determine their get- and set-ability. */
CF_EXPORT
CFTypeRef CFReadStreamCopyProperty(CFReadStreamRef stream, CFStringRef propertyName);
CF_EXPORT
CFTypeRef CFWriteStreamCopyProperty(CFWriteStreamRef stream, CFStringRef propertyName);

#if MAC_OS_X_VERSION_10_2 <= MAC_OS_X_VERSION_MAX_ALLOWED
/* Returns TRUE if the stream recognizes and accepts the given property-value pair; 
   FALSE otherwise. */
CF_EXPORT
Boolean CFReadStreamSetProperty(CFReadStreamRef stream, CFStringRef propertyName, CFTypeRef propertyValue);
CF_EXPORT
Boolean CFWriteStreamSetProperty(CFWriteStreamRef stream, CFStringRef propertyName, CFTypeRef propertyValue);
#endif

/* Asynchronous processing - If you wish to neither poll nor block, you may register 
   a client to hear about interesting events that occur on a stream.  Only one client
   per stream is allowed; registering a new client replaces the previous one.

   Once you have set a client, you need to schedule a run loop on which that client
   can be notified.  You may schedule multiple run loops (for instance, if you are 
   using a thread pool).  The client callback will be triggered via one of the scheduled
   run loops; It is the caller's responsibility to ensure that at least one of the 
   scheduled run loops is being run.

   NOTE: Unlike other CoreFoundation APIs, pasing a NULL clientContext here will remove
   the client.  If you do not care about the client context (i.e. your only concern
   is that your callback be called), you should pass in a valid context where every
   entry is 0 or NULL.

*/

CF_EXPORT
Boolean CFReadStreamSetClient(CFReadStreamRef stream, CFOptionFlags streamEvents, CFReadStreamClientCallBack clientCB, CFStreamClientContext *clientContext);
CF_EXPORT
Boolean CFWriteStreamSetClient(CFWriteStreamRef stream, CFOptionFlags streamEvents, CFWriteStreamClientCallBack clientCB, CFStreamClientContext *clientContext);

CF_EXPORT
void CFReadStreamScheduleWithRunLoop(CFReadStreamRef stream, CFRunLoopRef runLoop, CFStringRef runLoopMode);
CF_EXPORT
void CFWriteStreamScheduleWithRunLoop(CFWriteStreamRef stream, CFRunLoopRef runLoop, CFStringRef runLoopMode);

CF_EXPORT
void CFReadStreamUnscheduleFromRunLoop(CFReadStreamRef stream, CFRunLoopRef runLoop, CFStringRef runLoopMode);
CF_EXPORT
void CFWriteStreamUnscheduleFromRunLoop(CFWriteStreamRef stream, CFRunLoopRef runLoop, CFStringRef runLoopMode);


/* The following API is deprecated starting in 10.5; please use CFRead/WriteStreamCopyError(), above, instead */
enum {
    kCFStreamErrorDomainCustom = -1,      /* custom to the kind of stream in question */
    kCFStreamErrorDomainPOSIX = 1,        /* POSIX errno; interpret using <sys/errno.h> */
    kCFStreamErrorDomainMacOSStatus      /* OSStatus type from Carbon APIs; interpret using <MacTypes.h> */
};
typedef CFIndex CFStreamErrorDomain;

typedef struct {
    CFIndex domain; 
    SInt32 error;
} CFStreamError;
CF_EXPORT
CFStreamError CFReadStreamGetError(CFReadStreamRef stream);
CF_EXPORT
CFStreamError CFWriteStreamGetError(CFWriteStreamRef stream);


CF_EXTERN_C_END

#endif /* ! __COREFOUNDATION_CFSTREAM__ */

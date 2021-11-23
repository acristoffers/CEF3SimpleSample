/*******************************************************************************
 *   Copyright (c) 2013 Álan Crístoffer
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 ******************************************************************************/

/*
 * The event capture part was taken from
 * https://github.com/chromelyapps/Chromely/blob/master/src/mac/chromely_mac.mm
 */

#import <Cocoa/Cocoa.h>
#import <objc/runtime.h>

#include "include/cef_application_mac.h"
#include "include/wrapper/cef_library_loader.h"

bool g_handling_send_event = false;

// We will capture all events to keep a count
@interface NSApplication (CEFSimpleSampleNSApplication) <CefAppProtocol>
- (void)_swizzled_sendEvent:(NSEvent *)event;
- (void)_swizzled_terminate:(id)sender;
@end

@implementation NSApplication (CEFSimpleSampleNSApplication)
// wraps sendEvent and terminate
+ (void)load {
	Method original = class_getInstanceMethod(self, @selector(sendEvent));
	Method swizzled =
		class_getInstanceMethod(self, @selector(_swizzled_sendEvent));
	Method originalTerm = class_getInstanceMethod(self,
	                                              @selector(terminate:));
	Method swizzledTerm =
		class_getInstanceMethod(self, @selector(_swizzled_terminate:));

	method_exchangeImplementations(original, swizzled);
	method_exchangeImplementations(originalTerm, swizzledTerm);
}

- (BOOL)isHandlingSendEvent {
	return g_handling_send_event;
}

- (void)setHandlingSendEvent:(BOOL)handlingSendEvent {
	g_handling_send_event = handlingSendEvent;
}

- (void)_swizzled_sendEvent:(NSEvent *)event {
	CefScopedSendingEvent sendingEventScoper;

	[self _swizzled_sendEvent:event];
}

- (void)_swizzled_terminate:(id)sender {
	[self _swizzled_terminate:sender];
}

@end

int main(int argc, char *argv[]) {
	CefScopedLibraryLoader library_loader;

	if (!library_loader.LoadInMain()) {
		return 1;
	}

	return NSApplicationMain(argc, (const char **)argv);
}

/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SampleSkNSView.h"
#include "SkApplication.h"
#include "SampleWindow.h"
#include <crt_externs.h>

@implementation SampleSkNSView

- (id)initWithDefaults {
    if ((self = [super initWithDefaults])) {
        fWind = create_sk_window((__bridge void*)self, *_NSGetArgc(), *_NSGetArgv());
    }
    return self;
}

- (void)dealloc {
    delete fWind;
}

- (void)openFile:(std::string)url {
    ((SampleWindow*)fWind)->loadPng(url);
}

@end

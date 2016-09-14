//  AppDelegate.m

#include <string>
#include "AppDelegate.h"
#include "SampleSkNSView.h"

@interface AppDelegate ()

@property (weak) IBOutlet NSWindow *window;
@property (weak) IBOutlet SampleSkNSView *view;
@end

@implementation AppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification {
    NSRect viewRect = NSMakeRect(0, 0, 1000, 1000);
    SampleSkNSView* customView = [[SampleSkNSView alloc] initWithFrame:viewRect];
    [customView setTranslatesAutoresizingMaskIntoConstraints:NO];
    
    NSView* contentView = _window.contentView;
    [contentView addSubview:customView];
    _view = customView;
    
    NSDictionary *views = NSDictionaryOfVariableBindings(customView);
    [contentView addConstraints:
     [NSLayoutConstraint constraintsWithVisualFormat:@"H:|[customView]|"
                                             options:0
                                             metrics:nil
                                               views:views]];
    
    [contentView addConstraints:
     [NSLayoutConstraint constraintsWithVisualFormat:@"V:|[customView]|"
                                             options:0
                                             metrics:nil
                                               views:views]];
    // Insert code here to initialize your application
}

- (void)applicationWillTerminate:(NSNotification *)aNotification {
    // Insert code here to tear down your application
}

- (IBAction)openFile:(NSMenuItem *)sender {
    NSOpenPanel *openPanel = [NSOpenPanel openPanel];
    openPanel.title = @"Choose a PNG";
    openPanel.showsResizeIndicator = true;
    openPanel.showsHiddenFiles = false;
    openPanel.canChooseDirectories = false;
    openPanel.canCreateDirectories = true;
    openPanel.allowsMultipleSelection = false;
    openPanel.allowedFileTypes = [NSArray arrayWithObjects:@"png", nil];
    
    if ([openPanel runModal ] == NSModalResponseOK) {
        NSURL *url = openPanel.URL;
        if (url != nil) {
            std::string rawPath([url.path UTF8String]);
            [_view openFile:rawPath];
        }
    } else {
        return;
    }
}

@end

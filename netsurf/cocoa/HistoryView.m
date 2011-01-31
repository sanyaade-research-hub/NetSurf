/*
 * Copyright 2011 Sven Weidauer <sven.weidauer@gmail.com>
 *
 * This file is part of NetSurf, http://www.netsurf-browser.org/
 *
 * NetSurf is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * NetSurf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#import "cocoa/HistoryView.h"
#import "cocoa/font.h"
#import "cocoa/coordinates.h"
#import "cocoa/plotter.h"

#import "desktop/browser.h"
#import "desktop/history_core.h"
#import "desktop/plotters.h"

static NSRect cocoa_history_rect( struct browser_window *bw )
{
	int width, height;
	history_size( bw->history, &width, &height );
	return cocoa_rect( 0, 0, width + 10, height + 10 );
}

@implementation HistoryView

@synthesize browser;

- (id)initWithBrowser: (struct browser_window *)bw;
{
	NSParameterAssert( NULL != bw );
	if ((self = [super initWithFrame: cocoa_history_rect( bw )]) == nil) return nil;
	
	browser = bw;
	
	return self;
}

- (void) updateHistory;
{
	[self setFrameSize: cocoa_history_rect( browser ).size];

	NSView *superView = [self superview];
	if (nil != superView) {
		NSRect visibleRect = [superView visibleRect];
		NSRect rect = [self frame];
		NSRect frame = [superView frame];
		
		rect.origin.x = visibleRect.origin.x + (NSWidth( visibleRect ) - NSWidth( rect )) / 2.0;
		rect.origin.x = MAX( rect.origin.x, frame.origin.x );
		
		rect.origin.y = visibleRect.origin.y + (NSHeight( visibleRect ) - NSHeight( rect )) / 2.0;
		rect.origin.y = MAX( rect.origin.y, frame.origin.y );
		
		[self setFrameOrigin: rect.origin];
	}
	
	[self setNeedsDisplay: YES];
}

- (void) fadeIntoView: (NSView *) superView;
{
	[self setAlphaValue: 0];
	[superView addSubview: self];
	[[self animator] setAlphaValue: 1.0];
}

- (void) fadeOut;
{
	[[self animator] setAlphaValue: 0.0];
	[self performSelector: @selector(removeFromSuperview) withObject: nil 
			   afterDelay: [[NSAnimationContext currentContext] duration]];
}

- (void) viewDidMoveToSuperview;
{
	[self updateHistory];
}

- (void) drawRect: (NSRect)rect;
{
	NSBezierPath *path = [NSBezierPath bezierPathWithRoundedRect: NSInsetRect( [self bounds], 5, 5 ) 
														 xRadius: 10 yRadius: 10];

	[[NSColor colorWithDeviceWhite: 0 alpha: 0.8] setFill];
	[path fill];

	[path setLineWidth: 2.0];
	[[NSColor whiteColor] set];
	[path stroke];

	cocoa_set_font_scale_factor( 1.0 );
	cocoa_set_clip( rect );
	
	history_redraw( browser->history );
}

- (void) mouseUp: (NSEvent *)theEvent;
{
	const NSPoint location = [self convertPoint: [theEvent locationInWindow] fromView: nil];
	const bool newWindow = [theEvent modifierFlags] & NSCommandKeyMask;
	history_click( browser, browser->history, 
				   cocoa_pt_to_px( location.x ), cocoa_pt_to_px( location.y ), 
				   newWindow );
}

- (BOOL) isFlipped;
{
	return YES;
}


@end
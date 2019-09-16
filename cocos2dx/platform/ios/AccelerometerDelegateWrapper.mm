/****************************************************************************
 Copyright (c) 2010 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "AccelerometerDelegateWrapper.h"

@implementation AccelerometerDispatcher

static AccelerometerDispatcher* s_pAccelerometerDispatcher;

@synthesize delegate_;
@synthesize acceleration_;

+ (id) sharedAccelerometerDispather
{
    if (s_pAccelerometerDispatcher == nil) {
        s_pAccelerometerDispatcher = [[self alloc] init];
    }
    
    return s_pAccelerometerDispatcher;
}

- (id) init
{
    acceleration_ = new cocos2d::CCAcceleration();
	motionManager = [[CMMotionManager alloc] init];
    return self;
}

- (void) dealloc
{
	if (motionManager.accelerometerActive) {
		[motionManager stopAccelerometerUpdates];
	}
	[motionManager release];
    s_pAccelerometerDispatcher = 0;
    delegate_ = 0;
    delete acceleration_;
    [super dealloc];
}

- (void) addDelegate: (cocos2d::CCAccelerometerDelegate *) delegate
{
    delegate_ = delegate;
    
    if (delegate_)
    {
		[motionManager startAccelerometerUpdatesToQueue:[NSOperationQueue currentQueue] withHandler:^(CMAccelerometerData *accelerometerData, NSError *error) {
			[self accelerometer:accelerometerData];
		}];
	}
    else 
    {
		[motionManager stopAccelerometerUpdates];
    }
}

-(void) setAccelerometerInterval:(float)interval
{
	motionManager.accelerometerUpdateInterval = interval;
}

- (void)accelerometer:(CMAccelerometerData *)accelerometerData
{
    acceleration_->x = accelerometerData.acceleration.x;
    acceleration_->y = accelerometerData.acceleration.y;
    acceleration_->z = accelerometerData.acceleration.z;
    acceleration_->timestamp = accelerometerData.timestamp;
    
    double tmp = acceleration_->x;
    
    switch ([[UIApplication sharedApplication] statusBarOrientation]) 
    {
    case UIInterfaceOrientationLandscapeRight:
        acceleration_->x = -acceleration_->y;
        acceleration_->y = tmp;
        break;
        
    case UIInterfaceOrientationLandscapeLeft:
        acceleration_->x = acceleration_->y;
        acceleration_->y = -tmp;
        break;
        
    case UIInterfaceOrientationPortraitUpsideDown:
        acceleration_->x = -acceleration_->y;
        acceleration_->y = -tmp;
        break;
            
    case UIInterfaceOrientationPortrait:
        break;
    }

	if ( delegate_ ) {
		delegate_->didAccelerate(acceleration_);
	}
}

@end


//
//  VConIc3d.m
//  DevEng
//
//  Created by Sherman Chen on 3/2/16.
//  Copyright (c) 2016 Sherman Chen. All rights reserved.
//

#import "IcViewController.h"
#import <OpenGLES/ES2/glext.h>
#include "Ic3d.h"

const static GLfloat K_bkColor[4] = {0.2, 0.4, 0.9, 1.0};
@interface IcViewController () {
	
	bool m_hasInit;
    bool m_reqViewSizeChange;
}

@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL;
- (void)tearDownGL;
@end

@implementation IcViewController

- (void)viewDidLoad
{
	[super viewDidLoad];
	m_hasInit = false;
    m_reqViewSizeChange = false;
	
	self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
	
	if (!self.context) {
		NSLog(@"Failed to create ES context");
	}
	
	GLKView *view = (GLKView *)self.view;
	view.context = self.context;
	view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
	[self setupGL];
}
-(CGRect)getScaledViewRect
{
    float w = self.view.frame.size.width;
    float h = self.view.frame.size.height;
    float scrnScl = 1.0f;
    if ([[UIScreen mainScreen] respondsToSelector:@selector(displayLinkWithTarget:selector:)]) {
        scrnScl = [[UIScreen mainScreen] scale];
    }
    CGRect vrect = CGRectMake(0, 0, w*scrnScl, h*scrnScl);
    return vrect;
}

//---- These are to be override
// TODO: Ic3d_ -> IcEng_

-(void)Ic3d_onInit
{
    //----- Configure App
    NSString* nsPath = [[NSBundle mainBundle] resourcePath];
    std::string sPathRes = std::string([nsPath UTF8String]) +"/";
    
    auto& cfg = Ic3d::IcEng::getInstance()->m_cfg;
    cfg.m_sPath_shader = sPathRes + "IcShader/";
 
}
-(void)Ic3d_onViewRect:(CGRect)viewRect{};
-(void)Ic3d_onDrawUpdate:(double)deltaT{};

-(void) viewWillLayoutSubviews
{
    if(!m_hasInit) return;
    CGRect vrect = [self getScaledViewRect];
    [self Ic3d_onViewRect:vrect];
    NSLog(@"[Debug]: VConIc3d: viewWillLayoutSubviews");
}

-(void) viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator
{
    // best call super just in case
    [super viewWillTransitionToSize:size withTransitionCoordinator:coordinator];
    
    // will execute before rotation
    [coordinator animateAlongsideTransition:^(id  _Nonnull context) {
        
        // will execute during rotation
        
    } completion:^(id  _Nonnull context) {
        
        m_reqViewSizeChange = true;
        NSLog(@"---- VConIc3d: deviceRoated ----");
    }];
}


- (void)dealloc
{
	[self tearDownGL];
	
	if ([EAGLContext currentContext] == self.context) {
		[EAGLContext setCurrentContext:nil];
	}
}

- (void)didReceiveMemoryWarning
{
	[super didReceiveMemoryWarning];
	
	if ([self isViewLoaded] && ([[self view] window] == nil)) {
		self.view = nil;
		
		[self tearDownGL];
		
		if ([EAGLContext currentContext] == self.context) {
			[EAGLContext setCurrentContext:nil];
		}
		self.context = nil;
	}
	
	// Dispose of any resources that can be recreated.
}

- (BOOL)prefersStatusBarHidden {
	return YES;
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:self.context];

}

- (void)tearDownGL
{
	[EAGLContext setCurrentContext:self.context];
	
}

#pragma mark - GLKView and GLKViewController delegate methods

- (void)update
{

}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{

    //---- clear screen
    const auto& c = K_bkColor;
    glClearColor(c[0], c[1], c[2], c[3]);	// background color
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
	//---------------
	//	call delegate
	//---------------
    if(!m_hasInit)
    {
        CGRect vrect = [self getScaledViewRect];
        [self Ic3d_onInit];
        [self Ic3d_onViewRect:vrect];
        m_hasInit = true;
    }
    else
    {
        float deltaT = self.timeSinceLastDraw;
        [self Ic3d_onDrawUpdate:deltaT];
    }
}

@end
#include "AppDelegate.h"
#include "MainMenuScene.h"
#include "SplashScene.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

typedef struct tagResource
{
	cocos2d::Size size;
	char directory[100];
} Resource;


// Landscape config.
//// 800x480 = Normal screen, WVGA 
//static Resource smallResource = { cocos2d::Size(800, 480), "sd" };
//// 1280x800 = Extra large screen, WXGA, medium density screen (160)
//static Resource mediumResource = { cocos2d::Size(1280, 800), "hd" };
//// 1900x1200 = WUXGA, Devices with such as Nexus 7, Nvidia Shield.
//static Resource largeResource = { cocos2d::Size(1920, 1200), "shd" };

static Resource smallResource = { cocos2d::Size(400, 800), "sd" };
// 1280x800 = Extra large screen, WXGA, medium density screen (160)
static Resource mediumResource = { cocos2d::Size(800, 1280), "hd" };
// 1900x1200 = WUXGA, Devices with such as Nexus 7, Nvidia Shield.
static Resource largeResource = { cocos2d::Size(1200, 1920), "shd" };

static cocos2d::Size designResolutionSize = smallResource.size;

AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate() {
}

bool AppDelegate::applicationDidFinishLaunching() {
	// Initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	// Comment out this if statement if testing on an actual device.
	if (!glview) {
		glview = GLViewImpl::create("My Game");
		glview->setFrameSize(designResolutionSize.width, designResolutionSize.height);
		director->setOpenGLView(glview);
	}
	auto fileUtils = FileUtils::getInstance();
	CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("audio/background.mp3");

	// Set the design resolution with the NO_BORDER policy.
	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
	cocos2d::Size frameSize = glview->getFrameSize();
	std::vector<std::string> searchPath;

	// Here, we select the most appropriate resource (i.e. image set) according to the actual frame height of the device.
	// If the resource (image) size is different from design resolution size, we need to set content scale factor.
	// This will either scale the images up or down to match the design resolution size.
	// The content scale factor is the ratio of the resource's height to the height of design resolution.

	// if the frame's height is larger than the height of medium resource size, select large resource.
	float scaleFactor = 1.0f;
	if (frameSize.height > mediumResource.size.height) {
		//searchPath.push_back(largeResource.directory);
		//searchPath.push_back(mediumResource.directory);
		searchPath.push_back(smallResource.directory);
		scaleFactor = largeResource.size.height / designResolutionSize.height;
	}
	// if the frame's height is larger than the height of small resource size, select medium resource.
	else if (frameSize.height > smallResource.size.height) {
		//searchPath.push_back(mediumResource.directory);
		searchPath.push_back(smallResource.directory);
		scaleFactor = mediumResource.size.height / designResolutionSize.height;
	}
	// if the frame's height is smaller than the height of medium resource size, select small resource.
	else {
		searchPath.push_back(smallResource.directory);
		scaleFactor = smallResource.size.height / designResolutionSize.height;
	}

	director->setContentScaleFactor(scaleFactor);
	// Need to add our "data" path too.
	searchPath.push_back("data");
	fileUtils->setSearchPaths(searchPath);
	// Create the GameData singleton.
	m_ptr = GameData::sharedGameData();
	// Create the level loader object to load general game data.
	LevelLoader loader("data/Level2.json");
	//std::shared_ptr<LevelLoader> loader("data/GeneralGameData.json");

	// Set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0 / 60);
	// Run
	auto scene = Splash::createScene();
	director->runWithScene(scene);
	
	
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/background.mp3",true);
	CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1.0f);
	return true;
}

void AppDelegate::applicationDidEnterBackground() {
}

void AppDelegate::applicationWillEnterForeground() {
}
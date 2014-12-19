//
//  KillSwitch.cpp
//  XmasCracker
//
//  Created by Dan on 3/12/2014.
//
//

#include <fstream>
#include <sys/stat.h>

#include "KillSwitch.h"

#define SECONDS_IN_HOUR     60 * 60
#define HOURS_UNTIL_RECHECK 2 * SECONDS_IN_HOUR

#define MESSAGEBOX_TAG      1
#define LOADINGLABEL_TAG    2
#define OKBUTTON_TAG        3
#define MESSAGELABEL_TAG    4

USING_NS_CC;
using namespace cocos2d::ui;
using namespace cocos2d::network;

static const std::string kLocalConfigFileName = "killSwitch.config";

KillSwitch::KillSwitch()
{
    m_bMaintenanceMode  = false;
    m_bConfigLoading    = false;
    m_sAppUpdateLink    = "";
    
    m_iAppVersionCurrent = 0;
    m_iAppVersionMin    = 0;
    
    m_pMessageLayer = NULL;
    
    m_onCompletionFunction = nullptr;
}

KillSwitch::~KillSwitch()
{
}

void KillSwitch::loadConfig(const std::string &p_sConfigURL, cocos2d::Node * p_pNode, const std::function<void()> &p_onCompletionFunction)
{
    m_sConfigURL = p_sConfigURL;
    m_onCompletionFunction = p_onCompletionFunction;
    m_pNode = p_pNode;
    
    downloadConfig();
}

void KillSwitch::downloadConfig()
{
    CCLOG("Checking current config");
    if(!m_bConfigLoading)
    {
        m_bConfigLoading = true;
        createMessageLayer();
        
        std::string fullPath = FileUtils::getInstance()->getWritablePath() + kLocalConfigFileName;
        bool haveLocalConfig = FileUtils::getInstance()->isFileExist(fullPath);
        double seconds = 0;
        
        if(haveLocalConfig)
        {
            struct stat attrib;
            stat(fullPath.c_str(), &attrib);
            
            seconds = difftime(time(NULL), attrib.st_mtime);
        }
        
        if(!haveLocalConfig || seconds >= HOURS_UNTIL_RECHECK)
        {
            HttpRequest* request = new HttpRequest();
            
            request->setUrl(m_sConfigURL.c_str());
            
            request->setRequestType(HttpRequest::Type::GET);
            request->setResponseCallback( CC_CALLBACK_2(KillSwitch::onHttpRequestCompleted, this) );
            request->setTag("load KillSwitch Config");
            HttpClient::getInstance()->send(request);
            request->release();
            
            CCLOG("Downloading new config");
        }
        else
        {
            CCLOG("Using local config");
         
            m_pMessageLayer->runAction(Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create(std::bind(&KillSwitch::parseConfig, this))));
        }
    }
}

void KillSwitch::checkConfig()
{
    int appCurrentBuild = getBuildNumber();
    std::string message = "";
    bool isUpdateAvailable = !m_bMaintenanceMode && appCurrentBuild < m_iAppVersionCurrent;
    
    if(isUpdateAvailable)
    {
        CCLOG("App is less than current, but still valid");
        message = "There is a new update for " + getAppName();
    }

    if(m_bMaintenanceMode)
    {
        // App is in maintenance mode.
        CCLOG("App is in maintenance mode");
        CCLOG("Maintenance Message: %s", m_sMaintenanceMessage.c_str());
        message = m_sMaintenanceMessage;
    }
    else if(appCurrentBuild < m_iAppVersionMin)
    {
        CCLOG("App requires update before continuing.");
        message = "There is a mandatory update for " + getAppName() + ", please update to continue.";
    }
    
    std::ostringstream messageStream;
    messageStream <<  "Killswitch v0.1\n" <<
    "App Current Build: " << std::to_string(getBuildNumber()) + "\n" <<
    "Config URL: " << m_sConfigURL << "\n" <<
    "App Update Link: " << m_sAppUpdateLink << "\n" <<
    "App Latest Version: " << std::to_string(m_iAppVersionCurrent) << "\n" <<
    "App Minimum Version: " << std::to_string(m_iAppVersionCurrent) << "\n" <<
    std::boolalpha << "\n \nMaintenance Mode: " << m_bMaintenanceMode << "\n" <<
    "Maintenance Message:\n \n" << m_sMaintenanceMessage;
    
    CCLOG("%s", messageStream.str().c_str());
    
    m_bConfigLoading = false;
    
    if(message.length() > 0)
        showMessage(message, isUpdateAvailable);
    else
        onButtonPress(nullptr, cocos2d::ui::Widget::TouchEventType::ENDED);
}

void KillSwitch::createMessageLayer()
{
    if(m_pNode == nullptr) return; // Need to have a root node to display message on
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    float contentScale = 1 / Director::getInstance()->getContentScaleFactor();
    
    m_pMessageLayer = ModalLayer::create();
    m_pMessageLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
    m_pMessageLayer->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    
    auto backingLayer = LayerColor::create(Color4B(64, 64, 64, 0));
    backingLayer->setPosition(Vec2(-visibleSize.width / 2, -visibleSize.height / 2));//Vec2(-visibleSize.width / 2 + origin.x, -visibleSize.height / 2 + origin.y));
    m_pMessageLayer->addChild(backingLayer);
    
    auto messageBox = Scale9Sprite::create("MessageBox.png");
    messageBox->setContentSize(Size(visibleSize.width * 0.95f, 400));
    messageBox->setTag(MESSAGEBOX_TAG);
    
    m_pMessageLayer->addChild(messageBox);
    
    auto loadingLabel = Label::createWithTTF("Please wait...", "GeosansLight.ttf", 32 * contentScale);
    loadingLabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    loadingLabel->setPosition(Vec2(messageBox->getContentSize().width / 2, messageBox->getContentSize().height / 2));
    loadingLabel->setDimensions(messageBox->getContentSize().width - 20, 0);
    loadingLabel->setTextColor(Color4B::WHITE);
    loadingLabel->setTag(LOADINGLABEL_TAG);
    
    messageBox->addChild(loadingLabel);
    
    auto messageLabel = Label::createWithTTF("", "GeosansLight.ttf", 32 * contentScale);
    messageLabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
    messageLabel->setPosition(Vec2(messageBox->getContentSize().width / 2, messageBox->getContentSize().height / 2));
    messageLabel->setDimensions(messageBox->getContentSize().width - 20, 0);
    messageLabel->setTextColor(Color4B::WHITE);
    messageLabel->setVisible(false);
    messageLabel->setTag(MESSAGELABEL_TAG);
    
    messageBox->addChild(messageLabel);
    
    auto okButton = Button::create();
    okButton->loadTextures("MessageButton.png", "MessageButton_pressed.png", "MessageButton_disabled.png");
    okButton->setTitleFontName("GeosansLight.ttf");
    okButton->setTitleFontSize(32 * contentScale);
    okButton->setTitleText("OK");
    okButton->setScale9Enabled(true);
    okButton->setVisible(false);
    okButton->setContentSize(Size(300, okButton->getContentSize().height));
    okButton->setColor(Color3B(197, 35, 20));
    okButton->setTitleColor(Color3B::WHITE);
    okButton->setAnchorPoint(Vec2(0.5f, 0));
    okButton->setPosition(Vec2(messageBox->getContentSize().width / 2, 20));
    okButton->addTouchEventListener(CC_CALLBACK_2(KillSwitch::onButtonPress, this));
    okButton->setTag(OKBUTTON_TAG);
    
    messageBox->addChild(okButton);
    
    m_pUpdateButton = Button::create();
    m_pUpdateButton->loadTextures("MessageButton.png", "MessageButton_pressed.png", "MessageButton_disabled.png");
    m_pUpdateButton->setTitleFontName("GeosansLight.ttf");
    m_pUpdateButton->setTitleFontSize(32 * contentScale);
    m_pUpdateButton->setTitleText("Update");
    m_pUpdateButton->setScale9Enabled(true);
    m_pUpdateButton->setVisible(false);
    m_pUpdateButton->setContentSize(Size(300, m_pUpdateButton->getContentSize().height));
    m_pUpdateButton->setColor(Color3B(197, 35, 20));
    m_pUpdateButton->setTitleColor(Color3B::WHITE);
    m_pUpdateButton->setAnchorPoint(Vec2(0.5f, 0));
    m_pUpdateButton->setPosition(Vec2(messageBox->getContentSize().width / 2, 20));
    m_pUpdateButton->addTouchEventListener(CC_CALLBACK_2(KillSwitch::onButtonPress, this));
    
    messageBox->addChild(m_pUpdateButton);
    
    m_pNode->addChild(m_pMessageLayer, 100);
    
    messageBox->setContentSize(Size(loadingLabel->getContentSize().width + 20, loadingLabel->getContentSize().height + 20));
    
    loadingLabel->setPosition(Vec2(messageBox->getContentSize().width / 2, messageBox->getContentSize().height / 2));
    
    messageBox->setScale(0);
    messageBox->runAction(Sequence::createWithTwoActions(ScaleTo::create(0.2, 1.2, 1.2), ScaleTo::create(0.1, 1, 1)));
    backingLayer->runAction(FadeTo::create(0.2, 196));
}

void KillSwitch::showMessage(const std::string &p_sMessage, bool p_bShowUpdate)
{
    if(m_pMessageLayer == nullptr) return;
    
    auto messageBox = m_pMessageLayer->getChildByTag(MESSAGEBOX_TAG);
    auto loadingLabel = messageBox->getChildByTag(LOADINGLABEL_TAG);
    auto messageLabel = (Label*)messageBox->getChildByTag(MESSAGELABEL_TAG);
    auto okButton = (Button*)messageBox->getChildByTag(OKBUTTON_TAG);
    
    messageLabel->setString(p_sMessage);
    Size newSize = Size(messageLabel->getContentSize().width + 20, messageLabel->getContentSize().height + okButton->getContentSize().height + 40);
    
    messageLabel->setPosition(Vec2(newSize.width / 2, newSize.height / 2 + okButton->getContentSize().height - 40));
    
    auto hideLoading    = Sequence::createWithTwoActions(DelayTime::create(0.1), FadeOut::create(0.2));
    auto showMessage    = Sequence::createWithTwoActions(DelayTime::create(0.1), FadeIn::create(0.2));
    auto showOkBtn      = Sequence::createWithTwoActions(DelayTime::create(0.1), FadeIn::create(0.2));
    
    if (p_bShowUpdate)
    {
        m_pUpdateButton->setVisible(true);
        m_pUpdateButton->setOpacity(0);
        m_pUpdateButton->runAction(showOkBtn->clone());
        
        m_pUpdateButton->setPosition(Vec2(newSize.width * 0.25f, 20));
        okButton->setPosition(Vec2(newSize.width * 0.75f, 20));
        okButton->setTitleText("Later");
    }
    else
    {
        okButton->setPosition(Vec2(newSize.width / 2, 20));
    }
    
    messageLabel->setOpacity(0);
    okButton->setOpacity(0);
    messageLabel->setVisible(true);
    okButton->setVisible(true);
    
    messageBox->runAction(ActionTween::create(0.2, "height", messageBox->getContentSize().height, newSize.height));
    loadingLabel->runAction(hideLoading);
    messageLabel->runAction(showMessage);
    okButton->runAction(showOkBtn);
}

void KillSwitch::hideMessageLayer()
{
    if(m_pMessageLayer != nullptr)
    {
        //
        auto messageBox = m_pMessageLayer->getChildByTag(MESSAGEBOX_TAG);
        messageBox->runAction(getHideAction());
    }
}

void KillSwitch::flipKillSwitch()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    CCLOG("You pressed the close button. Windows Store Apps do not implement a close button.");
    return;
#endif
    
    Director::getInstance()->end();
    
    if(m_iAppVersionMin > getBuildNumber())
    {
        Application::getInstance()->openURL(m_sAppUpdateLink);
    }
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

Sequence * KillSwitch::getHideAction()
{
    auto scaleSequence = Sequence::createWithTwoActions(ScaleTo::create(0.1, 1.2, 1.2), ScaleTo::create(0.2, 0, 0));
    auto removeLayer = CallFunc::create( CC_CALLBACK_0(Layer::removeFromParentAndCleanup,m_pMessageLayer,true));
    auto sequence = Sequence::createWithTwoActions(scaleSequence, removeLayer);
    
    return sequence;
}

void KillSwitch::onButtonPress(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
    if(type != Widget::TouchEventType::ENDED) return;
    
    Action * sequence = nullptr;
    bool shouldFlipKillswitch = m_bMaintenanceMode || m_iAppVersionMin > getBuildNumber();
    
    if (shouldFlipKillswitch)
    {
        // Flip the switch
        sequence = Sequence::createWithTwoActions(getHideAction(), CallFunc::create(std::bind(&KillSwitch::flipKillSwitch, this)));
    }
    else
    {
        if (pSender == m_pUpdateButton)
        {
            Application::getInstance()->openURL(m_sAppUpdateLink);
        }
        
        // App OK
        if (m_onCompletionFunction != nullptr)
        {
            sequence = CallFunc::create(m_onCompletionFunction);
            //sequence = Sequence::createWithTwoActions(DelayTime::create(0.5f), CallFunc::create(m_successFunction));
        }
        else // no on success message, so just hide the message layer
        {
            sequence = getHideAction();
        }
    }
    
    if(sequence != nullptr && m_pMessageLayer != nullptr)
    {
        auto messageBox = m_pMessageLayer->getChildByTag(MESSAGEBOX_TAG);
        messageBox->runAction(sequence);
    }
    else
    {
        if (shouldFlipKillswitch)
            flipKillSwitch();
        else if (m_onCompletionFunction != nullptr)
            m_onCompletionFunction();
    }
}

void KillSwitch::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
    if (response && response->isSucceed())
    {
        // You can get original request type from: response->request->reqType
        if (0 != strlen(response->getHttpRequest()->getTag()))
        {
            CCLOG("%s completed", response->getHttpRequest()->getTag());
        }
        
        long statusCode = response->getResponseCode();
        CCLOG("response code: %ld", statusCode);
        
        // dump data
        std::vector<char> *v = response->getResponseData();
        std::string s = "";
        
        // Seems to be an odd issue with converting std::string to char*
        // So we count the braces and break when we close to be sure
        int braces = 0;
        for (unsigned int i = 0; i < v->size(); i++)
        {
            char c = (*v)[i];
            
            s += c;
            
            if(c == '{')
                braces++;
            else if(c == '}')
            {
                braces--;
                
                if(braces == 0) break;
            }
        }
        
        std::string fullPath = FileUtils::getInstance()->getWritablePath() + kLocalConfigFileName;
        
        std::ofstream ofs;
        ofs.open(fullPath.c_str(), std::ios::out | std::ios::trunc);
        ofs.write(s.c_str(), s.length());
        ofs.flush();
        ofs.close();
    }
    else
    {
        CCLOG("response failed");
        
        if (!response)
        {
            CCLOG("NULL response");
        }
        else if (!response->isSucceed())
        {
            CCLOG("error buffer: %s", response->getErrorBuffer());
        }
    }
    
    parseConfig();
}

void KillSwitch::parseConfig()
{
    std::string fullPath = FileUtils::getInstance()->getWritablePath() + kLocalConfigFileName;
    ssize_t bufferSize = 0;
    const char* mFileData = (const char*)FileUtils::getInstance()->getFileData(fullPath.c_str(), "r", &bufferSize);
    
    std::string json(mFileData);
    size_t strLength = json.length();
    
    if(bufferSize < strLength)
    {
        json = json.substr(0, bufferSize);
    }

    m_jsonConfig.Parse<0>(json.c_str());
    
    if(!m_jsonConfig.HasParseError())
    {
        const rapidjson::Value& appUpdateLink       =   m_jsonConfig["appUpdateLink"];
        const rapidjson::Value& appVersionCurrent   =   m_jsonConfig["appVersionCurrent"];
        const rapidjson::Value& appVersionMin       =   m_jsonConfig["appVersionMin"];
        const rapidjson::Value& maintenanceMode     =   m_jsonConfig["maintenanceMode"];
        const rapidjson::Value& maintenanceMessage  =   m_jsonConfig["maintenanceMessage"];
        
        if(appUpdateLink.IsString())
        {
			m_sAppUpdateLink = appUpdateLink.GetString();
        }
        
        if(appVersionCurrent.IsString())
        {
            m_iAppVersionCurrent = atoi(appVersionCurrent.GetString());
        }
        
        if(appVersionMin.IsString())
        {
            m_iAppVersionMin = atoi(appVersionMin.GetString());
        }
        
        if(maintenanceMode.IsBool())
        {
            m_bMaintenanceMode = maintenanceMode.GetBool();
        }
        
        if(maintenanceMessage.IsString())
        {
            m_sMaintenanceMessage = maintenanceMessage.GetString();
        }
        else
        {
            m_sMaintenanceMessage = getAppName() + " is currently down for maintenance.\n \nPlease try again later.";
        }
        
        checkConfig();
    }
    else
    {
        CCLOG("Error parsing JSON config");
    }
}
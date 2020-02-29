LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/AdventureGameScene.cpp\
                   ../../Classes/BackGroundLayer.cpp\
                   ../../Classes/BucketDoorZombies.cpp\
                   ../../Classes/BucketZombies.cpp\
                   ../../Classes/CabbagePult.cpp\
                   ../../Classes/Car.cpp\
                   ../../Classes/CherryBomb.cpp\
                   ../../Classes/CommonDoorZombies.cpp\
                   ../../Classes/CommonZombies.cpp\
                   ../../Classes/ConeDoorZombies.cpp\
                   ../../Classes/ConeZombies.cpp\
                   ../../Classes/Dialog.cpp\
                   ../../Classes/FirePeaShooter.cpp\
                   ../../Classes/GameEasterEggs.cpp\
                   ../../Classes/GameType.cpp\
                   ../../Classes/Garlic.cpp\
                   ../../Classes/GlobalVariable.cpp\
                   ../../Classes/HelpScene.cpp\
                   ../../Classes/InputDataScene.cpp\
                   ../../Classes/Jalapeno.cpp\
                   ../../Classes/JudgeProFile.cpp\
                   ../../Classes/LevelData.cpp\
                   ../../Classes/LoadingScene.cpp\
                   ../../Classes/LpmZombies.cpp\
                   ../../Classes/MainMenu.cpp\
                   ../../Classes/OptionsSence.cpp\
                   ../../Classes/Pause_Quit_Game.cpp\
                   ../../Classes/PeaShooter.cpp\
                   ../../Classes/Plants.cpp\
                   ../../Classes/PotatoMine.cpp\
                   ../../Classes/Quit_Game.cpp\
                   ../../Classes/QuitScene.cpp\
                   ../../Classes/Request.cpp\
                   ../../Classes/SelectPlantsGameReadyScene.cpp\
                   ../../Classes/SelectWorldScene.cpp\
                   ../../Classes/Spikeweed.cpp\
                   ../../Classes/SunFlower.cpp\
                   ../../Classes/Torchwood.cpp\
                   ../../Classes/UiIamgeText.cpp\
                   ../../Classes/UserLose.cpp\
                   ../../Classes/UserWinRequirement.cpp\
                   ../../Classes/WallNut.cpp\
                   ../../Classes/World_1.cpp\
                   ../../Classes/Zombies.cpp\
                   ../../Classes/HelloWorldScene.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END

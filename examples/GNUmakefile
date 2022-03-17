NAME = 0-minimal

clear:
	clear

#############
### LINUX ###
#############

BIN   = example
OBJ   = $(NAME).o
LIBS  = ../bin/linux/x86_64/libmatoya.a

CFLAGS  = -I../src
LDFLAGS = -lm -ldl -lpthread

all: clean clear app

clean:
	rm -f $(BIN) $(OBJ)

app: $(OBJ)
	cc -o $(BIN) $(OBJ) $(LIBS) $(LDFLAGS)

###############
### ANDROID ###
###############

ANDROID_PROJECT        = android
ANDROID_PACKAGE        = group.matoya
ANDROID_APPLICATION_ID = group.matoya.debug

ANDROID_NDK             = $(HOME)/android-ndk-r23
export ANDROID_HOME     = $(HOME)/android-home
export ANDROID_SDK_ROOT = $(ANDROID_HOME)/cmdline-tools

gradle:
	@java \
		-classpath $(ANDROID_PROJECT)/gradle/wrapper/gradle-wrapper.jar \
		org.gradle.wrapper.GradleWrapperMain \
		-p $(ANDROID_PROJECT) \
		$(CMD)

ndk:
	@mkdir -p $(ANDROID_PROJECT)/app/libs
	@$(ANDROID_NDK)/ndk-build $(CMD) -j4 \
		NAME=$(NAME) \
		APP_BUILD_SCRIPT=Android.mk \
		APP_PLATFORM=android-26 \
		NDK_PROJECT_PATH=. \
		NDK_OUT=$(ANDROID_PROJECT)/build \
		NDK_LIBS_OUT=$(ANDROID_PROJECT)/app/libs \
		--no-print-directory \
		| grep -v 'fcntl(): Operation not supported'	

android-clean:
	@make --no-print-directory ndk CMD=clean
	@make --no-print-directory gradle CMD=clean

android-build: android-clean clear
	@mkdir -p android/app/src/main/java/group/matoya/lib
	@cp ../src/unix/linux/android/Matoya.java android/app/src/main/java/group/matoya/lib/
	@make --no-print-directory ndk
	@make --no-print-directory gradle CMD=build

android: android-build 
	@make --no-print-directory gradle CMD=installDebug
	@$(ANDROID_HOME)/platform-tools/adb shell am force-stop $(ANDROID_APPLICATION_ID)
	@$(ANDROID_HOME)/platform-tools/adb logcat -c
	@$(ANDROID_HOME)/platform-tools/adb shell am start -n $(ANDROID_APPLICATION_ID)/$(ANDROID_PACKAGE).MainActivity
	@$(ANDROID_HOME)/platform-tools/adb logcat $(ANDROID_APPLICATION_ID) | grep ' E \|MTY'

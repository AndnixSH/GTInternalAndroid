APP_ABI 			:= armeabi-v7a arm64-v8a
APP_OPTIM       	:= debug
APP_PLATFORM    	:= android-16
APP_STL         	:= c++_static
APP_OPTIM			:= release

ifneq ($(APP_OPTIM), debug)
  $(info APP_OPTIM is $(APP_OPTIM) ...)
  APP_LDFLAGS  += -Wl,--gc-sections,--strip-all
  APP_CFLAGS   += -fvisibility=hidden -fvisibility-inlines-hidden
  APP_CFLAGS   += -g0 -O3 -fomit-frame-pointer -ffunction-sections -fdata-sections
  APP_CPPFLAGS += -fvisibility=hidden -fvisibility-inlines-hidden
  APP_CPPFLAGS += -g0 -O3 -fomit-frame-pointer -ffunction-sections -fdata-sections
endif
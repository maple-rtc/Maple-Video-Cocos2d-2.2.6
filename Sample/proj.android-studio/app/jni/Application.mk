APP_STL := c++_static

APP_CPPFLAGS := -frtti -DCC_ENABLE_CHIPMUNK_INTEGRATION=1 -fsigned-char -std=gnu++11 -Wno-multichar
APP_CPPFLAGS += -Wno-deprecated-declarations
APP_CPPFLAGS += -Wno-unsequenced
APP_CPPFLAGS += -Wno-return-type
APP_CPPFLAGS += -Wno-c++11-narrowing
APP_CPPFLAGS += -Wno-inconsistent-missing-override
#一時的に
APP_CPPFLAGS += -Wno-#warnings

APP_LDFLAGS := -latomic

APP_CFLAGS := -fexceptions

APP_SHORT_COMMANDS := true

ifeq ($(NDK_DEBUG),1)
  APP_CPPFLAGS += -DCOCOS2D_DEBUG=1
  APP_CPPFLAGS += -DDEBUG
  APP_OPTIM := debug
else
  APP_CPPFLAGS += -DNDEBUG
  APP_OPTIM := release
endif


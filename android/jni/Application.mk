# Uncomment this if you're using STL in your project
# See CPLUSPLUS-SUPPORT.html in the NDK documentation for more information
APP_STL := stlport_static 
APP_ABI := armeabi-v7a armeabi
APP_OPTIM := release

# Enable c++11 extentions in source code
APP_CPPFLAGS += -std=c++11

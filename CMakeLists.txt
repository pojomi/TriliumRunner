cmake_minimum_required(VERSION 3.16)
project(TriliumRunner VERSION 1.0.0)

set(QT_MIN_VERSION "6.5.0")
set(KF6_MIN_VERSION "6.0.0")

find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS Core Gui)
find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS
    Runner
    I18n
    Config
)

kcoreaddons_add_plugin(triliumrunner
    SOURCES triliumrunner.cpp
    INSTALL_NAMESPACE "kf6/krunner"
)

target_link_libraries(triliumrunner
    KF6::Runner
    KF6::I18n
    KF6::ConfigCore
    Qt6::Core
    Qt6::Gui
    sqlite3
)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
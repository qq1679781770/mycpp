TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG+=c++11
CONFIG+=pthread
SOURCES += main.cpp

HEADERS += \
    ATM.hpp \
    ATMMsg.hpp \
    atm_machine.hpp \
    bank_machine.hpp \
    interface_machine.hpp


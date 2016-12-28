#ifndef INTERFACE_MACHINE_HPP
#define INTERFACE_MACHINE_HPP
#include"ATMMsg.hpp"
class interface_machine
{
private:
    messaging::receive incoming;
    std::mutex iom;
public:
    messaging::sender get_sender()
    {
        return incoming;
    }
    void done()
    {
        get_sender().send(messaging::queue_close());
    }
    void run()
    {
        try
        {
            for(;;)
            {
                incoming.wait().handle<issue_money>([&](issue_money const& msg)
                {
                    {
                        std::lock_guard<std::mutex>lk(iom);
                        std::cout<<"Issue"<<msg.amount<<std::endl;
                    }
                }).handle<display_insufficient_funds>([&](display_insufficient_funds const& msg)
                {
                    std::lock_guard<std::mutex>lk(iom);
                    std::cout<<"Insufficient Funds"<<std::endl;
                }).handle<display_enter_pin>([&](display_enter_pin const& msg)
                {
                    std::lock_guard<std::mutex>lk(iom);
                    std::cout<<"please enter your pin(0-9):"<<std::endl;
                }).handle<display_enter_card>([&](display_enter_card const& msg)
                {
                    std::lock_guard<std::mutex>lk(iom);
                    std::cout<<"please enter your card:"<<std::endl;
                }).handle<display_balance>([&](display_balance const& msg)
                {
                    std::lock_guard<std::mutex>lk(iom);
                    std::cout<<"your balance:"<<msg.amount<<std::endl;
                }).handle<display_withdrawal_options>([&](display_withdrawal_options const& msg)
                {
                    std::lock_guard<std::mutex>lk(iom);
                    std::cout<<"Withdraw?(w)"<<std::endl;
                    std::cout<<"display balance? (b)"<<std::endl;
                    std::cout<<"Cancel? (c)"<<std::endl;
                }).handle<display_withdrawal_cancelled>([&](display_withdrawal_cancelled const& msg)
                {
                    std::lock_guard<std::mutex>lk(iom);
                    std::cout<<"withdraw cancel"<<std::endl;
                }).handle<display_pin_incorrect_message>([&](display_pin_incorrect_message const& msg)
                {
                    std::lock_guard<std::mutex>lk(iom);
                    std::cout<<"PIN incorrect"<<std::endl;
                }).handle<eject_card>([&](eject_card const& msg)
                {
                    std::lock_guard<std::mutex>lk(iom);
                    std::cout<<"eject card"<<std::endl;
                });
            }
        }
        catch(messaging::queue_close const&)
        {

        }
    }
};

#endif // INTERFACE_MACHINE_HPP

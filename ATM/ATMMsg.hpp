#ifndef ATMMSG_HPP
#define ATMMSG_HPP
#include"ATM.hpp"
struct withdraw
{
    std::string account;
    unsigned amount;
    mutable messaging::sender atm_queue;
    withdraw(std::string const& _account,unsigned _amount,messaging::sender _atm_queue):account(_account),amount(_amount),atm_queue(_atm_queue){}
};
struct withdraw_ok{};
struct withdraw_denied{};
struct cancel_withdrawal
{
    std::string account;
    unsigned amount;
    cancel_withdrawal(std::string const& _account,unsigned _amount):account(_account),amount(_amount){}
};
struct withdrawal_processed
{
    std::string account;
    unsigned amount;
    withdrawal_processed(std::string const& _account,unsigned _amount):account(_account),amount(_amount){}
};
struct card_insert
{
    std::string account;
    explicit card_insert(std::string const& _account):account(_account){}
};
struct digit_pressed
{
    char digit;
    explicit digit_pressed(char _digit):digit(_digit){}
};
struct clear_last_pressed{};
struct eject_card{};
struct withdraw_pressed
{
    unsigned amount;
    explicit withdraw_pressed(unsigned _amount):amount(_amount){}
};
struct cancel_pressed{};
struct issue_money
{
    unsigned amount;
    issue_money(unsigned _amount):amount(_amount){}
};
struct verify_pin
{
    std::string account;
    std::string pin;
    mutable messaging::sender atm_queue;
    verify_pin(std::string const& _account,std::string const& _pin,messaging::send _atm_queue):account(_account),pin(_pin),atm_queue(_atm_queue){}
};
struct pin_verified{};
struct pin_incorrect{};
struct display_enter_pin{};
struct display_enter_card{};
struct display_insufficient_funds{};
struct display_withdrawal_cancelled{};
struct display_pin_incorrect_message{};
struct display_withdrawal_options{};
struct get_balance
{
    std::string account;
    mutable messaging::sender atm_queue;
    get_balance(std::string const& _account,messaging::sender _atm_queue):account(_account),atm_queue(_atm_queue){}
};
struct balance
{
    unsigned amount;
    explicit balance(unsigned _amount):amount(_amount){}
};
struct display_balance
{
    unsigned amount;
    explicit display_balance(unsigned _amount):amount(_amonut){}
};
struct balance_pressed{};

#endif // ATMMSG_HPP

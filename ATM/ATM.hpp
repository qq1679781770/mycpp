#ifndef ATM_HPP
#define ATM_HPP
#include<iostream>
#include<mutex>
#include<queue>
#include<memory>
#include<condition_variable>
namespace messaging
{
       struct message_base//message基础类
       {
           virtual ~message_base(){}
       };
       template<typename MSG>
       struct wrapp_message:message_base//message特化
       {
           MSG msg;
           explicit  wrapp_message(MSG const& _msg):msg(_msg){}
        };
       //消息队列
       class queue_message
       {
       private:
           std::mutex mx;
           std::condition_variable cv;
           std::queue<std::shared_ptr<message_base>>q_message;//存储指向message_base指针
       public:
           template<typename T>
           void push(T const& msg)
           {
               std::lock_guard<std::mutex>lk(mx);
               q_message.push(std::make_shared<wrapp_message<T>>(msg));
               cv.notify_all();
           }
           std::shared_ptr<message_base> wait_and_pop()
           {
               std::unique_lock<std::mutex>lk(mx);
               cv.wait(lk,[&]{return !q_message.empty();});
               auto res=q_message.front();
               q_message.pop();
               return res;
           }
       };
       //发送消息类
       class sender
       {
        private:
           queue_message* q;
        public:
           sender():q(nullptr){}
           explicit sender(queue_message* _q):q(_q){}
           template<typename Message>
           void send(Message const& msg)
           {
               if(q)
               {
                   q->push(msg);
               }
           }
       };
       //特化调度类
       template<typename PreviousDispatcher,typename Msg,typename Func>
       class TemplateDispatcher
       {
       private:
           queue_message* q;
           PreviousDispatcher* prev;
           Func f;
           bool chained;
           template<typename otherDispatcher,typename otherMsg,typename otherFunc>friend class TemplateDispatcher;
           TemplateDispatcher(TemplateDispatcher const&)=delete;
           TemplateDispatcher& operator=(TemplateDispatcher const&)=delete;
           void wait_and_displatch()
           {
               for(;;)
               {
                   auto msg=q->wait_and_pop();
                   if(dispatch(msg))
                       break;
               }
           }
           bool dispatch(std::shared_ptr<message_base> const& msg)
           {
               if(wrapp_message<Msg>* wrapper=dynamic_cast<wrapp_message<Msg>*>(msg.get()))
               {
                   f(wrapper->msg);
                   return true;
               }
               else
               {
                   prev->dispatch(msg);
               }
           }
       public:
           TemplateDispatcher(TemplateDispatcher&& other):q(other.q),chained(other.chained),prev(other.prev),f(std::move(other.f))
           {
               other.chained=true;
           }
           TemplateDispatcher(queue_message* _q,PreviousDispatcher*  _prev,Func _f):chained(false),q(_q),prev(_prev),f(std::forward<Func>(_f))
           {
               _prev->chained=true;
           }
           template<typename otherMsg,typename otherFunc>
           TemplateDispatcher<TemplateDispatcher,otherMsg,otherFunc> handle(otherFunc&& of)
           {
               return TemplateDispatcher<TemplateDispatcher,otherMsg,otherFunc>(q,this,std::forward<otherFunc>(of));
           }
           ~TemplateDispatcher()noexcept(false)
           {
               if(!chained)
               {
                   wait_and_displatch();
               }
           }
       };
       class queue_close{};//关闭队列消息
      //调度类
      class dispatcher
      {
      private:
          queue_message* q;
          bool chained;
          dispatcher(dispatcher const&)=delete;
          dispatcher& operator=(dispatcher const&)=delete;
          template<typename Dispatcher,typename Msg,typename Func>
          friend class TemplateDispatcher;
          void wait_and_dispatch()
          {
              for(;;)
              {
                  auto msg=q->wait_and_pop();
                  dispatch(msg);
              }
          }
          bool dispatch(std::shared_ptr<message_base> const& msg)
          {
              if(dynamic_cast<wrapp_message<queue_close>*>(msg.get()))
              {
                  throw queue_close();
              }
              return false;
          }
      public:
          dispatcher(dispatcher&& other):q(other.q),chained(other.chained)
          {
              other.chained=true;
          }
          explicit dispatcher(queue_message* _q):q(_q),chained(false){}
          template<typename Msg,typename Func>
          TemplateDispatcher<dispatcher,Msg,Func> handle(Func&& f)
          {
              return TemplateDispatcher<dispatcher,Msg,Func>(q,this,std::forward<Func>(f));
          }
          ~dispatcher()noexcept(false)
          {
              if(!chained)
              {
                  wait_and_dispatch();
              }
          }
      };
       //接收类
       class receive
       {
       private:
           queue_message q;
       public:
           operator sender()
           {
               return sender(&q);
           }
           dispatcher wait()
           {
               return dispatcher(&q);
           }
       };
}

#endif // ATM_HPP

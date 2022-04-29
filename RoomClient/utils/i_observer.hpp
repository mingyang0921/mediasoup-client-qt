
#pragma once

#include <memory>
#include <string>
#include "absl/types/optional.h"
#include "rtc_base/thread.h"

namespace vi {

    class INotification;

    class IObserver
    {
    public:
        IObserver() {}
        
        IObserver(const IObserver& observer) {}
        
        virtual ~IObserver() = default;
        
        virtual rtc::Thread* scheduleThread() = 0;
        
        virtual void notify(const std::shared_ptr<INotification>& nf) = 0;
        
        virtual bool shouldAccept(const std::shared_ptr<INotification>& nf) = 0;
        
        virtual bool equals(const IObserver& observer) const = 0;
        
        virtual bool isValid() = 0;
        
        virtual IObserver* clone() const = 0;
    };

}

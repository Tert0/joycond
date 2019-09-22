#ifndef JOYCOND_VIRT_CTLR_COMBINED
#define JOYCOND_VIRT_CTLR_COMBINED

#include "virt_ctlr.h"
#include "phys_ctlr.h"

#include <libevdev/libevdev.h>

class virt_ctlr_combined : public virt_ctlr
{
    private:
        phys_ctlr *physl;
        phys_ctlr *physr;
        int epoll_fd;
        struct libevdev_uinput *uidev;
        int uifd;

        void relay_events(struct phys_ctlr *phys);
        void handle_uinput_event();
    public:
        virt_ctlr_combined(phys_ctlr *physl, phys_ctlr *physr, int epoll_fd);
        virtual ~virt_ctlr_combined();

        virtual void handle_events(int fd);
        virtual bool contains_phys_ctlr(phys_ctlr const *ctlr) const;
        virtual bool contains_phys_ctlr(char const *devpath) const;
        virtual bool contains_fd(int fd) const;
        virtual std::vector<phys_ctlr *> get_phys_ctlrs();
        virtual int get_uinput_fd();
};

#endif
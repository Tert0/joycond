#include "virt_ctlr_double_pro.h"

#include <iostream>
#include <virt_ctlr_pro.h>

virt_ctlr_pro* virt_ctlr_double_pro::get_controller() const {
    if(this->selected_second) {
        return this->ctlr2;
    }
    return this->ctlr1;
}

bool virt_ctlr_double_pro::contains_phys_ctlr(const std::shared_ptr<phys_ctlr> ctlr) const {
    return this->ctlr1->contains_phys_ctlr(ctlr) || this->ctlr2->contains_phys_ctlr(ctlr);
}

bool virt_ctlr_double_pro::contains_phys_ctlr(const char *devpath) const {
    return this->ctlr1->contains_phys_ctlr(devpath) || this->ctlr2->contains_phys_ctlr(devpath);
}

bool virt_ctlr_double_pro::contains_fd(int fd) const {
    return this->ctlr1->contains_fd(fd) || this->ctlr2->contains_fd(fd);
}

std::vector<std::shared_ptr<phys_ctlr>> virt_ctlr_double_pro::get_phys_ctlrs() {
    return this->ctlr1->get_phys_ctlrs(); // both phys ctlr should be the same
}

void virt_ctlr_double_pro::remove_phys_ctlr(const std::shared_ptr<phys_ctlr> phys) {
    this->ctlr1->remove_phys_ctlr(phys);
    this->ctlr2->remove_phys_ctlr(phys);
}

void virt_ctlr_double_pro::add_phys_ctlr(std::shared_ptr<phys_ctlr> phys) {
    this->ctlr1->add_phys_ctlr(phys);
    this->ctlr2->add_phys_ctlr(phys);
}

phys_ctlr::Model virt_ctlr_double_pro::needs_model() {
    return this->ctlr1->needs_model();
}

virt_ctlr_double_pro::virt_ctlr_double_pro(const std::shared_ptr<phys_ctlr>& phys, epoll_mgr &epoll_manager) :
    epoll_manager(epoll_manager),
    subscriber(nullptr),
    phys(phys),
    selected_second(false)
{
    const std::function interceptor = [=](input_event* e) {
        if(e->type == EV_KEY && e->code == BTN_THUMBR) {
            if(e->value == 1) {
                this->selected_second = !this->selected_second;
                this->phys->set_player_leds_to_player(this->selected_second ? this->player2 : this->player1);
                if(this->selected_second) {
                    printf("Switched to second Controller\n");
                } else {
                    printf("Switched to first Controller\n");
                }

            }
            return false;
        }
       return true;
    };

    this->ctlr1 = new virt_ctlr_pro(phys, epoll_manager, false, interceptor);
    this->ctlr2 = new virt_ctlr_pro(phys, epoll_manager, false, interceptor);

    this->subscriber = std::make_shared<epoll_subscriber>(std::vector({this->ctlr1->get_uinput_fd(), this->ctlr2->get_uinput_fd()}),
                                                    [=](int event_fd){this->handle_events(event_fd);});

    epoll_manager.add_subscriber(this->subscriber);
}

void virt_ctlr_double_pro::handle_events(const int fd) {
    virt_ctlr_pro* ctlr = this->get_controller();
    if(ctlr->contains_fd(fd)) {
        ctlr->handle_events(fd);
    }
}

void virt_ctlr_double_pro::set_player_leds_to_players(const unsigned int player1, const unsigned int player2) {
    this->player1 = player1;
    this->player2 = player2;

    this->phys->set_player_leds_to_player(this->selected_second ? this->player2 : this->player1);
}

virt_ctlr_double_pro::~virt_ctlr_double_pro() {
    this->epoll_manager.remove_subscriber(this->subscriber);
    delete this->ctlr1;
    delete this->ctlr2;
}

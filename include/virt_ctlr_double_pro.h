#ifndef VIRT_CTRL_DOUBLE_PRO_H
#define VIRT_CTRL_DOUBLE_PRO_H
#include <epoll_mgr.h>
#include <virt_ctlr.h>
#include <virt_ctlr_pro.h>

class virt_ctlr_double_pro : public virt_ctlr {
private:
    epoll_mgr& epoll_manager;
    std::shared_ptr<epoll_subscriber> subscriber;
    std::shared_ptr<phys_ctlr> phys;
    virt_ctlr_pro* ctlr1;
    virt_ctlr_pro* ctlr2;
    bool selected_second;
    unsigned int player1 = 0;
    unsigned int player2 = 0;

    virt_ctlr_pro* get_controller() const;


    bool contains_phys_ctlr(const std::shared_ptr<phys_ctlr> ctlr) const override;
    bool contains_phys_ctlr(const char *devpath) const override;
    bool contains_fd(int fd) const override;
    std::vector<std::shared_ptr<phys_ctlr>> get_phys_ctlrs() override;
    void remove_phys_ctlr(const std::shared_ptr<phys_ctlr> phys) override;
    void add_phys_ctlr(std::shared_ptr<phys_ctlr> phys) override;
    phys_ctlr::Model needs_model();

public:
    virt_ctlr_double_pro(const std::shared_ptr<phys_ctlr>& phys, epoll_mgr& epoll_manager);
    void handle_events(int fd);
    void set_player_leds_to_players(unsigned int player1, unsigned int player2);

    ~virt_ctlr_double_pro();
};

#endif //VIRT_CTRL_DOUBLE_PRO_H
